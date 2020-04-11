
#include <tradingcore2/logs.h>

CR2BEGIN

void __signalHandle(const char* data, int size) {
  std::string str = std::string(data, size);
  /*
  std::ofstream fs("glog_dump.log",std::ios::app);
  fs<<str;
  fs.close();
  */
  LOG(ERROR) << str;
}

LogHelper::LogHelper(const char* name) {
  google::InitGoogleLogging(name);

  google::SetStderrLogging(google::INFO);
  FLAGS_colorlogtostderr = true;
  google::SetLogDestination(google::INFO, "logs/i_");
  google::SetLogDestination(google::WARNING, "logs/w_");
  google::SetLogDestination(google::ERROR, "logs/w_");
  FLAGS_logbufsecs = 0;
  FLAGS_max_log_size = 100;
  FLAGS_stop_logging_if_full_disk = true;
  // google::SetLogFilenameExtension("91_");
  google::InstallFailureSignalHandler();
  google::InstallFailureWriter(&__signalHandle);
}

LogHelper::~LogHelper() { google::ShutdownGoogleLogging(); }

CR2END