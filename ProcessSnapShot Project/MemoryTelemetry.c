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
#include "HelpForDebug.h"

//Initializing global variables
struct Process* HeadP = NULL;
struct Process* TailP = NULL;

struct Dll* HeadD = NULL;
struct Dll* TailD = NULL;


struct Process* PrintMemoryInfo(DWORD processID)
{
	HeadD = NULL;
	TailD = NULL;
	struct Process* myProcess;  //Defining a pointer variable of type struct Process in order to enter the data into it
	size_t numConverted;

	HANDLE hProcess;  //Pointer to process
	PROCESS_MEMORY_COUNTERS pmc;

	//Open a process in order to receive information
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess) //If it cannot be opened, return NULL 
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return NULL;
	}

	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR FoundProcessName[MAX_PATH];
	TCHAR wDllName[MAX_PATH];

	//Get Process Name
	if (GetModuleFileNameEx(hProcess, 0, FoundProcessName, MAX_PATH))
	{
		//At this point, FoundProcessName contains the full path to the executable

		char processName[MAX_PATH];
		wcstombs_s(&numConverted, processName, MAX_PATH, FoundProcessName, MAX_PATH);
		if (strlen(processName) > 0)  //If the process name does not contain characters, return NULL
		{
			myProcess = (struct Process*)malloc(sizeof(struct Process));
			if (myProcess == NULL)
			{
				char* str = strerror(GetLastError());
				LogError(str);

				return;
			}

			strcpy(myProcess->processName, processName);
		}
		else
		{
			return NULL;
		}
	}
	else  //If cant get process name, return
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return;
	}

	myProcess->processID = processID;

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))  //According to the handle of the process I will receive a pmc (pmc is a struct that contains the data about the memory)
	{
		myProcess->memoryInfo.PageFaultCount = pmc.PageFaultCount;
		myProcess->memoryInfo.WorkingSetSize = pmc.WorkingSetSize;
		myProcess->memoryInfo.QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
		myProcess->memoryInfo.QuotaPeakPagedPoolUsage = pmc.QuotaPeakPagedPoolUsage;
		myProcess->memoryInfo.PagefileUsage = pmc.PagefileUsage;
	}


	//Get Dlls List
	int counterDLL = 0;
	int i = 0;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))  //The function receives the handle of the process (a pointer to it) and an array into which it will put the handles of the modules
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			//Get the full path to the module's file.
			if (GetModuleFileNameEx(hProcess, hMods[i], wDllName, MAX_PATH))
			{
				//*Get the module name and handle value.
				//Convert wChar to regular char array (string)
				char getDllName[MAX_PATH];
				wcstombs_s(&numConverted, getDllName, MAX_PATH, wDllName, MAX_PATH);
				if (strlen(getDllName) > 0)  //If the DLL name contains more than 0 characters, add him to the linked list
				{
					struct Dll* dllName = (struct Dll*)malloc(sizeof(struct Dll));
					if (dllName == NULL)
					{
						char* str = strerror(GetLastError());
						LogError(str);

						return;
					}

					strcpy(dllName->DLLName, getDllName);
					DllLinkedList(dllName);  //Creating a linked list of Dlls
					counterDLL++;
				}
			}
		}

		//PrintDLLList();  //For Debug

		myProcess->DLLCount = counterDLL;
		myProcess->dll = HeadD;
	}

	
	if (i == 0)  //If there are no DLL's in the process
	{
		myProcess->DLLCount = 0;
		myProcess->dll = NULL;
	}

	CloseHandle(hProcess);

	return myProcess;  //Return MyProcess to create a linked list of processes
}


struct SnapShot* GetProcessesInfo(struct SnapShot* prevSnapShot)
{
	//Get Processes
	
	//Receive all processes ID
	DWORD aProcesses[1024], cbNeeded, cProcesses;  //DWORD aProcesses[1024] -> An array into which the processes IDs will be entered
	unsigned int i;

	//*Receive all process ID and put in aProcesses Array
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))  //The function inserts into the array the entire list of processes running in the system
	{
		LogError("There are no processes in the system");

		return 1;
	}

	//Calculate how many process identifiers were returned
	cProcesses = cbNeeded / sizeof(DWORD);

	if (prevSnapShot == NULL) 
	{
		HeadP = NULL;
		TailP = NULL;
		int processCount = 0;
		int dllCountInSnap = 0;

		//Defining a pointer variable of type struct SnapShot in order to enter the data into it
		struct SnapShot* mySnapShot = (struct SnapShot*)malloc(sizeof(struct SnapShot));
		if (mySnapShot == NULL)
		{
			char* str = strerror(GetLastError());
			LogError(str);

			return;
		}

		//*Loop of all processes
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

		return mySnapShot;  //Return mySnapShot to create a linked list of SnapShots
	}
	else if (prevSnapShot != NULL)
	{
		//*Loop of all processes
		for (i = 0; i < cProcesses; i++)
		{
			AccumulateSnapShots(prevSnapShot, aProcesses[i]);  //Send to a function that accumulates data
		}

		//ProcessIDBubbleSort();  //For Debug
		//PrintProcessIDList(HeadP);  //For Debug

		prevSnapShot->process = HeadP;

		return prevSnapShot;
	}
}


void AccumulateSnapShots(struct SnapShot* prevSnapShot, DWORD processID)
{
	HeadD = NULL;
	TailD = NULL;
	struct Process* prevProcess = prevSnapShot->process;  //The processes of prev SnapShot
	struct Process* newProcess;  //Defining a pointer variable of type struct Process in order to enter the data into it
	size_t numConverted;

	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	//Open process in order to receive information
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
	
	//Get Process Name
	if (GetModuleFileNameEx(hProcess, 0, FoundProcessName, MAX_PATH))
	{
		//At this point, buffer contains the full path to the executable
		char processName[MAX_PATH];
		wcstombs_s(&numConverted, processName, MAX_PATH, FoundProcessName, MAX_PATH);
		if (strlen(processName) > 0)   //If the process name does not contain characters, return NULL
		{
			newProcess = (struct Process*)malloc(sizeof(struct Process));
			if (newProcess == NULL)
			{
				char* str = strerror(GetLastError());
				LogError(str);

				return;
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

	//Get Dlls List
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
				if (strlen(getDllName) > 0)   //If the DLL name contains more than 0 characters, add him to the linked list
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

	//Temporary variables with which we will run on the DLLs lists
	struct Dll* tempPrevDll = prevProcess->dll;
	struct Dll* tempNewDll = newProcess->dll;

	while (prevProcess != NULL)
	{ 
		//If its the same process, accumulate data
		if (prevProcess->processID == newProcess->processID && strcmp(prevProcess->processName, newProcess->processName) == 0)  
		{
			prevProcess->memoryInfo.PageFaultCount += newProcess->memoryInfo.PageFaultCount;
			prevProcess->memoryInfo.WorkingSetSize += newProcess->memoryInfo.WorkingSetSize;
			prevProcess->memoryInfo.QuotaPagedPoolUsage += newProcess->memoryInfo.QuotaPagedPoolUsage;
			prevProcess->memoryInfo.QuotaPeakPagedPoolUsage += newProcess->memoryInfo.QuotaPeakPagedPoolUsage;
			prevProcess->memoryInfo.PagefileUsage += newProcess->memoryInfo.PagefileUsage;

			//Check the DLLs lists and add to prevProcess if its new
			while (tempNewDll != NULL)
			{
				tempPrevDll = prevProcess->dll;  //Keeps the start (head) of the list
				while (tempPrevDll != NULL)
				{
					if (strcmp(tempPrevDll->DLLName, tempNewDll->DLLName) == 0)
					{
						break;
					}

					if (tempPrevDll->next == NULL)
					{
						struct Dll* newDll = (struct Dll*)malloc(sizeof(struct Dll));
						if (newDll == NULL)
						{
							char* str = strerror(GetLastError());
							LogError(str);

							return;
						}

						strcpy(newDll->DLLName, tempNewDll->DLLName);
						//printf("Prev DLL List: \n------------\n");   //For Debug
						//PrintDLLList(prevProcess->dll);
						//printf("The New DLL is: %s\n", tempNewDll->DLLName);   

						AddNewDllToLinkedList(tempPrevDll, newDll);
						LogEvent("A new DLL is added to the process");
						prevProcess->DLLCount++;
						prevSnapShot->DllCountInSnap++;
						//printf("\nNew DLL List: \n------------\n");   //For Debug
						//PrintDLLList(prevProcess->dll);
						
						break;
					}

					tempPrevDll = tempPrevDll->next;
				}

				tempNewDll = tempNewDll->next;
			}

			//Free newProcess and its DLL list at the end of data collection 
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

	if (prevProcess == NULL)  //if prevProcess == NULL -> its a new process
	{
		//printf("Prev Processes List:\n------------\n");   //For Debug
		//PrintProcessList(prevSnapShot->process);
		//printf("The New Process is: %s\n", newProcess->processName);

		AddNewProcessToLinkedList(TailP, newProcess);
		LogEvent("A new Process is added to the SnapShot");
		prevSnapShot->processCount++;
		prevSnapShot->DllCountInSnap += newProcess->DLLCount;
		//printf("\nNew Processes List is:\n------------\n");   //For Debug
		//PrintProcessList(prevSnapShot->process);
	}

	CloseHandle(hProcess);
}


