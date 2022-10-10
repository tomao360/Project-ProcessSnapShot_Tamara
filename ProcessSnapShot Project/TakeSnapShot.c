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
	struct SnapShot* singleSnapShot = GetProcessesInfo(prevSnapShot);
	
	return singleSnapShot;
}

struct SnapShot* Take20SnapShotsIn20Seconds()
{
	struct SnapShot* singleSnapShot = NULL;

	for (int i = 0; i < 20; i++)
	{
		singleSnapShot = TakeOneSnapShot(singleSnapShot);
		Sleep(1000);
	}

	return singleSnapShot;
}

struct SnapShot* TakeLongSnapShot()
{
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
	
	return singleSnapShot;
}

