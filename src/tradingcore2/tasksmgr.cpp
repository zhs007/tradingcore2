
#include <tradingcore2/ctrlconditionmgr.h>
#include <tradingcore2/exchangemgr.h>
#include <tradingcore2/pnl2.h>
#include <tradingcore2/strategy.h>
#include <tradingcore2/strategymgr.h>
#include <tradingcore2/tasksmgr.h>
#include <tradingcore2/utils.h>

CR2BEGIN

TasksMgr* TasksMgr::getSingleton() {
  static TasksMgr s_mgr;

  return &s_mgr;
}

void TasksMgr::init(const Config& cfg) {
  long cpus = sysconf(_SC_NPROCESSORS_ONLN);  // get # of online cores
  if (cfg.taskNums == 0) {
    m_maxTaskNums = cpus;
  } else if (cfg.taskNums >= cpus) {
    m_maxTaskNums = cpus;
  } else if (cfg.taskNums > 0) {
    m_maxTaskNums = cfg.taskNums;
  } else if (cfg.taskNums < 0) {
    if (-cfg.taskNums < cpus) {
      m_maxTaskNums = cpus + cfg.taskNums;
    } else {
      m_maxTaskNums = 1;
    }
  } else {
    assert(false && "TradingNode2Impl::init() error");
  }

  LOG(INFO) << "max tasks num :" << m_maxTaskNums;
}

void TasksMgr::release() {}

// runTradingTask - runTradingTask
::grpc::Status TasksMgr::runTradingTask(
    const ::tradingpb::SimTradingParams* params,
    ::tradingpb::PNLData* pnldata) {
  assert(params != NULL);
  assert(pnldata != NULL);

  LOG(INFO) << "runTradingTask...";

  logProtobuf("runTradingTask", *params);

  //   if (!isValidTokens(request, response, *m_pCfg)) {
  //     LOG(ERROR) << "runTradingTask:isValidTokens " <<
  //     request->basicrequest().token();

  //     return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "invalid
  //     token");
  //   }

  std::string mainMarket = "";
  for (auto i = 0; i < params->assets_size(); ++i) {
    auto ca = params->assets(i);
    if (mainMarket != "") {
      if (mainMarket != ca.market()) {
        return grpc::Status(grpc::StatusCode::UNKNOWN,
                            "I get a defferent market.");
      }
    } else {
      mainMarket = ca.market();
    }
  }

  auto exchange =
      tr2::ExchangeMgr::getSingleton()->newExchange(mainMarket.c_str());
  if (exchange == NULL) {
    LOG(ERROR) << "runTradingTask:getExchange " << mainMarket;

    return grpc::Status(grpc::StatusCode::UNKNOWN, "I can't get exchange");
  }

  if (params->startts() > 0 && params->endts() > 0) {
    if (params->startts() >= params->endts()) {
      LOG(ERROR) << "runTradingTask:invalid timestamp" << params->startts()
                 << " " << params->endts();

      return grpc::Status(grpc::StatusCode::UNKNOWN, "Invalid timestamp");
    }
  }

  std::string mainasset;
  for (auto i = 0; i < params->assets_size(); ++i) {
    auto ca = params->assets(i);
    exchange->loadData(ca.code().c_str(), params->startts(), params->endts(),
                       params->offset());

    if (i == params->mainassetindex()) {
      mainasset = ca.code();
    }
  }

  for (auto i = 0; i < params->candles_size(); ++i) {
    auto cs = params->candles(i);
    exchange->mergeCandles(&cs);
  }

  if (params->mainassetindex() < 0) {
    exchange->rebuildTimeStampList(NULL);
  } else {
    exchange->rebuildTimeStampList(mainasset.c_str());
  }

  LOG(ERROR) << "runTradingTask:rebuildTimeStampList ok!";

  auto pWallet = new tr2::Wallet(*exchange);

  auto sts = params->startts();
  if (sts <= 0) {
    sts = exchange->getFirstTimeStamp();
  }

  tr2::PNL2 pnl2;

  for (auto i = 0; i < params->strategies_size(); ++i) {
    auto cs = params->strategies(i);
    auto ret = CtrlConditionMgr::getSingleton()->isValidStrategy(cs);
    if (ret == -1) {
      return grpc::Status(grpc::StatusCode::UNKNOWN, "isValidStrategy");
    }

    auto strategy = tr2::StrategyMgr::getSingleton()->newStrategy(
        cs.name().c_str(), *pWallet, *exchange);
    if (strategy == NULL) {
      return grpc::Status(grpc::StatusCode::UNKNOWN, "non strategy");
    }

    strategy->init(cs);
    strategy->simulateTrading();

    strategy->buildIndicators(*params, pnl2);
  }

  LOG(ERROR) << "runTradingTask:simulateTrading ok!";

  // tr2::PNL2 pnl2;
  pWallet->buildPNL2(*params, pnl2);

  LOG(ERROR) << "runTradingTask:buildPNL2 ok!";

  //   auto pPNLData = response->add_pnl();
  pnldata->CopyFrom(pnl2.m_data);

  //   auto ni = response->mutable_nodeinfo();
  //   ni->set_curtasks(this->m_curTaskNums - 1);
  //   ni->set_maxtasks(this->m_maxTaskNums);

  delete pWallet;
  tr2::ExchangeMgr::getSingleton()->deleteExchange(exchange);

  LOG(INFO) << "TradingNode2Impl::runTradingTask end.";

  //   auto et = std::time(0);

  //   response->set_runseconds(et - ct);

  return grpc::Status::OK;
}

// runTask - runTask
::grpc::Status TasksMgr::runTask(const ::tradingpb::SimTradingParams* params,
                                 ::tradingpb::PNLData* pnldata) {
  auto cn = this->m_curTaskNums.load(std::memory_order_relaxed);
  if (cn >= this->m_maxTaskNums) {
    return grpc::Status(grpc::StatusCode::UNKNOWN, "no worker.");
  }

  while (!this->m_curTaskNums.compare_exchange_weak(
      cn, cn + 1, std::memory_order_release, std::memory_order_relaxed)) {
    cn = this->m_curTaskNums.load(std::memory_order_relaxed);
    if (cn >= this->m_maxTaskNums) {
      return grpc::Status(grpc::StatusCode::UNKNOWN, "no worker(1).");
    }
  }

  auto status = this->runTradingTask(params, pnldata);

  cn = this->m_curTaskNums.load(std::memory_order_relaxed);
  if (cn <= 0) {
    return grpc::Status(grpc::StatusCode::UNKNOWN, "invalid worker number.");
  }

  while (!this->m_curTaskNums.compare_exchange_weak(
      cn, cn - 1, std::memory_order_release, std::memory_order_relaxed)) {
    cn = this->m_curTaskNums.load(std::memory_order_relaxed);
    if (cn <= 0) {
      return grpc::Status(grpc::StatusCode::UNKNOWN,
                          "invalid worker number(1).");
    }
  }

  return status;
}

CR2END