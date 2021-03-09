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
  asset0->set_market("jqdata");
  asset0->set_code("000300_XSHG|1d");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jqdata");
  asset1->set_code("000300_XSHG|1d");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("weekday");
  buy0->add_vals(5);
  // auto sell0 = strategy0->add_sell();
  // sell0->set_name("weekday");
  // sell0->add_vals(1);
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_perhandmoney(1);
  auto sp = strategy0->mutable_paramssell();
  sp->set_pervolume(1);
  auto ip = strategy0->mutable_paramsinit();
  ip->set_money(10000);
  auto aip = strategy0->mutable_paramsaip();
  aip->set_money(10000);
  aip->set_type(::tradingpb::AIPTT_WEEKDAY);
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
  }

  // auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  // tr2::startTrainSingleIndicator2ExPool(cfg, *cnfund, "110022", "rsi",
  //                                       "../output", 10000, 5, 5 /* off0 */,
  //                                       5, 5 /* off2 */, 10, 2);
}

void moneyParts(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("jqdata");
  asset0->set_code("000300_XSHG|1d");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jqdata");
  asset1->set_code("000300_XSHG|1d");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("weekday");
  buy0->add_vals(1);
  auto sell0 = strategy0->add_sell();
  sell0->set_name("weekday");
  sell0->add_vals(2);
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_moneyparts(2);
  auto sp = strategy0->mutable_paramssell();
  // sp->set_pervolume(1);
  sp->set_keeptime(7 * 24 * 60 * 60);
  auto ip = strategy0->mutable_paramsinit();
  ip->set_money(10000);
  // auto aip = strategy0->mutable_paramsaip();
  // aip->set_money(10000);
  // aip->set_type(::tradingpb::AIPTT_WEEKDAY);
  // aip->set_day(1);

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
  }

  // auto cnfund = tr2::ExchangeMgr::getSingleton()->getExchange("cnfund");
  // tr2::startTrainSingleIndicator2ExPool(cfg, *cnfund, "110022", "rsi",
  //                                       "../output", 10000, 5, 5 /* off0 */,
  //                                       5, 5 /* off2 */, 10, 2);
}

void nextBuy(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("jqdata");
  asset0->set_code("000300_XSHG|1d");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jqdata");
  asset1->set_code("000300_XSHG|1d");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("weekday");
  buy0->add_vals(1);
  auto sell0 = strategy0->add_sell();
  sell0->set_name("weekday");
  sell0->add_vals(3);
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_perhandmoney(1);
  bp->set_nexttimes(1);
  auto sp = strategy0->mutable_paramssell();
  sp->set_pervolume(1);
  // sp->set_keeptime(7 * 24 * 60 * 60);
  auto ip = strategy0->mutable_paramsinit();
  ip->set_money(10000);
  // auto aip = strategy0->mutable_paramsaip();
  // aip->set_money(10000);
  // aip->set_type(::tradingpb::AIPTT_WEEKDAY);
  // aip->set_day(1);

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

void aipMonthDayTakeProfit(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("jqdata");
  asset0->set_code("000300_XSHG|1d");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("aip");
  auto asset1 = strategy0->mutable_asset();
  asset1->set_market("jqdata");
  asset1->set_code("000300_XSHG|1d");

  auto buy0 = strategy0->add_buy();
  buy0->set_name("monthdayex");
  buy0->add_vals(1);

  auto tp0 = strategy0->add_takeprofit();
  tp0->set_name("totalreturn");
  tp0->add_vals(1.2);
  tp0->add_operators(">=");

  auto tp1 = strategy0->add_takeprofit();
  tp1->set_name("timestamp");
  tp1->add_int64vals(tr2::str2timestampUTC("20130501", "%Y%m%d") +
                     60 * 60 * 24 * 365);
  tp1->add_operators(">=");

  auto bp = strategy0->mutable_paramsbuy();
  // bp->set_aipmoney(10000);
  bp->set_perhandmoney(1);

  auto tp = strategy0->mutable_paramstakeprofit();
  // bp->set_aipmoney(10000);
  tp->set_pervolume(1);
  tp->set_isfinish(true);

  auto ap = strategy0->mutable_paramsaip();
  ap->set_type(::tradingpb::AIPTimeType::AIPTT_MONTHDAY);
  ap->set_day(1);
  ap->set_money(10000);

  params.set_startts(tr2::str2timestampUTC("20130501", "%Y%m%d"));
  params.set_endts(tr2::str2timestampUTC("20200930", "%Y%m%d"));

  ::tradingpb::ReplyCalcPNL res;
  auto status = client.clacPNL(params, res);

  // client.waitStop();

  // ::tradingpb::ReplyServerInfo res;
  // auto status = client.getServerInfo(res);
  LOG(INFO) << "calcPNL " << status.error_code();

  if (status.ok()) {
    tr2::logProtobuf("reply ", res);
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

void takeProfitWeekDay(const tr2::Config& cfg) {
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
  auto takeprofit0 = strategy0->add_takeprofit();
  takeprofit0->set_name("totalreturn");
  takeprofit0->add_vals(1.01);
  takeprofit0->add_operators(">=");
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_perhandmoney(1);
  auto sp = strategy0->mutable_paramssell();
  sp->set_pervolume(1);
  auto ip = strategy0->mutable_paramsinit();
  ip->set_money(10000);
  auto tp = strategy0->mutable_paramstakeprofit();
  tp->set_pervolume(1);
  tp->set_isfinish(true);

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

void normalWeekDay6(const tr2::Config& cfg) {
  // 周3 且周5能卖，才买
  // 周5 卖
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
  buy0->set_name("weekday2");
  buy0->add_vals(3);
  buy0->add_vals(2);
  // auto buy1 = strategy0->add_buy();
  // buy1->set_name("indicatorsp");
  // buy1->add_operators("up");
  // buy1->add_strvals("ema.5");

  // auto buy2 = strategy0->add_buy();
  // buy2->set_name("weekday");
  // buy2->add_vals(1);
  // buy2->set_group(1);
  // auto buy3 = strategy0->add_buy();
  // buy3->set_name("indicatorsp");
  // buy3->add_operators("up");
  // buy3->add_strvals("ema.5");
  // buy3->set_group(1);

  auto sell0 = strategy0->add_sell();
  sell0->set_name("weekday");
  sell0->add_vals(5);

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

// normalWeekDay7 - 复杂条件，买入是 (A&&B) || (C&&D)，
// 卖出是 符合 A&&B 买入 且符合 E，则卖出 或 符合 C&&D 买入 且符合 F
void normalWeekDay7(const tr2::Config& cfg) {
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
  buy2->add_vals(2);
  buy2->set_group(1);
  // auto buy3 = strategy0->add_buy();
  // buy3->set_name("indicatorsp");
  // buy3->add_operators("up");
  // buy3->add_strvals("ema.5");
  // buy3->set_group(1);

  auto sell0 = strategy0->add_sell();
  sell0->set_name("weekday");
  sell0->add_vals(1);

  auto sell1 = strategy0->add_sell();
  sell1->set_name("ctrlconditionid");
  sell1->add_vals(1);
  sell1->add_strvals("buy");

  auto sell2 = strategy0->add_sell();
  sell2->set_name("weekday");
  sell2->add_vals(3);
  sell2->set_group(1);

  auto sell3 = strategy0->add_sell();
  sell3->set_name("ctrlconditionid");
  sell3->add_vals(2);
  sell3->add_strvals("buy");
  sell2->set_group(1);

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

// normalWeekDay8 - 周3 且周5能卖，才买入，周5，卖出持有超过一周的份额
void normalWeekDay8(const tr2::Config& cfg) {
  // 周3 且周5能卖，才买
  // 周5 卖
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
  buy0->set_name("weekday2");
  buy0->add_vals(3);
  buy0->add_vals(2);
  // auto buy1 = strategy0->add_buy();
  // buy1->set_name("indicatorsp");
  // buy1->add_operators("up");
  // buy1->add_strvals("ema.5");

  // auto buy2 = strategy0->add_buy();
  // buy2->set_name("weekday");
  // buy2->add_vals(1);
  // buy2->set_group(1);
  // auto buy3 = strategy0->add_buy();
  // buy3->set_name("indicatorsp");
  // buy3->add_operators("up");
  // buy3->add_strvals("ema.5");
  // buy3->set_group(1);

  auto sell0 = strategy0->add_sell();
  sell0->set_name("weekday");
  sell0->add_vals(5);

  // auto sell1 = strategy0->add_sell();
  // sell1->set_name("indicatorsp");
  // sell1->add_operators("down");
  // sell1->add_strvals("ema.5");
  // sell0->add_vals(1);
  auto bp = strategy0->mutable_paramsbuy();
  bp->set_perhandmoney(1);
  auto sp = strategy0->mutable_paramssell();
  sp->set_keeptime(7 * 24 * 60 * 60);
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

void normalEMA5_3(const tr2::Config& cfg) {
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
  buy0->set_name("indicatorsp2");
  buy0->add_vals(2);
  buy0->add_operators("upcross");
  buy0->add_strvals("ema.29");
  auto sell0 = strategy0->add_sell();
  sell0->set_name("indicatorsp2");
  sell0->add_vals(2);
  sell0->add_operators("downcross");
  sell0->add_strvals("ema.29");
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

  params.set_startts(tr2::str2timestampUTC("20150101", "%Y%m%d"));
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

void normalTAMA5(const tr2::Config& cfg, const char* indicator) {
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
  buy0->add_strvals(indicator);
  auto sell0 = strategy0->add_sell();
  sell0->set_name("indicatorsp");
  // sell0->add_vals(0.02);
  sell0->add_operators("downcross");
  sell0->add_strvals(indicator);
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

void normalTAMA5_2(const tr2::Config& cfg, const char* indicator) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("jqdata");
  asset0->set_code("000300_XSHG|1d");
  auto asset1 = params.add_assets();
  asset1->set_market("jqdata");
  asset1->set_code("000300_XSHG|5m");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset10 = strategy0->mutable_asset();
  asset10->set_market("jqdata");
  asset10->set_code("000300_XSHG|1d");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("indicatorsp");
  // buy0->add_vals(-0.015);
  buy0->add_operators("upcross");
  buy0->add_strvals(indicator);
  auto sell0 = strategy0->add_sell();
  sell0->set_name("indicatorsp");
  // sell0->add_vals(0.02);
  sell0->add_operators("downcross");
  sell0->add_strvals(indicator);
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

  // params.set_startts(tr2::str2timestampUTC("20130501", "%Y%m%d"));
  // params.set_endts(tr2::str2timestampUTC("20200930", "%Y%m%d") + 1987199);
  params.set_startts(0);
  params.set_endts(-1);

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

void normalTAMA_3(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("jqdata");
  asset0->set_code("000300_XSHG|1d");
  // auto asset1 = params.add_assets();
  // asset1->set_market("jqdata");
  // asset1->set_code("000300_XSHG|5m");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset10 = strategy0->mutable_asset();
  asset10->set_market("jqdata");
  asset10->set_code("000300_XSHG|1d");

  auto buy0 = strategy0->add_buy();
  buy0->set_name("indicatorsp");
  buy0->add_operators("up");
  buy0->add_strvals("ema.29");

  auto buy1 = strategy0->add_buy();
  buy1->set_name("weekday2");
  buy1->add_vals(4);
  buy1->add_vals(6);

  auto buy2 = strategy0->add_buy();
  buy2->set_name("weekday2");
  buy2->add_vals(1);
  buy2->add_vals(4);
  buy2->set_group(1);

  auto buy3 = strategy0->add_buy();
  buy3->set_name("indicatorsp");
  buy3->add_operators("down");
  buy3->add_strvals("ema.29");
  buy3->set_group(1);

  auto sell0 = strategy0->add_sell();
  sell0->set_name("weekday");
  sell0->add_vals(3);

  auto sell1 = strategy0->add_sell();
  sell1->set_name("ctrlconditionid");
  sell1->add_vals(1);
  sell1->add_strvals("buy");

  auto sell2 = strategy0->add_sell();
  sell2->set_name("weekday");
  sell2->add_vals(5);
  sell2->set_group(1);

  auto sell3 = strategy0->add_sell();
  sell3->set_name("ctrlconditionid");
  sell3->add_vals(2);
  sell3->add_strvals("buy");
  sell3->set_group(1);

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

  params.set_startts(tr2::str2timestampUTC("20130501", "%Y%m%d"));
  params.set_endts(tr2::str2timestampUTC("20200930", "%Y%m%d"));
  // params.set_startts(0);
  // params.set_endts(-1);

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

void normalEMA15(const tr2::Config& cfg) {
  tr2::NodeClient2 client(cfg.servs[0].host.c_str(),
                          cfg.servs[0].token.c_str());

  ::tradingpb::SimTradingParams params;

  auto asset0 = params.add_assets();
  asset0->set_market("bitmex");
  asset0->set_code("XBTUSD|1d");
  auto asset1 = params.add_assets();
  asset1->set_market("bitmex");
  asset1->set_code("XBTUSD|5m");

  auto strategy0 = params.add_strategies();
  strategy0->set_name("normal");
  auto asset01 = strategy0->mutable_asset();
  asset01->set_market("bitmex");
  asset01->set_code("XBTUSD|1d");
  auto buy0 = strategy0->add_buy();
  buy0->set_name("indicatorsp");
  // buy0->add_vals(-0.015);
  buy0->add_operators("upcross");
  buy0->add_strvals("ema.15");
  auto feebuy = strategy0->mutable_feebuy();
  feebuy->set_percentage(0.0003);
  auto sell0 = strategy0->add_sell();
  sell0->set_name("indicatorsp");
  // sell0->add_vals(0.02);
  sell0->add_operators("downcross");
  sell0->add_strvals("ema.15");
  auto feesell = strategy0->mutable_feesell();
  feesell->set_percentage(0.0005);
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

  params.add_indicators("ema.15");

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
  aipWeekDay(cfg);
  // aipMonthDay(cfg);
  // normalWeekDay(cfg);
  // normalWeekDay2(cfg);
  // normalROC1(cfg);
  // normalEMA5(cfg);
  // normalEMA5_3(cfg);
  // normalTAMA5(cfg, "ta-ema.5");
  // normalWeekDay3(cfg);
  // normalWeekDay5(cfg);
  // normalWeekDay6(cfg);
  // normalWeekDay7(cfg);
  // normalWeekDay8(cfg);
  // normalEMA15(cfg);
  // normalTAMA5_2(cfg, "ta-ema.29>day/1d/5m/53700");
  // moneyParts(cfg);
  // nextBuy(cfg);
  // normalTAMA_3(cfg);
  // takeProfitWeekDay(cfg);
  // aipMonthDayTakeProfit(cfg);

  return 0;
}