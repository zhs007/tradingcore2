#include <stdio.h>
#include <tradingcore2/tradingcore2.h>

void onCalcPNL(::grpc::Status status, const ::tradingpb::RequestCalcPNL& req,
               const ::tradingpb::ReplyCalcPNL& res) {
  LOG(INFO) << "onCalcPNL " << status.error_code();
}

void buyandhold(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("cnfunds");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("bah");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("cnfunds");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_indicator("buyandhold");
  buy0->add_vals(3);
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_initmoney(10000);
  bp->set_permoney(1);

  ::tradingpb::ReplyCalcPNL res;
  auto status = client.clacPNL(params, res);

  // client.waitStop();

  // ::tradingpb::ReplyServerInfo res;
  // auto status = client.getServerInfo(res);
  LOG(INFO) << "calcPNL " << status.error_code();

  if (status.ok()) {
    LOG(INFO) << res.DebugString();
  }

  // auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  // tr2::startTrainSingleIndicator2ExPool(cfg, *cnfund, "110022", "rsi",
  //                                       "../output", 10000, 5, 5 /* off0 */,
  //                                       5, 5 /* off2 */, 10, 2);
}

void aipWeekDay(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("cnfunds");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("aip");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("cnfunds");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_indicator("weekday");
  buy0->add_vals(3);
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_aipmoney(10000);
  // bp->set_permoney(1);

  ::tradingpb::ReplyCalcPNL res;
  auto status = client.clacPNL(params, res);

  // client.waitStop();

  // ::tradingpb::ReplyServerInfo res;
  // auto status = client.getServerInfo(res);
  LOG(INFO) << "calcPNL " << status.error_code();

  if (status.ok()) {
    LOG(INFO) << res.DebugString();
  }

  // auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  // tr2::startTrainSingleIndicator2ExPool(cfg, *cnfund, "110022", "rsi",
  //                                       "../output", 10000, 5, 5 /* off0 */,
  //                                       5, 5 /* off2 */, 10, 2);
}

void aipMonthDay(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("cnfunds");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("aip");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("cnfunds");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_indicator("monthday");
  buy0->add_vals(3);
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_aipmoney(10000);
  // bp->set_permoney(1);

  ::tradingpb::ReplyCalcPNL res;
  auto status = client.clacPNL(params, res);

  // client.waitStop();

  // ::tradingpb::ReplyServerInfo res;
  // auto status = client.getServerInfo(res);
  LOG(INFO) << "calcPNL " << status.error_code();

  if (status.ok()) {
    LOG(INFO) << res.DebugString();
  }

  // auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  // tr2::startTrainSingleIndicator2ExPool(cfg, *cnfund, "110022", "rsi",
  //                                       "../output", 10000, 5, 5 /* off0 */,
  //                                       5, 5 /* off2 */, 10, 2);
}

int main(int argc, char* argv[]) {
  putenv("TZ=UTC");
  tr2::LogHelper log(argv[0]);

  printf("tr2client starting...\n");
  printf("version is %s\n", tr2::getVersion());

  if (argc != 2) {
    printf("please type tr2client cfgfile.");

    return -1;
  }

  tr2::regAllIndicators();
  tr2::regAllExchanges();
  tr2::regAllCtrlConditionHelper();
  tr2::regAllStrategy();

  tr2::Config cfg;
  tr2::loadConfig(cfg, argv[1]);

  auto mgr = tr2::ExchangeMgr::getSingleton();
  mgr->init(cfg);

  // buyandhold(cfg);
  // aipWeekDay(cfg);
  aipMonthDay(cfg);

  return 0;
}