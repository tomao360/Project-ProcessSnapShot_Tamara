#include "MemoryTelemetry.h"

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

	struct Process* myProcess = (struct Process*)malloc(sizeof(struct Process));
	if (myProcess == NULL)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return NULL;
	}

	myProcess->processID = processID;

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
			strcpy(myProcess->processName, processName);
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
		myProcess->memoryInfo.PageFaultCount = pmc.PageFaultCount;
		myProcess->memoryInfo.WorkingSetSize = pmc.WorkingSetSize;
		myProcess->memoryInfo.QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
		myProcess->memoryInfo.QuotaPeakPagedPoolUsage = pmc.QuotaPeakPagedPoolUsage;
		myProcess->memoryInfo.PagefileUsage = pmc.PagefileUsage;
	}


	// Get Dlls List
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

				struct Dll* dllName = (struct Dll*)malloc(sizeof(struct Dll));
				if (dllName == NULL)
				{
					char* str = strerror(GetLastError());
					LogError(str);

					return;
				}

				// Convert wChar to regular char array (string)
				char getDllName[MAX_PATH];
				size_t numConverted;
				wcstombs_s(&numConverted, getDllName, MAX_PATH, wDllName, MAX_PATH);
				if (strlen(getDllName) > 1)
				{
					strcpy(dllName->DLLName, getDllName);
					DllLinkedList(dllName);
				}
			}
		}

		//PrintDLLList();
		myProcess->DLLCount = i;
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

	HeadP = NULL;
	TailP = NULL;
	int processCount = 0;

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
			}

			mySnapShot->processCount = processCount;
		}

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

		return prevSnapShot;
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



	struct Dll* tempDll = prevProcess->dll;
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
				prevProcess->dll = tempDll;
				while (prevProcess->dll != NULL)
				{
					if (strcmp(prevProcess->dll->DLLName, newProcess->dll->DLLName) == 0)
					{
						break;
					}

					if (prevProcess->dll->next == NULL)
					{
						struct Dll* newDll = (struct Dll*)malloc(sizeof(struct Dll));
						*newDll = *(newProcess->dll);
						AddNewDllToLinkedList(prevProcess->dll, newDll);
						prevSnapShot->process->DLLCount++;
						break;
					}

					prevProcess->dll = prevProcess->dll->next;
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
		newProcess->dll = tempNewDll;
		AddNewProcessToLinkedList(prevProcess->prev, newProcess); //שןלחת את הזנב כי אני מגיה לNULL
		prevSnapShot->processCount++;
	}


	CloseHandle(hProcess);
}



/*void PrintProcessList()
{
	struct Process* currentProcess = HeadP;

	int i = 0;
	printf("The Process List:\n");
	while (currentProcess != NULL)
	{
		i++;
		printf("%d: %s\n",i, currentProcess->processName);
		currentProcess = currentProcess->next;
	}
}

void PrintDLLList()
{
	struct Dll* currentDLL = HeadD;

	int i = 0;
	printf("The DLL List:\n");
	while (currentDLL != NULL)
	{
		i++;
		printf("%d: %s\n", i, currentDLL->DLLName);
		currentDLL = currentDLL->next;
	}
}*/





