#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

void startServ(const tr2::Config &cfg)
{
  auto pServ = tr2::newNodeServer2(cfg);
  //   pServ->init(cfg);
  pServ->run();
}

int main(int argc, char *argv[])
{
  putenv("TZ=UTC");
  tr2::LogHelper log(argv[0]);
  char buf[1024];

  LOG(INFO) << "tr2serv (" << tr2::getVersion() << ") starting...";
  LOG(INFO) << "curpath is " << getcwd(buf, 1024);
  // LOG(INFO) << "tr2serv starting...";
  // printf("tr2serv starting...\n");
  // printf("version is %s\n", tr2::getVersion());

  std::string strparams;
  if (argc != 2)
  {
    strparams = "../../../cfg/config.yaml";

    // LOG(ERROR) << "tr2serv argv file.";
    // printf("please type server cfgfile.\n");

    // return -1;
  }
  else
  {
    strparams = argv[1];
  }

  tr2::initTALib();
  tr2::regAllIndicators();
  tr2::regAllExchanges();
  tr2::regAllCtrlConditionHelper();
  tr2::regAllStrategy();

  tr2::Config cfg;
  tr2::loadConfig(cfg, strparams.c_str());

  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);

  auto mgrTasks = tr2::TasksMgr::getSingleton();
  mgrTasks->init(cfg);

  std::thread worker([cfg]()
                     {
                       tr2::WorkerMgr mgr(cfg);
                       while (true)
                       {
                         tr2::reqTasks(cfg.trdb2Serv.c_str(), cfg.trdb2Token.c_str(), &mgr);

                         if (mgr.canFinish())
                         {
                           return;
                         }

                         std::this_thread::sleep_for(std::chrono::seconds(5));
                       }
                     });

  // LOG(INFO) << "tr2serv started.";

  // startServ(cfg);

  // LOG(INFO) << "tr2serv exit.";

  worker.join();

  LOG(INFO) << "worker exit.";

  tr2::releaseTALib();

  return 0;
}