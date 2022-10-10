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


//struct Dll_Header
//{
//	int version;
//	char reserved[100];
//};
//extern struct Dll_Header dllFileHeader;
//
//struct Process_Header
//{
//	int version;
//	char reserved[100];
//};
//extern struct Process_Header processFileHeader;

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
	struct Process* processDictionary;
	struct Dictionary* next;
	struct Dictionary* prev;
};
extern struct DLL_Dictionary* Head_Dictionary;
extern struct DLL_Dictionary* Tail_Dictionary;
