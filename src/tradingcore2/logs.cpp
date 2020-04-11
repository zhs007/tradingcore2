
#include <tradingcore2/logs.h>

CR2BEGIN

void initLogs(const char* name) { google::InitGoogleLogging(name); }

void releaseLogs() { google::ShutdownGoogleLogging(); }

CR2END