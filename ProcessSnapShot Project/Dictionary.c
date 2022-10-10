//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <Windows.h>
//#include <Psapi.h>
//#pragma warning(disable : 4996)
//
//#include "MemoryTelemetry.h"
//#include "LogFile.h"
//#include "Structs.h"
//#include "LinkedLists.h"
//
//struct DLL_Dictionary* Head_Dictionary = NULL;
//struct DLL_Dictionary* Tail_Dictionary = NULL;
//
////Insert values into the dictionary: key and val
//void AddToDictionary(char dllDictionary_key[MAX_PATH], struct Process* processDictionary_val)
//{
//	struct DLL_Dictionary* currentItem = (struct DLL_Dictionary*)malloc(sizeof(struct DLL_Dictionary));
//	if (currentItem == NULL)
//	{
//		printf("ERROR - Not enough memory");
//		return;
//	}
//
//	struct DLL_Dictionary* headOfDictionary = (struct DLL_Dictionary*)malloc(sizeof(struct DLL_Dictionary));
//	if (currentItem == NULL)
//	{
//		printf("ERROR - Not enough memory");
//		return;
//	}
//
//	currentItem->processDictionary = (struct Process*)malloc(sizeof(struct Process));
//	
//		
//		strcpy(currentItem->keyDLL, dllDictionary_key);
//		strcpy(currentItem->processDictionary->processName, processDictionary_val->processName);
//
//		if (Head_Dictionary == NULL)
//		{
//			Head_Dictionary = currentItem;
//			Tail_Dictionary = currentItem;
//			currentItem->next = NULL;
//			currentItem->prev = NULL;
//		}
//		else
//		{
//			while (headOfDictionary != NULL)
//			{
//				if (strcmp(headOfDictionary->keyDLL, dllDictionary_key) == 0)
//				{
//					AddNewValueToDictionary(headOfDictionary, processDictionary_val);
//					free(currentItem);
//				}
//				if (headOfDictionary->next != NULL)
//				{
//					headOfDictionary->next = 
//				}
//
//			}
//			currentItem->prev = Tail_Dictionary;
//			currentItem->next = NULL;
//			Tail_Dictionary->next = currentItem;
//			Tail_Dictionary = currentItem;
//		}
//	
//}
//
//
//void MakeDllDictionary(struct SnapShot* SnapShotHead)
//{
//	struct SnapShot* currentSnapShot = SnapShotHead;
//	struct Process* currentProcess;
//	struct Dll* currentDLL;
//
//	while (currentSnapShot != NULL)
//	{
//		currentProcess = currentSnapShot->process;
//
//		while (currentProcess != NULL)
//		{
//			currentDLL = currentProcess->dll;
//
//			while (currentDLL != NULL)
//			{
//				AddToDictionary(currentDLL->DLLName, currentProcess);
//				currentDLL = currentDLL->next;
//			}
//
//			currentProcess = currentProcess->next;
//		}
//
//		currentSnapShot = currentSnapShot->next;
//	}
//}
////
////void AddNewValueToDictionary()
////{
////
////}