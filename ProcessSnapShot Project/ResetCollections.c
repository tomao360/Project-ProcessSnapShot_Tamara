#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

#include "ResetCollections.h"
#include "LogFile.h"
#include "Structs.h"
#include "LinkedLists.h"


void ResetCollections()
{
	struct SnapShot* currentSnapShot = HeadS;
	struct SnapShot* releaseSnapShot;
	struct Process* currentProcess;
	struct Process* releaseProcess;
	struct Dll* currentDLL;
	struct Dll* releaseDLL;
	
	while (currentSnapShot != NULL)
	{
		currentProcess = currentSnapShot->process;

		while (currentProcess != NULL)
		{
			currentDLL = currentProcess->dll;

			while (currentDLL != NULL)
			{
				releaseDLL = currentDLL;
				currentDLL = currentDLL->next;
				free(releaseDLL);
			}

			releaseProcess = currentProcess;
			currentProcess = currentProcess->next;
			free(releaseProcess);
		}

		releaseSnapShot = currentSnapShot;
		currentSnapShot = currentSnapShot->next;
		free(releaseSnapShot);
	}

	HeadS = NULL;
	TailS = NULL;
	HeadP = NULL;
	TailP = NULL;
	HeadD = NULL;
	TailD = NULL;
	snapShotIDCounter = 1;
}


void ResetDllDictionary()
{
	struct DLL_Dictionary* currentDll = HeadD_Dictionary;
	struct DLL_Dictionary* releaseDll;

	while (currentDll != NULL)
	{
		releaseDll = currentDll;
		currentDll = currentDll->next;
		free(releaseDll);
	}

	HeadD_Dictionary = NULL;
	TailD_Dictionary = NULL;
}


void ResetProcessDictionary()
{
	struct Process_Dictionary* currentProcess = HeadP_Dictionary;
	struct Process_Dictionary* releaseProcess;

	while (currentProcess != NULL)
	{
		releaseProcess = currentProcess;
		currentProcess = currentProcess->next;
		free(releaseProcess);
	}

	HeadP_Dictionary = NULL;
	TailP_Dictionary = NULL;
}
