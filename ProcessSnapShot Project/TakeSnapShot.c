#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <Psapi.h>
#pragma warning(disable : 4996)

#include "TakeSnapShot.h"
#include "LogFile.h"
#include "MemoryTelemetry.h"
#include "Structs.h"
#include "LinkedLists.h"


struct SnapShot* TakeOneSnapShot(struct SnapShot* prevSnapShot)
{
	LogEvent("Start taking One SnapShot");

	struct SnapShot* singleSnapShot = GetProcessesInfo(prevSnapShot);

	LogEvent("Taking One SnapShot has finished");

	return singleSnapShot;
}

struct SnapShot* Take20SnapShotsIn20Seconds()
{
	LogEvent("Start taking Twenty SnapShots");

	struct SnapShot* singleSnapShot = NULL;

	for (int i = 0; i < 20; i++)
	{
		singleSnapShot = TakeOneSnapShot(singleSnapShot);
		Sleep(1000);
	}

	LogEvent("Taking Twenty SnapShots has finished");

	return singleSnapShot;
}

struct SnapShot* TakeLongSnapShot()
{
	LogEvent("Start taking Long SnapShot");
	struct SnapShot* singleSnapShot = NULL;

	char stop = 0;
	while (stop != '4')
	{
		singleSnapShot = TakeOneSnapShot(singleSnapShot);
		Sleep(1000);

		if (kbhit())
		{
			stop = getch();
		}
	}

	LogEvent("End taking Long SnapShot");
	
	return singleSnapShot;
}


//Calculates the average of the WorkingSetSize memory in one SnapShot
SIZE_T MemoryAverageInSnapShot(struct SnapShot* snapShotHead)
{
	struct Process* currentProcess = snapShotHead->process;

	SIZE_T sum = 0;
	SIZE_T avg;

	while (currentProcess != NULL)
	{
		sum += currentProcess->memoryInfo.WorkingSetSize;
		currentProcess = currentProcess->next;
	}

	avg = sum / snapShotHead->processCount;

	return avg;
}


//Calculates the average of the WorkingSetSize memory in all the SnapShots
SIZE_T MemoryAverageInAllSnapShots(struct SnapShot* snapShotHead)
{
	struct SnapShot* currentSnapShot = snapShotHead;
	struct Process* currentProcess = currentSnapShot->process;

	SIZE_T sumMemory = 0;
	int sumProcesses = 0;
	SIZE_T avg;

	while (currentSnapShot != NULL)
	{
		currentProcess = currentSnapShot->process;

		while (currentProcess != NULL)
		{
			sumMemory += currentProcess->memoryInfo.WorkingSetSize;
			currentProcess = currentProcess->next;
		}

		sumProcesses = sumProcesses + currentSnapShot->processCount;

		currentSnapShot = currentSnapShot->next;
	}

	avg = sumMemory / sumProcesses;

	return avg;
}


//Finds the process with the highest WorkingSetSize in one SnapShot
SIZE_T ProcessWithHighestWorkingSetSize(struct SnapShot* snapShotHead)
{
	struct SnapShot* currentSnapShot = snapShotHead;
	struct Process* currentProcess = currentSnapShot->process;
	struct Process* checkProcess = currentSnapShot->process->memoryInfo.WorkingSetSize;
	SIZE_T processCheck = checkProcess;
	int numCurrentProcess = 0;
	int numCheckProcess = 0;


	currentProcess = currentSnapShot->process;
	while (currentProcess != NULL)
	{
		numCurrentProcess++;

		if (currentProcess->memoryInfo.WorkingSetSize > processCheck)
		{
			processCheck = currentProcess->memoryInfo.WorkingSetSize;
			numCheckProcess = numCurrentProcess;
		}

		currentProcess = currentProcess->next;

	}

	return numCheckProcess;
}






