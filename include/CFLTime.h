#pragma once

void startTime();
void printEndTime(char* str);


#include <string>

void OpenDebugFile(const std::wstring& szFilePath);
void CloseDebugFile();


#ifdef DEBUG
#define CGLOG_INIT(x) do { OpenDebugFile(x); } while(0)
#define CGLOG_CLOSE do { CloseDebugFile(); } while(0)

#define CGTIME_START do { startTime(); } while(0)
#define CGTIME_END(x) do { printEndTime(x); } while(0)
#else
#if 1
#define CGLOG_INIT(x) do { OpenDebugFile(x); } while(0)
#define CGLOG_CLOSE do { CloseDebugFile(); } while(0)

#define CGTIME_START do { startTime(); } while(0)
#define CGTIME_END(x) do { printEndTime(x); } while(0)
#else
#define CGLOG_INIT(x) 
#define CGLOG_CLOSE 

#define CGTIME_START 
#define CGTIME_END(x) 
#endif // 0

#endif // DEBUG
