#include <Windows.h>
#include <Psapi.h>

struct SnapShot* GetProcessesInfo(struct SnapShot* prevSnapShot);
struct Process* PrintMemoryInfo(DWORD processID);
void AccumulateSnapShots(struct SnapShot* prevSnapShot, DWORD processID);



