/*
* DebugLogFile
* github:https://github.com/cfl997/DebugLogFile
* author : cfl997
*/


#include "CFLTime.h"


#include <mutex>
#include <iostream>



std::chrono::steady_clock::time_point start, end;

void startTime()
{
	start = std::chrono::steady_clock::now();
}

void printEndTime(char* str)
{
	// ֹͣ��ʱ
	end = std::chrono::steady_clock::now();
	// ����ʱ��ת��Ϊ����
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto durationnano = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	// ������
	std::cout << "CFLTIME-" << str << ": " << duration.count() << "ms" << std::endl;
	std::cout << "CFLTIME-" << str << ": " << durationnano.count() << "nanos" << std::endl;
	//std::cout << str << 1000000000 / durationnano.count() << "fps" << std::endl;
}



#include <iostream>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>

//#define USE_WCOUT

#ifdef USE_WCOUT
#define USE_COUT
#else
#define USE_COUT
#endif // USE_WCOUT

std::wfilebuf* g_pDebugFileBuf_w = nullptr; // std::wcout
std::filebuf* g_pDebugFileBuf = nullptr; // std::cout

std::wstreambuf* originalWcoutBuffer = nullptr; // ԭʼ���ַ����������
std::streambuf* originalCoutBuffer = nullptr; // ԭʼխ�ַ����������

void OpenDebugFile(const std::wstring& szFilePath)
{
    std::wstring strFileName;
    if (szFilePath.empty()) {
        return;
    }

    std::time_t now = std::time(nullptr);
    std::tm timeInfo;
#ifdef _WIN32
    localtime_s(&timeInfo, &now);
#else
    localtime_r(&now, &timeInfo);
#endif

    std::wstringstream ss;
    ss << szFilePath;
    ss << L"/CGTIME-CFLRENDERGL";
    ss << std::put_time(&timeInfo, L"%Y%m%d-%H%M%S");
    ss << L".log";
    strFileName = ss.str();

#ifdef USE_WCOUT
    g_pDebugFileBuf_w = new std::wfilebuf;
    if (!g_pDebugFileBuf_w->open(strFileName.c_str(), std::ios::out | std::ios::app)) {
        delete g_pDebugFileBuf_w;
        g_pDebugFileBuf_w = nullptr;
        return;
    }
    // ����ԭʼ�Ŀ��ַ�������������ض���
    originalWcoutBuffer = std::wcout.rdbuf();
    std::wcout.rdbuf(g_pDebugFileBuf_w);
#endif //USE_WCOUT

#ifdef USE_COUT
    g_pDebugFileBuf = new std::filebuf;
    if (!g_pDebugFileBuf->open(strFileName.c_str(), std::ios::out | std::ios::app)) {
        delete g_pDebugFileBuf;
        g_pDebugFileBuf = nullptr;
        return;
    }
    // ����ԭʼ��խ�ַ�������������ض���
    originalCoutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(g_pDebugFileBuf);

    std::cout << timeInfo.tm_hour << ":" << timeInfo.tm_min << ":" << timeInfo.tm_sec << std::endl;
#endif //USE_COUT
}

void CloseDebugFile()
{
#ifdef USE_WCOUT
    // ��ԭ���ַ����
    if (originalWcoutBuffer) {
        std::wcout.rdbuf(originalWcoutBuffer);
        originalWcoutBuffer = nullptr;
    }

    if (g_pDebugFileBuf_w) {
        g_pDebugFileBuf_w->close();
        delete g_pDebugFileBuf_w;
        g_pDebugFileBuf_w = nullptr;
    }
#endif //USE_WCOUT

#ifdef USE_COUT
    // ��ԭխ�ַ����
    if (originalCoutBuffer) {
        std::cout.rdbuf(originalCoutBuffer);
        originalCoutBuffer = nullptr;
    }

    if (g_pDebugFileBuf) {
        g_pDebugFileBuf->close();
        delete g_pDebugFileBuf;
        g_pDebugFileBuf = nullptr;
    }
#endif //USE_COUT
}
