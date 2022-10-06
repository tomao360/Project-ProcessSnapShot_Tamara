#include "TakeSnapShot.h"

int main()
{
	time_t t;
	time(&t);
	struct tm* timeinfo;
	struct SnapShot* SnapShot;
	int snapShotIDCounter = 0;

	char userResponse = NULL;
	while (userResponse != 'E')
	{
		printf("\nPlease choose one of the following options:\n1-Take One Process SnapShot\n2-Take 20 Process SnapShots for 20 seconds (accumulating the data)\n3-Start Long SnapShot (ends when the user presses 'END')\n4-End Long SnapShot\n5-Generate HTML Report\n6-Reset Collections\n7-Save to File\n8-Load from File\n'E'-Exit the Program\n");
		userResponse = getch();
		switch (userResponse)
		{
		case '1': //Take One Process SnapShot
			timeinfo = localtime(&t);
			char str[1000];
			sprintf(str, "The Sample Time&Date: %02d.%02d.%d:%02d:%02d:%02d", timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			SnapShot = TakeOneSnapShot(NULL);
			strcpy(SnapShot->sampleTime, str);
			snapShotIDCounter++;
			SnapShot->sampleID = snapShotIDCounter;
			SnapShotLinkedList(SnapShot);
			//PrintProcessList();
			break;
		case '2': //Take 20 Process SnapShots for 20 seconds (accumulating the data) -> Gives 1 List 
			SnapShot = Take20SnapShotsIn20Seconds();
			snapShotIDCounter++;
			SnapShot->sampleID = snapShotIDCounter;
			SnapShotLinkedList(SnapShot);
			break;
		case '3': //Start Long SnapShot (ends when the user presses 'END')

			break;
		case '4': //End Long SnapShot

			break;
		case '5': //Generate HTML Report

			break;
		case '6': //Reset Collections - Deleting all SanpShots from the memory
			 ResetCollections();
			break;
		case '7': //Save to File

			break;
		case '8': //Load from File

			break;
		case 'E': //Exit the program
			printf("Thank you for using our program.\n");
			break;

		default:
			printf("Wrong selection, please try again\n");
			break;
		}
	}

	return 0;
}


