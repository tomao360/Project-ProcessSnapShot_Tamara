#include <windows.h>
#include <psapi.h>

struct Dll
{
	char DLLName[MAX_PATH];
	struct Dll* next;
	struct Dll* prev;
};
extern struct Dll* HeadD;
extern struct Dll* TailD;

struct Process
{
	unsigned long processID;
	char processName[MAX_PATH];
	PROCESS_MEMORY_COUNTERS memoryInfo;
	struct Dll* dll;
	int DLLCount;
	struct Process* next;
	struct Process* prev;
};
extern struct Process* HeadP;
extern struct Process* TailP;


struct SnapShot
{
	struct Process* process;
	int processCount;
	int sampleID;
	char sampleTime[100];
	struct SnapShot* next;
	struct SnapShot* prev;
};
extern struct SnapShot* HeadS;
extern struct SnapShot* TailS;


/*struct Process_Header
{
	int version;
	char reserved[1000];
};
struct Process_Header fileHeader;*/