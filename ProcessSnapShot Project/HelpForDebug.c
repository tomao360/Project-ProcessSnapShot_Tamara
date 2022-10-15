#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)

#include "HelpForDebug.h"
#include "LogFile.h"
#include "Structs.h"

void PrintDLLList(struct Dll* dll)
{
	struct Dll* currentDLL = dll;

	int i = 0;
	printf("The DLL List:\n");
	while (currentDLL != NULL)
	{
		i++;
		printf("%d: %s\n", i, currentDLL->DLLName);
		currentDLL = currentDLL->next;
	}
}


void PrintProcessList(struct Process* process)
{
	struct Process* currentProcess = process;

	int i = 0;
	printf("The Process List:\n");
	while (currentProcess != NULL)
	{
		i++;
		printf("%d: %s\n", i, currentProcess->processName);
		currentProcess = currentProcess->next;
	}
}


void PrintProcessAndDllCountList(struct Process* process)
{
	struct Process* currentProcess = HeadP;

	int i = 0;
	printf("The Process List:\n");
	while (currentProcess != NULL)
	{
		i++;
		printf("%d: Process: %s\nDllCount: %d\n\n", i, currentProcess->processName, currentProcess->DLLCount);
		currentProcess = currentProcess->next;
	}
}


void PrintProcessIDList(struct Process* processID)
{
	struct Process* currentProcess = processID;

	int i = 0;
	printf("The ProcessID List:\n");
	while (currentProcess != NULL)
	{
		i++;
		printf("%d: %d\n", i, currentProcess->processID);
		currentProcess = currentProcess->next;
	}
}


void ReplaceProcessID(struct Process* item)
{
	if (item == NULL) //No Items in the list
	{
		return;
	}
	else if (item->next == NULL) //One Item in the list
	{
		return;
	}
	else if (item == HeadP && item->next == TailP) //2 Items in the list: Head and Tail
	{
		item->prev = TailP;
		item->next = NULL;
		TailP->prev = NULL;
		TailP->next = item;
		TailP = item;
		HeadP = item->prev;
	}
	else if (item == HeadP && item->next != NULL) //2 first Items in the list
	{
		item->next = item->next->next;
		item->next->prev->next = item;
		item->next->prev->prev = NULL;
		item->prev = item->next->prev;
		item->next->prev = item;
		HeadP = item->prev;
	}
	else if (item->next == TailP && item->prev != NULL) //2 last Items in the list
	{
		item->prev->next = item->next;
		item->next->prev = item->prev;
		item->next->next = item;
		item->prev = item->next;
		item->next = NULL;
		TailP = item;
	}
	else //2 middle Items
	{
		item->prev->next = item->next;
		item->next = item->next->next;
		item->next->prev->prev = item->prev;
		item->prev = item->next->prev;
		item->next->prev->next = item;
		item->next->prev = item;
	}
}

//Sorts the list
void ProcessIDBubbleSort()
{
	//Starting to sort the list
	struct Process* currentItem = HeadP;
	if (currentItem == NULL)
	{
		return;
	}

	int counter = 1;
	while (counter != 0)
	{
		counter = 0;
		currentItem = HeadP;

		while (currentItem != NULL)
		{
			if (currentItem->next != NULL && currentItem->processID > currentItem->next->processID)
			{
				ReplaceProcessID(currentItem);
				counter++;
				currentItem = currentItem->prev;
			}

			currentItem = currentItem->next;
		}
	}
}


void PrintSnapShot()
{
	struct SnapShot* currentSnapShot = HeadS;
	struct Process* currentProcess;
	struct Dll* currentDLL;

	int P = 1;
	int D = 1;

	while (currentSnapShot != NULL)
	{
		printf("SnapDhot Number: %d\n", currentSnapShot->sampleID);

		currentProcess = currentSnapShot->process;
		P = 1;
		while (currentProcess != NULL)
		{
			printf("\n#%d: Process: %s\n\n", P, currentProcess->processName);

			currentDLL = currentProcess->dll;
			D = 1;
			while (currentDLL != NULL)
			{
				printf("#%d: Dll: %s\n", D, currentDLL->DLLName);
				currentDLL = currentDLL->next;
				D++;
			}

			currentProcess = currentProcess->next;
			P++;
		}

		currentSnapShot = currentSnapShot->next;
	}
}

