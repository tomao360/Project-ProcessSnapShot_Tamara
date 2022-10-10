#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

#include "ResetCollections.h"
#include "LogFile.h"
#include "Structs.h"


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
}



