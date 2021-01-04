#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <conio.h>
#include <iostream>
#include  <time.h>
#include <stdlib.h>

using namespace std;

/** 로딩 과정중에서 발생한 메모리 소요량을 측정한다. */
static PROCESS_MEMORY_COUNTERS_EX g_mc;

char timeBuffer[2048];

char* getCurrentTime()
{
	memset(timeBuffer, 0x00, 2048);

	time_t curTime = time(NULL);
	struct tm* pLocal = localtime(&curTime);

	sprintf(timeBuffer, "%04d-%02d-%02d %02d:%02d:%02d",
		pLocal->tm_year + 1900, pLocal->tm_mon + 1, pLocal->tm_mday,
		pLocal->tm_hour, pLocal->tm_min, pLocal->tm_sec);

	return timeBuffer;
}

void writeFileContents(char* logPath, char* contents)
{
	FILE* f;

	f = fopen(logPath, "at");
	if (f != NULL) {
		fputs(contents, f);
		fclose(f);
	}
}

int main(int argc, char** argv)
{
	if (argc != 3) {
		printf("[PROGRAM] [INTERVAL SEC] [LOG_PATH]\n\n");
		return -1;
	}

	int sleepTime = atoi(argv[1]);
	sleepTime *= 1000;
	char contents[2048] = { 0x00, };
	char logPath[2048] = { 0x00, };

	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);

	strcpy(logPath, argv[2]);

	while (true)
	{
		memset(contents, 0x00, 2048);

		GlobalMemoryStatusEx(&memInfo);
		DWORDLONG totalPhysMem = memInfo.ullTotalPhys / 1048576; // 사용량 (MB)
		DWORDLONG availPhyMem = memInfo.ullAvailPhys / 1048576; // 사용 가능 (MB)
		DWORDLONG usedPhyMem = totalPhysMem - availPhyMem; // 사용 중 (MB)

		sprintf(contents, "%s,%llu,%llu,%llu\n", getCurrentTime(), totalPhysMem, usedPhyMem, availPhyMem);

		writeFileContents(logPath, contents);

		Sleep(sleepTime);
	}
	return 0;
}