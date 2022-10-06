#include "LinkedLists.h"

void DllLinkedList(struct Dll* currentDLL)
{
	if (HeadD == NULL)
	{
		HeadD = currentDLL;
		TailD = currentDLL;
		currentDLL->next = NULL;
		currentDLL->prev = NULL;
	}
	else
	{
		TailD->next = currentDLL;
		currentDLL->prev = TailD;
		currentDLL->next = NULL;
		TailD = currentDLL;
	}
}

void ProcessLinkedList(struct Process* currentProcess)
{
	if (HeadP == NULL)
	{
		HeadP = currentProcess;
		TailP = currentProcess;
		currentProcess->next = NULL;
		currentProcess->prev = NULL;
	}
	else
	{
		TailP->next = currentProcess;
		currentProcess->prev = TailP;
		currentProcess->next = NULL;
		TailP = currentProcess;
	}
}

void AddNewDllToLinkedList(struct Dll* prevDLL, struct Dll* currentDLL)
{
	prevDLL->next = currentDLL;
	currentDLL->prev = prevDLL;
	currentDLL->next = NULL;
}

void AddNewProcessToLinkedList(struct Process* prevProcess, struct Process* currentProcess)
{
	prevProcess->next = currentProcess;
	currentProcess->prev = prevProcess;
	currentProcess->next = NULL;
}


struct SnapShot* HeadS = NULL;
struct SnapShot* TailS = NULL;
void SnapShotLinkedList(struct SnapShot* currentSnapShot)
{
	if (HeadS == NULL)
	{
		HeadS = currentSnapShot;
		TailS = currentSnapShot;
		currentSnapShot->next = NULL;
		currentSnapShot->prev = NULL;
	}
	else
	{
		TailS->next = currentSnapShot;
		currentSnapShot->prev = TailS;
		currentSnapShot->next = NULL;
		TailS = currentSnapShot;
	}
}
