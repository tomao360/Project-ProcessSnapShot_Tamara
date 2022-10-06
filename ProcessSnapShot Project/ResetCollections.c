#include "ResetCollections.h"


void ResetCollections()
{
	struct SnapShot* currentSnapShot = HeadS;
	struct SnapShot* releaseSnapShot;
	struct Process* currentProcess = HeadP;
	struct Process* releaseProcess;
	struct Dll* currentDLL = HeadD;
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
}



