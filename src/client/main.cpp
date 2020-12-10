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
  asset0->set_market("jrj");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("bah");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jrj");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("buyandhold");
  buy0->add_vals(3);
  auto bp = strategy0->mutable_paramsbuy();
  // bp->set_initmoney(10000);
  bp->set_perinitmoney(1);
  auto ip = strategy0->mutable_paramsinit();
  ip->set_money(10000);

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
  asset0->set_market("jrj");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("aip");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jrj");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("weekday");
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
  asset0->set_market("jrj");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("aip");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jrj");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("monthday");
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

void normalWeekDay(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("jrj");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jrj");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("weekday");
  buy0->add_vals(3);
  auto sell0 = strategy0->add_sell();
  sell0->set_name("weekday");
  sell0->add_vals(4);
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_perhandmoney(1);
  auto sp = strategy0->mutable_paramssell();
  sp->set_pervolume(1);
  auto ip = strategy0->mutable_paramsinit();
  ip->set_money(10000);

  params.set_startts(tr2::str2timestampUTC("20200101", "%Y%m%d"));
  params.set_endts(tr2::str2timestampUTC("20200201", "%Y%m%d"));

  ::tradingpb::ReplyCalcPNL res;
  auto status = client.clacPNL(params, res);

  // client.waitStop();

  // ::tradingpb::ReplyServerInfo res;
  // auto status = client.getServerInfo(res);
  LOG(INFO) << "calcPNL " << status.error_code();

  if (status.ok()) {
    tr2::logProtobuf("reply ", res);
    // LOG(INFO) << res.DebugString();
  }

  // auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  // tr2::startTrainSingleIndicator2ExPool(cfg, *cnfund, "110022", "rsi",
  //                                       "../output", 10000, 5, 5 /* off0 */,
  //                                       5, 5 /* off2 */, 10, 2);
}

void normalWeekDay2(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("jrj");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jrj");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("weekdayex");
  buy0->add_vals(5);
  auto sell0 = strategy0->add_sell();
  sell0->set_name("weekdayex");
  sell0->add_vals(1);
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_perhandmoney(1);
  auto sp = strategy0->mutable_paramssell();
  sp->set_pervolume(1);
  // auto ip = strategy0->mutable_paramsinit();
  // ip->set_money(10000);
  auto aip = strategy0->mutable_paramsaip();
  aip->set_money(10000);
  aip->set_type(tradingpb::AIPTT_WEEKDAY);
  aip->set_day(3);

  params.set_startts(tr2::str2timestampUTC("20200101", "%Y%m%d"));
  params.set_endts(tr2::str2timestampUTC("20200301", "%Y%m%d"));

  ::tradingpb::ReplyCalcPNL res;
  auto status = client.clacPNL(params, res);

  // client.waitStop();

  // ::tradingpb::ReplyServerInfo res;
  // auto status = client.getServerInfo(res);
  LOG(INFO) << "calcPNL " << status.error_code();

  if (status.ok()) {
    tr2::logProtobuf("reply ", res);
    // LOG(INFO) << res.DebugString();
  }

  // auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  // tr2::startTrainSingleIndicator2ExPool(cfg, *cnfund, "110022", "rsi",
  //                                       "../output", 10000, 5, 5 /* off0 */,
  //                                       5, 5 /* off2 */, 10, 2);
}

void normalWeekDay3(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("jrj");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jrj");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("weekdayex");
  buy0->add_vals(5);
  auto sell0 = strategy0->add_sell();
  sell0->set_name("weekdayex");
  sell0->add_vals(1);
  auto sell1 = strategy0->add_sell();
  sell1->set_name("indicatorsp");
  sell1->add_operators("down");
  sell1->add_strvals("ema.5");
  // sell0->add_vals(1);
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_perhandmoney(1);
  auto sp = strategy0->mutable_paramssell();
  sp->set_pervolume(1);
  // auto ip = strategy0->mutable_paramsinit();
  // ip->set_money(10000);
  auto aip = strategy0->mutable_paramsaip();
  aip->set_money(10000);
  aip->set_type(tradingpb::AIPTT_WEEKDAY);
  aip->set_day(3);

  params.set_startts(tr2::str2timestampUTC("20200101", "%Y%m%d"));
  params.set_endts(tr2::str2timestampUTC("20200301", "%Y%m%d"));

  ::tradingpb::ReplyCalcPNL res;
  auto status = client.clacPNL(params, res);

  // client.waitStop();

  // ::tradingpb::ReplyServerInfo res;
  // auto status = client.getServerInfo(res);
  LOG(INFO) << "calcPNL " << status.error_code();

  if (status.ok()) {
    tr2::logProtobuf("reply ", res);
    // LOG(INFO) << res.DebugString();
  }

  // auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  // tr2::startTrainSingleIndicator2ExPool(cfg, *cnfund, "110022", "rsi",
  //                                       "../output", 10000, 5, 5 /* off0 */,
  //                                       5, 5 /* off2 */, 10, 2);
}

void normalWeekDay5(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("jrj");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jrj");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("weekday");
  buy0->add_vals(5);
  // auto buy1 = strategy0->add_buy();
  // buy1->set_name("indicatorsp");
  // buy1->add_operators("up");
  // buy1->add_strvals("ema.5");

  auto buy2 = strategy0->add_buy();
  buy2->set_name("weekday");
  buy2->add_vals(1);
  buy2->set_group(1);
  // auto buy3 = strategy0->add_buy();
  // buy3->set_name("indicatorsp");
  // buy3->add_operators("up");
  // buy3->add_strvals("ema.5");
  // buy3->set_group(1);

  auto sell0 = strategy0->add_sell();
  sell0->set_name("weekday");
  sell0->add_vals(2);

  // auto sell1 = strategy0->add_sell();
  // sell1->set_name("indicatorsp");
  // sell1->add_operators("down");
  // sell1->add_strvals("ema.5");
  // sell0->add_vals(1);
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_perhandmoney(1);
  auto sp = strategy0->mutable_paramssell();
  sp->set_pervolume(1);
  // auto ip = strategy0->mutable_paramsinit();
  // ip->set_money(10000);
  auto aip = strategy0->mutable_paramsaip();
  aip->set_money(10000);
  aip->set_type(tradingpb::AIPTT_WEEKDAY);
  aip->set_day(1);

  params.set_startts(tr2::str2timestampUTC("20200101", "%Y%m%d"));
  params.set_endts(tr2::str2timestampUTC("20200301", "%Y%m%d"));

  ::tradingpb::ReplyCalcPNL res;
  auto status = client.clacPNL(params, res);

  // client.waitStop();

  // ::tradingpb::ReplyServerInfo res;
  // auto status = client.getServerInfo(res);
  LOG(INFO) << "calcPNL " << status.error_code();

  if (status.ok()) {
    tr2::logProtobuf("reply ", res);
    // LOG(INFO) << res.DebugString();
  }

  // auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  // tr2::startTrainSingleIndicator2ExPool(cfg, *cnfund, "110022", "rsi",
  //                                       "../output", 10000, 5, 5 /* off0 */,
  //                                       5, 5 /* off2 */, 10, 2);
}

void normalROC1(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("jrj");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jrj");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("indicatorsv");
  buy0->add_vals(-0.015);
  buy0->add_operators("<=");
  buy0->add_strvals("roc.1");
  auto sell0 = strategy0->add_sell();
  sell0->set_name("indicatorsv");
  sell0->add_vals(0.02);
  sell0->add_operators(">=");
  sell0->add_strvals("roc.1");
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_perhandmoney(1);
  auto sp = strategy0->mutable_paramssell();
  sp->set_pervolume(1);
  auto ip = strategy0->mutable_paramsinit();
  ip->set_money(10000);
  // auto aip = strategy0->mutable_paramsaip();
  // aip->set_money(10000);
  // aip->set_type(tradingpb::AIPTT_WEEKDAY);
  // aip->set_day(3);

  params.set_startts(tr2::str2timestampUTC("20200101", "%Y%m%d"));
  params.set_endts(tr2::str2timestampUTC("20200301", "%Y%m%d"));

  ::tradingpb::ReplyCalcPNL res;
  auto status = client.clacPNL(params, res);

  // client.waitStop();

  // ::tradingpb::ReplyServerInfo res;
  // auto status = client.getServerInfo(res);
  LOG(INFO) << "calcPNL " << status.error_code();

  if (status.ok()) {
    tr2::logProtobuf("reply ", res);
    // LOG(INFO) << res.DebugString();
  }

  // auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  // tr2::startTrainSingleIndicator2ExPool(cfg, *cnfund, "110022", "rsi",
  //                                       "../output", 10000, 5, 5 /* off0 */,
  //                                       5, 5 /* off2 */, 10, 2);
}

void normalEMA5(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("jrj");
  asset0->set_code("001631");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jrj");
  asset1->set_code("001631");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("indicatorsp");
  // buy0->add_vals(-0.015);
  buy0->add_operators("upcross");
  buy0->add_strvals("ema.5");
  auto sell0 = strategy0->add_sell();
  sell0->set_name("indicatorsp");
  // sell0->add_vals(0.02);
  sell0->add_operators("downcross");
  sell0->add_strvals("ema.5");
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_perhandmoney(1);
  auto sp = strategy0->mutable_paramssell();
  sp->set_pervolume(1);
  auto ip = strategy0->mutable_paramsinit();
  ip->set_money(10000);
  // auto aip = strategy0->mutable_paramsaip();
  // aip->set_money(10000);
  // aip->set_type(tradingpb::AIPTT_WEEKDAY);
  // aip->set_day(3);

  params.set_startts(tr2::str2timestampUTC("20200101", "%Y%m%d"));
  params.set_endts(tr2::str2timestampUTC("20200301", "%Y%m%d"));

  ::tradingpb::ReplyCalcPNL res;
  auto status = client.clacPNL(params, res);

  // client.waitStop();

  // ::tradingpb::ReplyServerInfo res;
  // auto status = client.getServerInfo(res);
  LOG(INFO) << "calcPNL " << status.error_code();

  if (status.ok()) {
    tr2::logProtobuf("reply ", res);
    // LOG(INFO) << res.DebugString();
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
  // aipMonthDay(cfg);
  // normalWeekDay(cfg);
  // normalWeekDay2(cfg);
  // normalROC1(cfg);
  // normalEMA5(cfg);
  // normalWeekDay3(cfg);
  normalWeekDay5(cfg);

  return 0;
}