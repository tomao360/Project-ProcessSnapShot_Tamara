#include <Windows.h>
#include <Psapi.h>

struct DLL_Dictionary* MakeDllDictionary(struct SnapShot* SnapShotHead);
void AddToDllDictionary(char dllDictionary_key[MAX_PATH], struct Process* processDictionary_val);
void AddNewProcessToDll(struct Process* currentProcess, struct Process* newProcessDict, struct DLL_Dictionary* currenDllDict);
int NumOfDllInAllSnapShots(struct DLL_Dictionary* snapShotHead);