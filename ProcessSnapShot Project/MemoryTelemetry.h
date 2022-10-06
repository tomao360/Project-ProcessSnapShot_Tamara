#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "LogFile.h"
#include "LinkedLists.h"


struct SnapShot* GetProcessesInfo(struct SnapShot*);
struct Process* PrintMemoryInfo(DWORD);
//void DllLinkedList(struct Dll*);
//void ProcessLinkedList(struct Process*);
//struct SnapShot* AccumulateSnapShots(struct SnapShot*, struct SnapShot*);
void AccumulateSnapShots(struct SnapShot*, DWORD);
//void AddNewDllToLinkedList(struct Dll*, struct Dll*);
//void AddNewProcessToLinkedList(struct Process*, struct Process*);

//void PrintProcessList();
//void PrintDLLList();

