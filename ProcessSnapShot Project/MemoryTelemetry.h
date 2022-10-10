#include <Windows.h>
#include <Psapi.h>

struct SnapShot* GetProcessesInfo(struct SnapShot*);
struct Process* PrintMemoryInfo(DWORD);
void PrintDLLList(struct Dll*);
void PrintProcessList(struct Process*);
void PrintProcessIDList(struct Process*);
void AccumulateSnapShots(struct SnapShot*, DWORD);

//void Replace(struct Process*);
//void BubbleSort();
//void PrintProcessList();
//void PrintDLLList();

