#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <tradingcore2/client/train2.h>
#include <tradingcore2/exchange.h>
#include <tradingcore2/proto/tradingcore2.grpc.pb.h>
#include <tradingcore2/train.h>
#include <tradingcore2/utils.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "./utils.h"

CR2BEGIN

class TrainClient2Pool {
 public:
  struct Client {
    std::shared_ptr<grpc::Channel> channel;
    std::unique_ptr<tradingcore2pb::TradingCore2Service::Stub> stub;
    std::string token;
    int maxTaskNums;
  };

  typedef std::queue<tradingcore2pb::RequestTrain*> TrainTaskList;
  typedef std::vector<std::thread*> ThreadList;
  typedef std::vector<Client*> ClientList;

 public:
  TrainClient2Pool() : m_pCfg(NULL), m_isStop(false) {}
  ~TrainClient2Pool() { release(); }

 public:
  void init(const Config& cfg) {
    std::unique_lock<std::mutex> lock(m_mtx);

    // assert(m_threads.empty());
    assert(m_pCfg == NULL);

    m_pCfg = &cfg;

    for (auto it = cfg.servs.begin(); it != cfg.servs.end(); ++it) {
      auto cc = new Client();

      cc->channel = grpc::CreateChannel(it->host.c_str(),
                                        grpc::InsecureChannelCredentials());
      cc->stub = tradingcore2pb::TradingCore2Service::NewStub(cc->channel);
      cc->token = it->token;

      auto cn = _getServerInfo(*cc);
      assert(cn > 0);

      cc->maxTaskNums = cn;

      m_clients.push_back(cc);

      // for (int i = 0; i < cc->maxTaskNums; ++i) {
      //   std::thread* pt =
      //       new std::thread(&TrainClient2Pool::onWorker, this, cc);

      //   m_threads.push_back(pt);
      // }
    }
  }

  void start() {
    {
      std::unique_lock<std::mutex> lock(m_mtx);
      m_isStop = false;
    }

    m_curThreadNums = 0;

    for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
      for (int i = 0; i < (*it)->maxTaskNums; ++i) {
        m_curThreadNums++;
        std::thread t(&TrainClient2Pool::onWorker, this, *it);

        t.detach();
      }
    }

    // for (auto it = m_threads.begin(); it != m_threads.end(); ++it) {
    //   m_curThreadNums++;
    //   (*it)->detach();
    // }

    while (true) {
      if (m_curThreadNums == 0) {
        return;
      }

      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }

  void release() {
    std::unique_lock<std::mutex> lock(m_mtx);

    m_isStop = true;

    // __releaseThreads();
    __releaseResults();
    __releaseClients();
    __releaseTasks();
    __releasePool();
  }

  void saveResults(const char* fn) {
    std::unique_lock<std::mutex> lock(m_mtx);
    saveTrainResult(fn, m_results);
  }

 public:
  // Assembles the client's payload and sends it to the server.
  void train(const char* exchangeName, const char* assetsName,
             const char* indicatorName, const char* outputPath, Money invest,
             int avgtimes, IndicatorDataValue off0, IndicatorDataValue off1,
             IndicatorDataValue off2, IndicatorDataValue maxoff2,
             float minValidReturn, IndicatorDataValue minval,
             IndicatorDataValue maxval, IndicatorDataValue cv0,
             IndicatorDataValue cv0off) {
    {
      std::unique_lock<std::mutex> lock(m_mtx);
      if (m_isStop) {
        return;
      }
    }

    auto rt = this->newRequestTrain();

    auto req = rt->mutable_train();

    req->set_exchangename(exchangeName);
    req->set_assetsname(assetsName);
    req->set_invest(invest);
    req->set_outputpath(outputPath);
    req->set_minvalidreturn(minValidReturn);

    auto si2 = req->mutable_si2();
    si2->set_indicatorname(indicatorName);
    si2->set_avgtimes(avgtimes);
    si2->set_off0(off0);
    si2->set_off1(off1);
    si2->set_off2(off2);
    si2->set_maxoff2(maxoff2);
    si2->set_minval(minval);
    si2->set_maxval(maxval);
    si2->set_cv0(cv0);
    si2->set_cv0off(cv0off);

    this->pushRequestTrain(rt);
  }

 private:
  void onWorker(Client* pClient) {
    printf("thread start...\n");

    while (true) {
      auto task = popRequestTrain();
      if (task == NULL) {
        break;
      }

      setRequest_Token(task, pClient->token.c_str());

      tradingcore2pb::ReplyTrain reply;

      grpc::ClientContext context;

      grpc::Status status = pClient->stub->train(&context, *task, &reply);
      printf("task end.\n");

      if (status.ok()) {
        for (auto i = 0; i < reply.train().nodes_size(); ++i) {
          TrainResult tr;
          setTradeHistory2TrainResult(tr, reply.train().nodes(i));

          this->addTrainResult(tr);
        }
      }

      deleteRequestTrain(task);
    }

    printf("thread end.\n");
    m_curThreadNums--;
  }

 private:
  int _getServerInfo(Client& client) {
    tradingcore2pb::RequestServerInfo request;
    setRequest_Token(&request, client.token.c_str());

    tradingcore2pb::ReplyServerInfo reply;

    grpc::ClientContext context;

    grpc::Status status = client.stub->getServerInfo(&context, request, &reply);

    printf("_getServerInfo %d\n", reply.maxtasks());
    printf("_getServerInfo status %d\n", status.ok());

    if (status.ok()) {
      return reply.maxtasks();
    }

    return 0;
  }

  // void __releaseThreads() {
  //   for (auto it = m_threads.begin(); it != m_threads.end(); ++it) {
  //     delete *it;
  //   }

  //   m_threads.clear();
  // }

  void __releaseClients() {
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
      delete *it;
    }

    m_clients.clear();
  }

  tradingcore2pb::RequestTrain* newRequestTrain() {
    tradingcore2pb::RequestTrain* rt = NULL;

    {
      std::unique_lock<std::mutex> lock(m_mtx);
      if (m_pool.empty()) {
        return new tradingcore2pb::RequestTrain();
      }

      rt = m_pool.back();
    }

    assert(rt != NULL);
    rt->Clear();

    return rt;
  }

  void deleteRequestTrain(tradingcore2pb::RequestTrain* rt) {
    assert(rt != NULL);

    std::unique_lock<std::mutex> lock(m_mtx);
    m_pool.push(rt);
  }

  void __releasePool() {
    while (!m_pool.empty()) {
      auto pt = m_pool.front();
      delete pt;
      m_pool.pop();
    }
  }

  void pushRequestTrain(tradingcore2pb::RequestTrain* rt) {
    assert(rt != NULL);

    std::unique_lock<std::mutex> lock(m_mtx);
    m_tasks.push(rt);
  }

  tradingcore2pb::RequestTrain* popRequestTrain() {
    std::unique_lock<std::mutex> lock(m_mtx);
    if (m_tasks.empty()) {
      return NULL;
    }

    auto rt = m_tasks.front();
    m_tasks.pop();

    return rt;
  }

  void __releaseTasks() {
    while (!m_tasks.empty()) {
      auto pt = m_tasks.front();
      delete pt;
      m_tasks.pop();
    }
  }

  void addTrainResult(TrainResult& tr) {
    std::unique_lock<std::mutex> lock(m_mtx);
    m_results.push_back(tr);
  }

  void __releaseResults() {
    for (auto it = m_results.begin(); it != m_results.end(); ++it) {
      if (it->data != NULL) {
        delete (tradingcore2pb::TrainNodeResult*)it->data;
      }
    }

    m_results.clear();
  }

 private:
  const Config* m_pCfg;
  // std::shared_ptr<grpc::Channel> m_channel;

  std::mutex m_mtx;

  TrainTaskList m_pool;
  TrainTaskList m_tasks;

  // ThreadList m_threads;
  ClientList m_clients;

  bool m_isStop;
  std::atomic<int> m_curThreadNums;

  TrainResultList m_results;
};

// startTrainSingleIndicator2ExPool -
// 单独一个indicator的策略，考虑方向，grpc调用
bool startTrainSingleIndicator2ExPool(
    const Config& cfg, Exchange& exchange, const char* assetsName,
    const char* indicatorName, const char* outputPath, Money invest,
    int avgtimes, IndicatorDataValue off0, IndicatorDataValue off1,
    IndicatorDataValue off2, IndicatorDataValue maxoff2, float minValidReturn) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  TrainClient2Pool client;

  client.init(cfg);

  IndicatorDataValue minval, maxval;
  if (!calcIndicatorRange(exchange, assetsName, indicatorName, avgtimes, minval,
                          maxval)) {
    return false;
  }

  TrainResultList lst;

  minval = scaleValue(minval, off0);
  maxval = scaleValue(maxval, off0);
  auto maxtimes = 0;
  int curtimes = 0;

  for (auto cv0 = minval; cv0 <= maxval; cv0 += off0) {
    for (auto cv0off = off2; cv0off <= maxoff2; cv0off += off2) {
      maxtimes++;
    }
  }

  auto st = time(NULL);

  for (auto cv0 = minval; cv0 <= maxval; cv0 += off0) {
    for (auto cv0off = off2; cv0off <= maxoff2; cv0off += off2) {
      client.train(exchange.getTypeName(), assetsName, indicatorName,
                   outputPath, invest, avgtimes, off0, off1, off2, maxoff2,
                   minValidReturn, minval, maxval, cv0, cv0off);

      curtimes++;

      auto ct = time(NULL);
      printf("Progress is %.2f%% (%d/%d)\n", 100.0f * curtimes / maxtimes,
             curtimes, maxtimes);

      if (ct > st) {
        printf("current time is %d s, last time is %d s\n", (int)(ct - st),
               (int)((ct - st) * maxtimes / curtimes) - (int)(ct - st));
      }
    }
  }

  client.start();
  client.saveResults("../output/train.csv");
  //   for (int i = 0; i < 100; i++) {
  //     // std::string user("world " + std::to_string(i));
  //     client.train(user);  // The actual RPC call!
  //   }

  //   std::cout << "Press control-c to quit" << std::endl << std::endl;
  // thread_.join();  // blocks forever

  return true;
}

CR2END