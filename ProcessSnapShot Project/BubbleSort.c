#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)

#include "MemoryTelemetry.h"
#include "LogFile.h"
#include "Structs.h"

//Making swap between two Items
void Replace(struct Process* dllCount)
{
	if (dllCount == NULL) //No Items in the list
	{
		//LogEventVal("No need to swap. There is no Items in the list.", item->num);
		return;
	}
	else if (dllCount->next == NULL) //One Item in the list
	{
		//LogEventVal("No need to swap. There is only 1 Item in the list.", item->num);
		return;
	}
	else if (dllCount == HeadP && dllCount->next == TailP) //2 Items in the list: Head and Tail
	{
		dllCount->prev = TailP;
		dllCount->next = NULL;
		TailP->prev = NULL;
		TailP->next = dllCount;
		TailP = dllCount;
		HeadP = dllCount->prev;
	}
	else if (dllCount == HeadP && dllCount->next != NULL) //2 first Items in the list
	{
		dllCount->next = dllCount->next->next;
		dllCount->next->prev->next = dllCount;
		dllCount->next->prev->prev = NULL;
		dllCount->prev = dllCount->next->prev;
		dllCount->next->prev = dllCount;
		HeadP = dllCount->prev;
	}
	else if (dllCount->next == TailP && dllCount->prev != NULL) //2 last Items in the list
	{
		dllCount->prev->next = dllCount->next;
		dllCount->next->prev = dllCount->prev;
		dllCount->next->next = dllCount;
		dllCount->prev = dllCount->next;
		dllCount->next = NULL;
		TailP = dllCount;
	}
	else //2 middle Items
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

	LogEvent("Swap Item");
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

	PrintProcessList(currentItem);
	LogEvent("End sort list");
}


