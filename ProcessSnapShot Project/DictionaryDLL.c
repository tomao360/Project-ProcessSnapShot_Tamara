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

int countOfProcess = 0;  //A variable that will help calculate the amount of processes for a key in the dictionary 

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

	LogEvent("Making a DLL Dictionary has finished");
	return HeadD_Dictionary;
}


//Insert values into the dictionary: key->DLl Name and val->Process
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

		countOfProcess++;
		currenDll->processCount = countOfProcess;
		countOfProcess = 0;
	}
	else
	{
		while (headOfDictionary != NULL)
		{
			if (strcmp(headOfDictionary->keyDLL, currenDll->keyDLL) == 0)
			{
				AddNewProcessToDll(headOfDictionary->processDictionary, currenDll->processDictionary, headOfDictionary);

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

				countOfProcess++;
				currenDll->processCount = countOfProcess;
				countOfProcess = 0;

				break;
			}

			headOfDictionary = headOfDictionary->next;
		}
	}
}


void AddNewProcessToDll(struct Process* currentProcess, struct Process* newProcessDict, struct DLL_Dictionary* currenDllDict)
{
	struct Process* currentDictionaryP = currentProcess;
	struct DLL_Dictionary* currenDll = currenDllDict;

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

			currenDll->processCount = currenDll->processCount + 1;

			break;
		}

		currentDictionaryP = currentDictionaryP->next;
	}
}


//Calculation of the amount of Dlls in SnapShots after a dictionary
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
