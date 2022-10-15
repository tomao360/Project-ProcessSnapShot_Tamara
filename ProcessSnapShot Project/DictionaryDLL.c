#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <Psapi.h>
#pragma warning(disable : 4996)

#include "DictionaryDLL.h"
#include "LogFile.h"
#include "Structs.h"

//Initializing global variables
struct DLL_Dictionary* HeadD_Dictionary = NULL;
struct DLL_Dictionary* TailD_Dictionary = NULL;


struct DLL_Dictionary* MakeDllDictionary(struct SnapShot* SnapShotHead)
{
	if (SnapShotHead == NULL)
	{
		LogError("There are no existing SnapShots in the program");

		return;
	}

	struct SnapShot* currentSnapShot = SnapShotHead;
	struct Process* currentProcess;
	struct Dll* currentDLL;

	while (currentSnapShot != NULL)
	{
		currentProcess = currentSnapShot->process;

		while (currentProcess != NULL)
		{
			currentDLL = currentProcess->dll;

			while (currentDLL != NULL)
			{
				AddToDllDictionary(currentDLL->DLLName, currentProcess);
				currentDLL = currentDLL->next;
			}

			currentProcess = currentProcess->next;
		}

		currentSnapShot = currentSnapShot->next;
	}

	return HeadD_Dictionary;
}


//Insert values into the dictionary: key and val
void AddToDllDictionary(char dllDictionary_key[MAX_PATH], struct Process* processDictionary_val)
{
	struct DLL_Dictionary* currenDll = (struct DLL_Dictionary*)malloc(sizeof(struct DLL_Dictionary));
	if (currenDll == NULL)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return;
	}

	currenDll->processDictionary = (struct Process*)malloc(sizeof(struct Process));
	if (currenDll->processDictionary == NULL)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return;
	}

	//DLL information
	strcpy(currenDll->keyDLL, dllDictionary_key);
	currenDll->next = NULL;
	currenDll->prev = NULL;

	//Process information inside the dictionary
	strcpy(currenDll->processDictionary->processName, processDictionary_val->processName);
	currenDll->processDictionary->memoryInfo = processDictionary_val->memoryInfo;
	currenDll->processDictionary->processID = processDictionary_val->processID;
	currenDll->processDictionary->next = NULL;
	currenDll->processDictionary->prev = NULL;

	//A variable that will help to go through the list from the beginning to the end
	struct DLL_Dictionary* headOfDictionary = HeadD_Dictionary;

	if (HeadD_Dictionary == NULL)
	{
		HeadD_Dictionary = currenDll;
		TailD_Dictionary = currenDll;
		currenDll->next = NULL;
		currenDll->prev = NULL;
	}
	else
	{
		while (headOfDictionary != NULL)
		{
			if (strcmp(headOfDictionary->keyDLL, currenDll->keyDLL) == 0)
			{
				AddNewProcessToDll(headOfDictionary->processDictionary, currenDll->processDictionary);

				free(currenDll->processDictionary);
				free(currenDll);
				break;
			}
			if (headOfDictionary->next == NULL)
			{
				TailD_Dictionary->next = currenDll;
				currenDll->prev = TailD_Dictionary;
				currenDll->next = NULL;
				TailD_Dictionary = currenDll;
				break;
			}

			headOfDictionary = headOfDictionary->next;
		}
	}

	LogEvent("Making a DLL Dictionary had finished");
}


void AddNewProcessToDll(struct Process* currentProcess, struct Process* newProcessDict)
{
	struct Process* currentDictionaryP = currentProcess;

	while (currentDictionaryP != NULL)
	{
		if (currentDictionaryP->processID == newProcessDict->processID && strcmp(currentDictionaryP->processName, newProcessDict->processName) == 0)
		{
			break;
		}
		if (currentDictionaryP->next == NULL)
		{
			struct Process* newProcess = (struct Process*)malloc(sizeof(struct Process));
			if (newProcess == NULL)
			{
				char* str = strerror(GetLastError());
				LogError(str);

				return;
			}

			strcpy(newProcess->processName, newProcessDict->processName);
			newProcess->memoryInfo = newProcessDict->memoryInfo;
			newProcess->processID = newProcessDict->processID;

			currentDictionaryP->next = newProcess;
			newProcess->prev = currentDictionaryP;
			newProcess->next = NULL;

			break;
		}

		currentDictionaryP = currentDictionaryP->next;
	}
}


int NumOfDllInAllSnapShots(struct DLL_Dictionary* snapShotHead)
{
	struct DLL_Dictionary* currentDll = snapShotHead->keyDLL;
	int numOfDll = 0;

	while (currentDll != NULL)
	{
		numOfDll++;
		currentDll = currentDll->next;
	}

	return numOfDll;
}