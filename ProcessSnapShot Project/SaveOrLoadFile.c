#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)

#include "SaveOrLoadFile.h"
#include "LogFile.h"
#include "Structs.h"
#include "LinkedLists.h"


void SaveIntoFile()
{
	if (HeadS == NULL)
	{
		printf("There are no existing SnapShots");
		LogError("There are no existing SnapShots");
		return;
	}

	char fileName[100];
	printf("Please enter a file name: ");
	scanf("%s", fileName);

	FILE* f = fopen(fileName, "wb");
	if (f == NULL)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		printf("Error opening the file");

		return;
	}
	else
	{
		LogEvent("Start saving the SnapShots into a file");
		int write;
		struct SnapShot* currentSnapShot = HeadS;
		struct Process* currentProcess = currentSnapShot->process;
		struct Dll* currentDLL = currentProcess->dll;

		//Write the header to the file
		write = fwrite(&snapShotFileHeader, sizeof(struct SnapShot_Header), 1, f); 
		if (write == NULL)
		{
			char* str = strerror(GetLastError());
			LogError(str);

			return;
		}
		
		//Write the SnapShots to the file
		for (int i = 0; i < snapShotFileHeader.SnapShotCount; i++)
		{
			write = fwrite(currentSnapShot, sizeof(struct SnapShot), 1, f);
			if (write == NULL)
			{
				char* str = strerror(GetLastError());
				LogError(str);

				return;
			}

			currentProcess = currentSnapShot->process;

			while (currentProcess != NULL)
			{
				write = fwrite(currentProcess, sizeof(struct Process), 1, f);
				if (write == NULL)
				{
					char* str = strerror(GetLastError());
					LogError(str);

					return;
				}

				currentDLL = currentProcess->dll;

				while (currentDLL != NULL)
				{
					write = fwrite(currentDLL, sizeof(struct Dll), 1, f);
					if (write == NULL)
					{
						char* str = strerror(GetLastError());
						LogError(str);

						return;
					}

					currentDLL = currentDLL->next;
				}

				currentProcess = currentProcess->next;
			}

			currentSnapShot = currentSnapShot->next;
		}

		fclose(f);
	
		LogEvent("Saving the SnapShots into file has finished");
	}
}


struct SnapShot* LoadFromFile()
{
	HeadP = NULL;
	TailP = NULL;
	HeadD = NULL;
	TailD = NULL;
	
	char fileName[100];
	printf("Please enter a file name: ");
	scanf("%s", fileName);

	FILE* f = fopen(fileName, "rb");
	if (f == NULL)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		printf("Error opening the file");

		return;
	}
	else
	{
		LogEvent("Start loading a file and creating SnapShots");

		int read;
		struct SnapShot* currentSnapShot;
		struct Process* currentProcess;
		struct Dll* currentDLL;
		struct SnapShot* SnapShotList = NULL;
		
		//Read the header from the file
		read = fread(&snapShotFileHeader, sizeof(struct SnapShot_Header), 1, f); 
		if (read == 0)
		{
			char* str = strerror(GetLastError());
			LogError(str);

			return;
		}
		
		//Read the SnapShots from the file
		for (int i = 0; i < snapShotFileHeader.SnapShotCount; i++)
		{
			currentSnapShot = (struct SnapShot*)malloc(sizeof(struct SnapShot));
			if (currentSnapShot == NULL)
			{
				char* str = strerror(GetLastError());
				LogError(str);

				return;
			}

			read = fread(currentSnapShot, sizeof(struct SnapShot), 1, f);
			if (read == 0)
			{
				char* str = strerror(GetLastError());
				LogError(str);

				return;
			}

			currentProcess = (struct Process*)malloc(sizeof(struct Process));
			if (currentProcess == NULL)
			{
				char* str = strerror(GetLastError());
				LogError(str);

				return;
			}

			for (int j = 0; j < currentSnapShot->processCount; j++)
			{
				read = fread(currentProcess, sizeof(struct Process), 1, f);
				if (read == 0)
				{
					char* str = strerror(GetLastError());
					LogError(str);

					return;
				}

				currentDLL = (struct Dll*)malloc(sizeof(struct Dll));
				if (currentDLL == NULL)
				{
					char* str = strerror(GetLastError());
					LogError(str);

					return;
				}

				for (int x = 0; x < currentProcess->DLLCount; x++)
				{
					read = fread(currentDLL, sizeof(struct Dll), 1, f);
					if (read == 0)
					{
						char* str = strerror(GetLastError());
						LogError(str);

						return;
					}

					DllLinkedList(currentDLL);

					currentDLL = (struct Dll*)malloc(sizeof(struct Dll));
					if (currentDLL == NULL)
					{
						char* str = strerror(GetLastError());
						LogError(str);

						return;
					}
				}

				currentProcess->dll = HeadD; //Gets the HeadD of the DLL list
				HeadD = NULL;
				TailD = NULL;
				free(currentDLL);

				ProcessLinkedList(currentProcess);

				currentProcess = (struct Process*)malloc(sizeof(struct Process));
				if (currentSnapShot == NULL)
				{
					char* str = strerror(GetLastError());
					LogError(str);

					return;
				}
			}

			currentSnapShot->process = HeadP;  //Gets the HeadP of the Processes list
			free(currentProcess);

			currentSnapShot->sampleID = snapShotIDCounter;
			SnapShotList = SnapShotLinkedList(currentSnapShot);
			HeadP = NULL;
			TailP = NULL;
		}

		fclose(f);
		LogEvent("File uploading and creating the SnapShots linked list has finished");
		return SnapShotList;
	}
}




