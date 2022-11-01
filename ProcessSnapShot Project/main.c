#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#pragma warning(disable : 4996)

#include "LogFile.h"
#include "Structs.h"
#include "LinkedLists.h"
#include "ResetCollections.h"
#include "SaveOrLoadFile.h"
#include "TakeSnapShot.h"
#include "BubbleSort.h"
#include "DictionaryDLL.h"
#include "DictionaryProcess.h"
#include "HelpForDebug.h"
#include "GenerateHTML.h"


struct SnapShot_Header snapShotFileHeader;

int main()
{
	time_t t;
	time(&t);
	struct tm* timeinfo;
	struct SnapShot* SnapShot = NULL;
	struct SnapShot* SnapShotList = NULL;
	struct DLL_Dictionary* DictionaryDll;
	struct Process_Dictionary* DictionaryProcess;
	char str[1000];
	

	//initialization of struct SnapShot_Header 
	snapShotFileHeader.version = 1;
	snapShotFileHeader.SnapShotCount = 0;

	char userResponse = NULL;
	while (userResponse != 'E')
	{
		printf("\nPlease choose one of the following options:\n1-Take One Process SnapShot\n2-Take 20 Process SnapShots for 20 seconds (accumulating the data)\n3-Start Long SnapShot\n4-End Long SnapShot\n5-Generate HTML Report\n6-Reset Collections\n7-Save to File\n8-Load from File\n'E'-Exit the Program\n");
		userResponse = getch();
		switch (userResponse)
		{
		case '1': //Take One Process SnapShot
			timeinfo = localtime(&t);
			sprintf(str, "%d.%02d.%02d: %02d:%02d:%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			SnapShot = TakeOneSnapShot(NULL);
			strcpy(SnapShot->sampleTime, str);
			SnapShot->sampleID = snapShotIDCounter;
			LogEvent("Start Bubble Sorting the linked list");
			BubbleSort();
			SnapShot->process = HeadP;
			SnapShotList = SnapShotLinkedList(SnapShot);
			//PrintProcessList();  //For Debug
			break;
		case '2': //Take 20 Process SnapShots for 20 seconds (accumulating the data) -> Gives 1 List 
			timeinfo = localtime(&t);
			sprintf(str, "%d.%02d.%02d: %02d:%02d:%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			SnapShot = Take20SnapShotsIn20Seconds();
			strcpy(SnapShot->sampleTime, str);
			SnapShot->sampleID = snapShotIDCounter;
			LogEvent("Start Bubble Sorting the linked list");
			BubbleSort();
			SnapShot->process = HeadP;
			SnapShotList = SnapShotLinkedList(SnapShot);
			break;
		case '3': //Start Long SnapShot (ends when the user presses '4')
			timeinfo = localtime(&t);
			sprintf(str, "%d.%02d.%02d: %02d:%02d:%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			SnapShot = TakeLongSnapShot();
			strcpy(SnapShot->sampleTime, str);
			SnapShot->sampleID = snapShotIDCounter;
			LogEvent("Start Bubble Sorting the linked list");
			BubbleSort();
			SnapShot->process = HeadP;
			SnapShotList = SnapShotLinkedList(SnapShot);
			break;
		case '5': //Generate HTML Report
			LogEvent("Start generate HTML report");
			LogEvent("Start making a DLL Dictionary");
			DictionaryDll = MakeDllDictionary(HeadS);
			LogEvent("Start making a Processes Dictionary");
			DictionaryProcess = MakeProcessDictionary(HeadS);
			generateHtml();
			LogEvent("Generating HTML report has finished");
			LogEvent("Start Reset Dll and Process Dictionaries");
			ResetDllDictionary();
			ResetProcessDictionary();
			LogEvent("Reset Dll and Process Dictionaries has finished");
			break;
		case '6': //Reset Collections - Deleting all SanpShots from the memory
		    LogEvent("Start Reset Collections");
			ResetCollections();
			SnapShotList = NULL;
			LogEvent("Reset Collections has finished");
			break;
		case '7': //Save to File
			SaveIntoFile();
			break;
		case '8': //Load from File
			SnapShotList = LoadFromFile();
			//PrintSnapShot();  //For Debug
			break;
		case 'E': //Exit the program
			LogEvent("Exit the program");
			ResetCollections();
			printf("Thank you for using my program.\n");
			break;

		default:
			printf("Wrong selection, please try again\n");
			break;
		}
	}

	return 0;
}


