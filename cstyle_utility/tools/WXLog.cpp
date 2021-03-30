#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <tchar.h>
#include <direct.h>

#define MAX_PATH 260

char szFilePath[260] = "WXLog.log";

static int iUseCurrentPath = 0;//默认使用

int SetLogName(char* pFilePath) {
	strcpy(szFilePath,pFilePath);
	iUseCurrentPath = 1;
	return 0;
}

int SetLogName2(char* pFilePath) {
	strcpy(szFilePath2,pFilePath);
	iUseCurrentPath = 1;
	return 0;
}

int WriteLog(const char *format, ...) {
	

	if (iUseCurrentPath == 0) {
		//TCHAR _szPath[MAX_PATH + 1] = { 0 };
		//GetModuleFileName(NULL, _szPath, MAX_PATH);
		//(_tcsrchr(_szPath, _T('\\')))[1] = 0;//删除文件名，只获得路径 字串

		//wcscat(_szPath, L"WXLog.log");

		char *buffer;
		buffer = getcwd(NULL, 0);
		strcat(buffer, "\\WXLog.txt");
		strcpy(szFilePath, buffer);
		iUseCurrentPath = 1;
	}

	FILE* pFile = fopen(szFilePath, "a");

	if (pFile == NULL) {
		return -1;
	}

	va_list arg;
	int done;

	va_start(arg, format);
	//done = vfprintf (stdout, format, arg);

	time_t time_log = time(NULL);
	struct tm* tm_log = localtime(&time_log);
	
	fprintf(pFile, "[%04d-%02d-%02d %02d:%02d:%02d]    ", tm_log->tm_year + 1900, tm_log->tm_mon + 1, tm_log->tm_mday, tm_log->tm_hour, tm_log->tm_min, tm_log->tm_sec);

	done = vfprintf(pFile, format, arg);
	va_end(arg);

	fprintf(pFile, "\n");
	fflush(pFile);

	fclose(pFile);
	return done;
}
