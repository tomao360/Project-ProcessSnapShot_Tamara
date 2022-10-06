#include "TakeSnapShot.h"

struct SnapShot* TakeOneSnapShot(struct SnapShot* prevSnapShot)
{
	struct SnapShot* singleSnapShot = GetProcessesInfo(prevSnapShot);

	return singleSnapShot;
}

struct SnapShot* Take20SnapShotsIn20Seconds()
{
	struct SnapShot* singleSnapShot = NULL;

	for (int i = 0; i < 20; i++)
	{
		singleSnapShot = TakeOneSnapShot(singleSnapShot);
		Sleep(1000);
	}

	return singleSnapShot;
}