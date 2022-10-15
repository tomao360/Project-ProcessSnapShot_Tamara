#include <stdio.h>
#pragma warning(disable : 4996)

#include "LinkedLists.h"
#include "LogFile.h"
#include "Structs.h"


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
	TailD = currentDLL;
}

void AddNewProcessToLinkedList(struct Process* prevProcess, struct Process* currentProcess)
{
	prevProcess->next = currentProcess;
	currentProcess->prev = prevProcess;
	currentProcess->next = NULL;
	TailP = currentProcess;
}

//Initializing global variables
int snapShotIDCounter = 1;
struct SnapShot* HeadS = NULL;
struct SnapShot* TailS = NULL;
struct SnapShot* SnapShotLinkedList(struct SnapShot* currentSnapShot)
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

	snapShotIDCounter++;
	return HeadS;
}
