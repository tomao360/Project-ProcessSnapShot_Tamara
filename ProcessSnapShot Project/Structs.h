#include <Windows.h>
#include <Psapi.h>

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
	int DllCountInSnap;
	int sampleID;
	char sampleTime[100];
	struct SnapShot* next;
	struct SnapShot* prev;
};
extern struct SnapShot* HeadS;
extern struct SnapShot* TailS;


struct SnapShot_Header
{
	int version;
	int SnapShotCount;
	char reserved[100];
};
extern struct SnapShot_Header snapShotFileHeader;


struct DLL_Dictionary
{
	char keyDLL[MAX_PATH];
	int processCount;
	struct Process* processDictionary;
	struct DLL_Dictionary* next;
	struct DLL_Dictionary* prev;
};
extern struct DLL_Dictionary* HeadD_Dictionary;
extern struct DLL_Dictionary* TailD_Dictionary;


struct Process_Dictionary
{
	struct Process* processDictionarykey;
	struct Process_Dictionary* next;
	struct Process_Dictionary* prev;
};

extern struct Process_Dictionary* HeadP_Dictionary;
extern struct Process_Dictionary* TailP_Dictionary;