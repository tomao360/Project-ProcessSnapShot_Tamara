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

	struct SnapShot* mySnapShot = (struct SnapShot*)malloc(sizeof(struct SnapShot));
	if (mySnapShot == NULL)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		return;
	}

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

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

	// For each Process to get its Memory Information

	/*if (prevSnapShot != NULL)
	{
		mySnapShot = AccumulateSnapShots(prevSnapShot, mySnapShot);
	}*/

	return mySnapShot;
}


void PrintProcessList()
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
}





