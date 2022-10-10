
void DllLinkedList(struct Dll*);
void ProcessLinkedList(struct Process*);
void AddNewDllToLinkedList(struct Dll*, struct Dll*);
void AddNewProcessToLinkedList(struct Process*, struct Process*);
struct SnapShot* SnapShotLinkedList(struct SnapShot*);

extern int snapShotIDCounter;
