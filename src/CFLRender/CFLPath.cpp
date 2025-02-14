#include "CFLPath.h"

#include <stdio.h>
#include <string>
#include <tchar.h>
#include <codecvt>
#include <map>
#include <unordered_map>
#include <assert.h>
#include <iostream>

#include <Windows.h>

#define MAX_PATH_COUNT (CG_PATH_MAX_COUNT)
LPTSTR szPathArray[MAX_PATH_COUNT];
static bool isPathInit = false;

void CGPath_SetPath(CGPathType type, const wchar_t* szPath)
{
#ifdef _DEBUG
	if (type >= MAX_PATH_COUNT || type < 0)
	{
		return;
	}
#endif

	if (szPathArray[type])
	{
		delete[] szPathArray[type];
		szPathArray[type] = NULL;
	}
	int nlen = (int)_tcslen(szPath);
	szPathArray[type] = new TCHAR[nlen + 1];
	_tcscpy_s(szPathArray[type], nlen + 1, szPath);
}

bool CFL_GLRENDER_API CGPath_Init()
{
	if (isPathInit)
	{
		return isPathInit;
	}
	memset(szPathArray, 0, sizeof(szPathArray));

	TCHAR szBinPath[512];
	//获取当前应用程序的完整路径
	GetModuleFileName(NULL, szBinPath, 512);

	int nLen = (int)_tcslen(szBinPath);
	for (int i = nLen - 1; i >= 0; i--)
	{
		if (szBinPath[i] == '\\')
		{
			szBinPath[i] = 0;
			break;
		}
	}
	// 分目录
	std::wstring szPath(szBinPath);
	CGPath_SetPath(CG_PATH_BIN, szPath.c_str());

	std::wstring szSubPath = szPath + _T("\\plugin");
	CGPath_SetPath(CG_PATH_PLUGIN, szSubPath.c_str());

	szSubPath = szPath + _T("\\Fonts");
	CGPath_SetPath(CG_PATH_FONTS, szSubPath.c_str());

	szSubPath = szPath + _T("\\Config");
	CGPath_SetPath(CG_PATH_CONFIG, szSubPath.c_str());
	


	isPathInit = true;
	bool inittrue = CGText_Init();
	if (!inittrue)
	{
		std::cout << "CGText_Init successful!" << std::endl;
	}
	assert(inittrue);

	return isPathInit;
}

wchar_t* CGPath_GetPath(CGPathType dwType)
{
#ifdef _DEBUG
	if (dwType >= MAX_PATH_COUNT || dwType < 0)
	{
		return NULL;
	}
#endif
	return szPathArray[dwType];
}

void CGPath_Release()
{
	if (!isPathInit)
		return;
	for (int i = 0; i < MAX_PATH_COUNT; i++)
	{
		if (szPathArray[i])
		{
			delete[] szPathArray[i];
			szPathArray[i] = NULL;
		}
	}
	memset(szPathArray, 0, sizeof(LPTSTR) * MAX_PATH_COUNT);
}

CFL_GLRENDER_API std::string wstr2utf8(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> m_cv;
	return m_cv.to_bytes(wstr);
}

CFL_GLRENDER_API std::wstring utf82wstr(const std::string& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> m_cv;
	return m_cv.from_bytes(wstr);
}


/*
* TextCode
*/

std::wstring AnsiToWString(const std::string& str)
{
	int len;
	int slength = static_cast<int>(str.length()) + 1;
	len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
	std::wstring wstr(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, &wstr[0], len);
	return wstr;
}


#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>

static const std::wstring TextCodeDataName = L"/Code2Text.csv";
std::unordered_map<long, std::wstring>g_TextCode;
static bool isTextInit = false;
std::mutex mutex;

CFL_GLRENDER_API bool CGText_Init()
{
	if (!isPathInit)
		return false;
	std::lock_guard lock(mutex);

	std::wstring binpathstr{ CGPath_GetPath(CGPathType::CG_PATH_BIN) };
	binpathstr += TextCodeDataName;

	g_TextCode.clear();

	std::wifstream file(binpathstr);
	if (!file.is_open()) {
		std::wcerr << L"无法打开文件: " << binpathstr << std::endl;
		return false;
	}

	// 设置文件流使用UTF-8编码
	file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));

	std::wstring line;
	while (std::getline(file, line)) {
		std::wstringstream linestream(line);
		std::wstring keyStr, value;
		if (std::getline(linestream, keyStr, L',') && std::getline(linestream, value)) {
			try {
				long key = std::stol(keyStr);
				g_TextCode[key] = value;
			}
			catch (const std::invalid_argument&) {
				std::wcerr << L"CSV文件中无效的键: " << keyStr << std::endl;
				continue;
			}
			catch (const std::out_of_range&) {
				std::wcerr << L"CSV文件中超出范围的键: " << keyStr << std::endl;
				continue;
			}
		}
		else {
			std::wcerr << L"CSV文件中无效的行: " << line << std::endl;
			assert(false);
		}
	}
	file.close();

	isTextInit = true;
	return isTextInit;
}

CFL_GLRENDER_API const std::wstring CGText_TextByCode(long code)
{
	if (!isTextInit)
		return std::wstring{};
	std::lock_guard lock(mutex);
	return g_TextCode[code];
}
#include "CFLTime.h"
CFL_GLRENDER_API void CGLog_INIT(const std::wstring& path)
{
	OpenDebugFile(path);
}

CFL_GLRENDER_API void CGLog_CLOSE()
{
	CloseDebugFile();
}

CFL_GLRENDER_API void CGTime_START()
{
	startTime();
}

CFL_GLRENDER_API void CGTime_END(char* str)
{
	printEndTime(str);
}


