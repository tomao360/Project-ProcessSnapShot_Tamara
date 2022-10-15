#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable : 4996)

#include "LogFile.h"

void Log(char message[100])
{
	time_t t;
	time(&t);
	struct tm* timeinfo;
	timeinfo = localtime(&t);

	char str[1000];
	sprintf(str, "%d.%02d.%02d: %02d:%02d:%02d - %s\n", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);

	char fileName[100];
	sprintf(fileName, "Log_%d-%02d-%02d.log", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);

	FILE* f = fopen(fileName, "a");
	if (f == NULL)
	{
		printf("Error opening the file");
		return;
	}
	fputs(str, f);
	fclose(f);
}

void LogEvent(char message[1000])
{
	char str[100];
	sprintf(str, "Event - %s", message);
	Log(str);
}

void LogError(char message[1000])
{
	char str[100];
	sprintf(str, "Error - %s", message);
	Log(str);
}

void LogWarning(char message[1000])
{
	char str[100];
	sprintf(str, "Warning - %s", message);
	Log(str);
}



