#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)

#include "MemoryTelemetry.h"
#include "LogFile.h"
#include "Structs.h"
#include "HelpForDebug.h"


//Making swap between two Processes
void Replace(struct Process* dllCount)
{
	if (dllCount == NULL) //No Processes in the list
	{
		LogEvent("No need to swap. There are no Processes in the list.");
		return;
	}
	else if (dllCount->next == NULL) //One Process in the list
	{
		LogEvent("No need to swap. There is only 1 Process in the list.");
		return;
	}
	else if (dllCount == HeadP && dllCount->next == TailP) //2 Processes in the list: HeadP and TailP
	{
		dllCount->prev = TailP;
		dllCount->next = NULL;
		TailP->prev = NULL;
		TailP->next = dllCount;
		TailP = dllCount;
		HeadP = dllCount->prev;
	}
	else if (dllCount == HeadP && dllCount->next != NULL) //2 first Processes in the list
	{
		dllCount->next = dllCount->next->next;
		dllCount->next->prev->next = dllCount;
		dllCount->next->prev->prev = NULL;
		dllCount->prev = dllCount->next->prev;
		dllCount->next->prev = dllCount;
		HeadP = dllCount->prev;
	}
	else if (dllCount->next == TailP && dllCount->prev != NULL) //2 last Processes in the list
	{
		dllCount->prev->next = dllCount->next;
		dllCount->next->prev = dllCount->prev;
		dllCount->next->next = dllCount;
		dllCount->prev = dllCount->next;
		dllCount->next = NULL;
		TailP = dllCount;
	}
	else //2 middle Processes
	{
		dllCount->prev->next = dllCount->next;
		dllCount->next = dllCount->next->next;
		dllCount->next->prev->prev = dllCount->prev;
		dllCount->prev = dllCount->next->prev;
		dllCount->next->prev->next = dllCount;
		dllCount->next->prev = dllCount;
	}
}

//Sorts the list
void BubbleSort()
{
	//Starting to sort the list
	struct Process* currentItem = HeadP;
	if (currentItem == NULL)
	{
		LogError("The List is empty");
		return;
	}

	LogEvent("Swap Process");
	int counter = 1;
	while (counter != 0)
	{
		counter = 0;
		currentItem = HeadP;

		while (currentItem != NULL)
		{
			if (currentItem->next != NULL && currentItem->DLLCount > currentItem->next->DLLCount)
			{
				Replace(currentItem);
				counter++;
				currentItem = currentItem->prev;
			}

			currentItem = currentItem->next;
		}
	}

	PrintProcessAndDllCountList(currentItem);
	LogEvent("End Bubble sorting the linked list");
}


