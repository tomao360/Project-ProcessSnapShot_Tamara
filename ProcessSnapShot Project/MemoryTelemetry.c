#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <Psapi.h>
#pragma warning(disable : 4996)

#include "MemoryTelemetry.h"
#include "LogFile.h"
#include "Structs.h"
#include "LinkedLists.h"

struct Process* HeadP = NULL;
struct Process* TailP = NULL;

struct Dll* HeadD = NULL;
struct Dll* TailD = NULL;


struct Process* PrintMemoryInfo(DWORD processID)
{
	HeadD = NULL;
	TailD = NULL;

	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	// Open process in order to receive information
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return NULL;
	}

	struct Process* myProcess; 

	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR FoundProcessName[MAX_PATH];
	TCHAR wDllName[MAX_PATH];

	// Get Process Name
	if (GetModuleFileNameEx(hProcess, 0, FoundProcessName, MAX_PATH))
	{
		// At this point, buffer contains the full path to the executable
		char processName[MAX_PATH];
		size_t numConverted;
		wcstombs_s(&numConverted, processName, MAX_PATH, FoundProcessName, MAX_PATH);
		if (strlen(processName) > 1)
		{
			myProcess = (struct Process*)malloc(sizeof(struct Process));
			if (myProcess == NULL)
			{
				char* str = strerror(GetLastError());
				LogError(str);

				return NULL;
			}

			strcpy(myProcess->processName, processName);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return;
	}

	myProcess->processID = processID;

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		myProcess->memoryInfo.PageFaultCount = pmc.PageFaultCount;
		myProcess->memoryInfo.WorkingSetSize = pmc.WorkingSetSize;
		myProcess->memoryInfo.QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
		myProcess->memoryInfo.QuotaPeakPagedPoolUsage = pmc.QuotaPeakPagedPoolUsage;
		myProcess->memoryInfo.PagefileUsage = pmc.PagefileUsage;
	}


	// Get Dlls List
	int counterDLL = 0;
	int i = 0;
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			// Get the full path to the module's file.
			if (GetModuleFileNameEx(hProcess, hMods[i], wDllName, MAX_PATH))
			{
				// * Get the module name and handle value.
				//printf("%s", wDllName);

				// Convert wChar to regular char array (string)
				char getDllName[MAX_PATH];
				size_t numConverted;
				wcstombs_s(&numConverted, getDllName, MAX_PATH, wDllName, MAX_PATH);
				if (strlen(getDllName) > 1)
				{
					struct Dll* dllName = (struct Dll*)malloc(sizeof(struct Dll));
					if (dllName == NULL)
					{
						char* str = strerror(GetLastError());
						LogError(str);

						return;
					}
					strcpy(dllName->DLLName, getDllName);
					DllLinkedList(dllName);
					counterDLL++;
				}
			}
		}

		//PrintDLLList();
		myProcess->DLLCount = counterDLL;
		myProcess->dll = HeadD;
	}

	if (i == 0)
	{
		myProcess->DLLCount = 0;
		myProcess->dll = NULL;
	}

	CloseHandle(hProcess);
	return myProcess;
}


struct SnapShot* GetProcessesInfo(struct SnapShot* prevSnapShot)
{
	// Get Processes
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	// * Receive all process ID and put in aProcesses Array
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		// Error. Write to log
		return 1;
	}

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	if (prevSnapShot == NULL)
	{
		HeadP = NULL;
		TailP = NULL;
		int processCount = 0;
		int dllCountInSnap = 0;

		struct SnapShot* mySnapShot = (struct SnapShot*)malloc(sizeof(struct SnapShot));
		if (mySnapShot == NULL)
		{
			char* str = strerror(GetLastError());
			LogError(str);

			return;
		}

		// *Loop of all processes
		for (i = 0; i < cProcesses; i++)
		{
			mySnapShot->process = PrintMemoryInfo(aProcesses[i]);

			if (mySnapShot->process != NULL)
			{
				ProcessLinkedList(mySnapShot->process);
				processCount++;
				dllCountInSnap += mySnapShot->process->DLLCount;
			}
		}

		mySnapShot->DllCountInSnap = dllCountInSnap;
		mySnapShot->processCount = processCount;
		mySnapShot->process = HeadP;
		return mySnapShot;
	}

	// For each Process to get its Memory Information

	else if (prevSnapShot != NULL)
	{
		for (i = 0; i < cProcesses; i++)
		{
			AccumulateSnapShots(prevSnapShot, aProcesses[i]);
		}
		//BubbleSort();
		PrintProcessIDList(HeadP);
		prevSnapShot->process = HeadP;
		return prevSnapShot;
	}
}

/*void AccumulateSnapShots(struct SnapShot* prevSnapShot, DWORD processID)
{
	struct Process* prevProcess = prevSnapShot->process;

	HeadD = NULL;
	TailD = NULL;

	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	// Open process in order to receive information
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return NULL;
	}

	struct Process* newProcess = (struct Process*)malloc(sizeof(struct Process));


	newProcess->processID = processID;

	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR FoundProcessName[MAX_PATH];
	TCHAR wDllName[MAX_PATH];
	size_t numConverted;

	// Get Process Name
	if (GetModuleFileNameEx(hProcess, 0, FoundProcessName, MAX_PATH))
	{
		// At this point, buffer contains the full path to the executable
		char processName[MAX_PATH];
		wcstombs_s(&numConverted, processName, MAX_PATH, FoundProcessName, MAX_PATH);
		if (strlen(processName) > 1)
		{
			strcpy(newProcess->processName, processName);
		}
	}
	else
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return;
	}
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		newProcess->memoryInfo.PageFaultCount = pmc.PageFaultCount;
		newProcess->memoryInfo.WorkingSetSize = pmc.WorkingSetSize;
		newProcess->memoryInfo.QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
		newProcess->memoryInfo.QuotaPeakPagedPoolUsage = pmc.QuotaPeakPagedPoolUsage;
		newProcess->memoryInfo.PagefileUsage = pmc.PagefileUsage;
	}

	int i = 0;
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			if (GetModuleFileNameEx(hProcess, hMods[i], wDllName, MAX_PATH))
			{
				struct Dll* dllName = (struct Dll*)malloc(sizeof(struct Dll));
				if (dllName == NULL)
				{
					char* str = strerror(GetLastError());
					LogError(str);
					return;
				}

				char getDllName[MAX_PATH];
				wcstombs_s(&numConverted, getDllName, MAX_PATH, wDllName, MAX_PATH);
				if (strlen(getDllName) > 1)
				{
					strcpy(dllName->DLLName, getDllName);
					DllLinkedList(dllName);
				}
			}
		}
	}

	newProcess->DLLCount = i;
	newProcess->dll = HeadD;


	struct Dll* tempPrevDll = prevProcess->dll;
	struct Dll* tempNewDll = newProcess->dll;

	while (prevProcess != NULL)
	{
		if (prevProcess->processID == newProcess->processID)
		{
			prevProcess->memoryInfo.PageFaultCount += newProcess->memoryInfo.PageFaultCount;
			prevProcess->memoryInfo.WorkingSetSize += newProcess->memoryInfo.WorkingSetSize;
			prevProcess->memoryInfo.QuotaPagedPoolUsage += newProcess->memoryInfo.QuotaPagedPoolUsage;
			prevProcess->memoryInfo.QuotaPeakPagedPoolUsage += newProcess->memoryInfo.QuotaPeakPagedPoolUsage;
			prevProcess->memoryInfo.PagefileUsage += newProcess->memoryInfo.PagefileUsage;

			while (tempNewDll != NULL) //NEW DLL LIST
			{
				tempPrevDll = prevProcess->dll;
				while (tempPrevDll != NULL) //PREV DLL LIST
				{
					if (strcmp(tempPrevDll->DLLName, tempNewDll->DLLName) == 0)
					{
						break;
					}

					if (tempPrevDll->next == NULL)
					{
						if (strcmp(tempPrevDll->DLLName, tempNewDll->DLLName) != 0)
						{
							struct Dll* newDll = (struct Dll*)malloc(sizeof(struct Dll));
							strcpy(newDll->DLLName, tempNewDll->DLLName);
							//AddNewDllToLinkedList(prevProcess->dll, newDll);
							TailD->next = newDll;
							newDll->prev = TailD;
							newDll->next = NULL;
							TailD = newDll;
							prevSnapShot->process->DLLCount++;
							break; 
						}
					}

					tempPrevDll = tempPrevDll->next;
				}

				tempNewDll = tempNewDll->next;
			}

			while (newProcess->dll != NULL)
			{
				tempNewDll = newProcess->dll; // Temp שומר על ראש הרשימה 
				newProcess->dll = newProcess->dll->next;
				free(tempNewDll);
			}

			free(newProcess);
			break;
		}
		
		prevProcess = prevProcess->next;
	}

	if (prevProcess == NULL)
	{
		//newProcess->dll = tempNewDll;
		//AddNewProcessToLinkedList(TailP, newProcess); //שןלחת את הזנב כי אני מגיה לNULL
		TailP->next = newProcess;
		newProcess->prev = TailP;
		newProcess->next = NULL;
		TailP = newProcess;
		prevSnapShot->processCount++;
	}


	CloseHandle(hProcess);
}*/

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

//void PrintProcessList(struct Process* process)
//{
//	struct Process* currentProcess = process;
//
//	int i = 0;
//	printf("The Process List:\n");
//	while (currentProcess != NULL)
//	{
//		i++;
//		printf("%d: %s\n",i, currentProcess->processName);
//		currentProcess = currentProcess->next;
//	}
//}

void PrintProcessList(struct Process* process)
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


void AccumulateSnapShots(struct SnapShot* prevSnapShot, DWORD processID)
{
	struct Process* prevProcess = prevSnapShot->process;

	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	HeadD = NULL;
	TailD = NULL;

	// Open process in order to receive information
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return NULL;
	}

	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR FoundProcessName[MAX_PATH];
	TCHAR wDllName[MAX_PATH];
	size_t numConverted;

	struct Process* newProcess;

	// Get Process Name
	if (GetModuleFileNameEx(hProcess, 0, FoundProcessName, MAX_PATH))
	{
		// At this point, buffer contains the full path to the executable
		char processName[MAX_PATH];
		wcstombs_s(&numConverted, processName, MAX_PATH, FoundProcessName, MAX_PATH);
		if (strlen(processName) > 1)
		{
			newProcess = (struct Process*)malloc(sizeof(struct Process));
			if (newProcess == NULL)
			{
				char* str = strerror(GetLastError());
				LogError(str);

				return NULL;
			}
			strcpy(newProcess->processName, processName);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return;
	}

	newProcess->processID = processID;

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		newProcess->memoryInfo.PageFaultCount = pmc.PageFaultCount;
		newProcess->memoryInfo.WorkingSetSize = pmc.WorkingSetSize;
		newProcess->memoryInfo.QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
		newProcess->memoryInfo.QuotaPeakPagedPoolUsage = pmc.QuotaPeakPagedPoolUsage;
		newProcess->memoryInfo.PagefileUsage = pmc.PagefileUsage;
	}

	int counterDLL = 0;
	int i = 0;
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			if (GetModuleFileNameEx(hProcess, hMods[i], wDllName, MAX_PATH))
			{
				char getDllName[MAX_PATH];
				wcstombs_s(&numConverted, getDllName, MAX_PATH, wDllName, MAX_PATH);
				if (strlen(getDllName) > 1)
				{
					struct Dll* dllName = (struct Dll*)malloc(sizeof(struct Dll));
					if (dllName == NULL)
					{
						char* str = strerror(GetLastError());
						LogError(str);
						return;
					}
					strcpy(dllName->DLLName, getDllName);
					DllLinkedList(dllName);
					counterDLL++;
				}
			}
		}
	}

	newProcess->DLLCount = counterDLL;
	newProcess->dll = HeadD;



	struct Dll* tempPrevDll = prevProcess->dll;
	struct Dll* tempNewDll = newProcess->dll;

	while (prevProcess != NULL)
	{
		if (prevProcess->processID == newProcess->processID)
		{
			prevProcess->memoryInfo.PageFaultCount += newProcess->memoryInfo.PageFaultCount;
			prevProcess->memoryInfo.WorkingSetSize += newProcess->memoryInfo.WorkingSetSize;
			prevProcess->memoryInfo.QuotaPagedPoolUsage += newProcess->memoryInfo.QuotaPagedPoolUsage;
			prevProcess->memoryInfo.QuotaPeakPagedPoolUsage += newProcess->memoryInfo.QuotaPeakPagedPoolUsage;
			prevProcess->memoryInfo.PagefileUsage += newProcess->memoryInfo.PagefileUsage;

			while (tempNewDll != NULL)
			{
				tempPrevDll = prevProcess->dll;
				while (tempPrevDll != NULL)
				{
					if (strcmp(tempPrevDll->DLLName, tempNewDll->DLLName) == 0)
					{
						break;
					}

					if (tempPrevDll->next == NULL)
					{
						struct Dll* newDll = (struct Dll*)malloc(sizeof(struct Dll));
						strcpy(newDll->DLLName, tempNewDll->DLLName);
						printf("Prev DLL List: \n------------\n");
						PrintDLLList(prevProcess->dll);
						printf("The New DLL is: %s\n", tempNewDll->DLLName);

						AddNewDllToLinkedList(tempPrevDll, newDll);
						prevProcess->DLLCount++;
						prevSnapShot->DllCountInSnap++;
						printf("\nNew DLL List: \n------------\n");
						PrintDLLList(prevProcess->dll);
						
						break;
					}

					tempPrevDll = tempPrevDll->next;
				}

				tempNewDll = tempNewDll->next;
			}

			while (newProcess->dll != NULL)
			{
				tempNewDll = newProcess->dll; 
				newProcess->dll = newProcess->dll->next;
				free(tempNewDll);
			}

			free(newProcess);
			break;
		}
		 
		prevProcess = prevProcess->next;
	}

	if (prevProcess == NULL)
	{
		//newProcess->dll = tempNewDll;
		//printf("Prev Processes List:\n------------\n");
		//PrintProcessList(prevSnapShot->process);
		//printf("The New Process is: %s\n", newProcess->processName);

		AddNewProcessToLinkedList(TailP, newProcess);
		prevSnapShot->processCount++;
		prevSnapShot->DllCountInSnap += newProcess->DLLCount;
		//printf("\nNew Processes List is:\n------------\n");
		//PrintProcessList(prevSnapShot->process);
	}


	CloseHandle(hProcess);
}

//void Replace(struct Process* item)
//{
//	if (item == NULL) //No Items in the list
//	{
//		return;
//	}
//	else if (item->next == NULL) //One Item in the list
//	{
//		return;
//	}
//	else if (item == HeadP && item->next == TailP) //2 Items in the list: Head and Tail
//	{
//		item->prev = TailP;
//		item->next = NULL;
//		TailP->prev = NULL;
//		TailP->next = item;
//		TailP = item;
//		HeadP = item->prev;
//	}
//	else if (item == HeadP && item->next != NULL) //2 first Items in the list
//	{
//		item->next = item->next->next;
//		item->next->prev->next = item;
//		item->next->prev->prev = NULL;
//		item->prev = item->next->prev;
//		item->next->prev = item;
//		HeadP = item->prev;
//	}
//	else if (item->next == TailP && item->prev != NULL) //2 last Items in the list
//	{
//		item->prev->next = item->next;
//		item->next->prev = item->prev;
//		item->next->next = item;
//		item->prev = item->next;
//		item->next = NULL;
//		TailP = item;
//	}
//	else //2 middle Items
//	{
//		item->prev->next = item->next;
//		item->next = item->next->next;
//		item->next->prev->prev = item->prev;
//		item->prev = item->next->prev;
//		item->next->prev->next = item;
//		item->next->prev = item;
//	}
//}
//
////Sorts the list
//void BubbleSort()
//{
//	//Starting to sort the list
//	struct Process* currentItem = HeadP;
//	if (currentItem == NULL)
//	{
//		LogError("The List is empty");
//		return;
//	}
//
//	LogEvent("Swap Item");
//	int counter = 1;
//	while (counter != 0)
//	{
//		counter = 0;
//		currentItem = HeadP;
//
//		while (currentItem != NULL)
//		{
//			if (currentItem->next != NULL && currentItem->processID > currentItem->next->processID)
//			{
//				Replace(currentItem);
//				counter++;
//				currentItem = currentItem->prev;
//			}
//
//			currentItem = currentItem->next;
//		}
//	}
//}



