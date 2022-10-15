#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <Psapi.h>
#pragma warning(disable : 4996)

#include "DictionaryProcess.h"
#include "LogFile.h"
#include "Structs.h"

//Initializing global variables
struct Process_Dictionary* HeadP_Dictionary = NULL;
struct Process_Dictionary* TailP_Dictionary = NULL;


struct Process_Dictionary* MakeProcessDictionary(struct SnapShot* SnapShotHead)
{
	if (SnapShotHead == NULL)
	{
		LogError("There are no existing SnapShots in the program");

		return;
	}

	struct SnapShot* currentSnapShot = SnapShotHead;
	struct Process* currentProcess;
	
	while (currentSnapShot != NULL)
	{
		currentProcess = currentSnapShot->process;

		while (currentProcess != NULL)
		{
			AddToProcessDictionary(currentProcess);
			currentProcess = currentProcess->next;
		}

		currentSnapShot = currentSnapShot->next;
	}

	return HeadP_Dictionary;
}


//Insert value into the dictionary: key
void AddToProcessDictionary(struct Process* processDictionary_key)
{
	struct Process_Dictionary* currentProcess = (struct Process_Dictionary*)malloc(sizeof(struct Process_Dictionary));
	if (currentProcess == NULL)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return;
	}

	currentProcess->processDictionarykey = (struct Process*)malloc(sizeof(struct Process));
	if (currentProcess->processDictionarykey == NULL)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return;
	}
	
	//Process information inside the dictionary
	strcpy(currentProcess->processDictionarykey->processName, processDictionary_key->processName);
	currentProcess->processDictionarykey->memoryInfo = processDictionary_key->memoryInfo;
	currentProcess->processDictionarykey->processID = processDictionary_key->processID;
	currentProcess->processDictionarykey->next = NULL;
	currentProcess->processDictionarykey->prev = NULL;

	//A variable that will help to go through the list from the beginning to the end
	struct Process_Dictionary* headOfDictionary = HeadP_Dictionary;

	if (HeadP_Dictionary == NULL)
	{
		HeadP_Dictionary = currentProcess;
		TailP_Dictionary = currentProcess;
		currentProcess->next = NULL;
		currentProcess->prev = NULL;
	}
	else
	{
		while (headOfDictionary != NULL)
		{
			if (headOfDictionary->processDictionarykey->processID == currentProcess->processDictionarykey->processID && strcmp(headOfDictionary->processDictionarykey->processName, currentProcess->processDictionarykey->processName) == 0)
			{
				free(currentProcess);
				break;
			}
			if (headOfDictionary->next == NULL)
			{
				TailP_Dictionary->next = currentProcess;
				currentProcess->prev = TailP_Dictionary;
				currentProcess->next = NULL;
				TailP_Dictionary = currentProcess;
				break;
			}

			headOfDictionary = headOfDictionary->next;
		}
	}

	LogEvent("Making a Processes Dictionary had finished");
}



int NumOfProcessesInAllSnapShots(struct Process_Dictionary* snapShotHead)
{
	struct Process_Dictionary* currentProcess = snapShotHead;
	int numOfProcesses = 0;

	while (currentProcess != NULL)
	{
		numOfProcesses++;
		currentProcess = currentProcess->next;
	}

	return numOfProcesses;
}