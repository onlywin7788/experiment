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

int main(int argc, char** argv)
{
	int sleepTime = atoi(argv[1]);


	sleepTime *= 1000;

	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);


	while (true)
	{
		GlobalMemoryStatusEx(&memInfo);
		DWORDLONG totalPhysMem = memInfo.ullTotalPhys; // 사용량
		DWORDLONG availPhyMem = memInfo.ullAvailPhys; // 사용 가능
		DWORDLONG usedPhyMem = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

		printf("%s,%llu,%llu,%llu", getCurrentTime(), totalPhysMem, usedPhyMem, availPhyMem);
		Sleep(sleepTime);
		printf("\n");
	}

	printf("\n\n\n");

	return 0;
}