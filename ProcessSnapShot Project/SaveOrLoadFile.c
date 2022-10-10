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
		//the snapShotFileHeader not exists
		printf("There are no existing Snapshots");
		return;
	}

	char fileName[100];
	printf("Please enter a file name: ");
	scanf("%s", fileName);

	FILE* f = fopen(fileName, "wb");
	if (f == NULL)
	{
		//ERROR
		return;
	}
	else
	{
		int write;
		struct SnapShot* currentSnapShot = HeadS;
		struct Process* currentProcess = currentSnapShot->process;
		struct Dll* currentDLL = currentProcess->dll;

		write = fwrite(&snapShotFileHeader, sizeof(struct SnapShot_Header), 1, f);
		if (write == NULL)
		{
			//error
			return;
		}
		//write = fwrite(&processFileHeader, sizeof(struct Process_Header), 1, f);
		//if (write == NULL)
		//{
		//	//error
		//	return;
		//}
		//write = fwrite(&dllFileHeader, sizeof(struct Dll_Header), 1, f);
		//if (write == NULL)
		//{
		//	//error
		//	return;
		//}

		for (int i = 0; i < snapShotFileHeader.SnapShotCount; i++)
		{
			write = fwrite(currentSnapShot, sizeof(struct SnapShot), 1, f);
			if (write == NULL)
			{
				//error
				return;
			}
			currentProcess = currentSnapShot->process;

			while (currentProcess != NULL)
			{
				write = fwrite(currentProcess, sizeof(struct Process), 1, f);
				if (write == NULL)
				{
					//error
					return;
				}

				currentDLL = currentProcess->dll;

				while (currentDLL != NULL)
				{
					write = fwrite(currentDLL, sizeof(struct Dll), 1, f);
					if (write == NULL)
					{
						//error
						return;
					}
					currentDLL = currentDLL->next;
				}

				currentProcess = currentProcess->next;
			}

			currentSnapShot = currentSnapShot->next;
		}

		fclose(f);
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
		//ERROR
		return;
	}
	else
	{
		int read;
		struct SnapShot* currentSnapShot;
		struct Process* currentProcess;
		struct Dll* currentDLL;
		struct SnapShot* SnapShotList = NULL;
		
		read = fread(&snapShotFileHeader, sizeof(struct SnapShot_Header), 1, f); //Read the header from the file
		if (read == 0)
		{
			//ERROR
			return;
		}
		//read = fread(&processFileHeader, sizeof(struct Process_Header), 1, f); //Read the header from the file
		//if (read == 0)
		//{
		//	//ERROR
		//	return;
		//}
		//read = fread(&dllFileHeader, sizeof(struct Dll_Header), 1, f); //Read the header from the file
		//if (read == 0)
		//{
		//	//ERROR
		//	return;
		//}

		for (int i = 0; i < snapShotFileHeader.SnapShotCount; i++) //Read the struct Item from the file
		{
			currentSnapShot = (struct SnapShot*)malloc(sizeof(struct SnapShot));
			if (currentSnapShot == NULL)
			{
				//ERROR
				return;
			}
			read = fread(currentSnapShot, sizeof(struct SnapShot), 1, f);
			if (read == 0)
			{
				//ERROR
				return;
			}

			currentProcess = (struct Process*)malloc(sizeof(struct Process));
			if (currentProcess == NULL)
			{
				//ERROR
				return;
			}
			for (int j = 0; j < currentSnapShot->processCount; j++)
			{
				read = fread(currentProcess, sizeof(struct Process), 1, f);
				if (read == 0)
				{
					//ERROR
					return;
				}
				currentDLL = (struct Dll*)malloc(sizeof(struct Dll));
				if (currentDLL == NULL)
				{
					//ERROR
					return;
				}
				for (int x = 0; x < currentProcess->DLLCount; x++)
				{
					read = fread(currentDLL, sizeof(struct Dll), 1, f);
					if (read == 0)
					{
						//ERROR
						return;
					}
					DllLinkedList(currentDLL);
					currentDLL = (struct Dll*)malloc(sizeof(struct Dll));
					if (currentDLL == NULL)
					{
						//ERROR
						return;
					}
				}

				currentProcess->dll = HeadD;
				HeadD = NULL;
				TailD = NULL;
				free(currentDLL);

				ProcessLinkedList(currentProcess);
				currentProcess = (struct Process*)malloc(sizeof(struct Process));
				if (currentSnapShot == NULL)
				{
					//ERROR
					return;
				}
			}

			currentSnapShot->process = HeadP;
			free(currentProcess);
			currentSnapShot->sampleID = snapShotIDCounter;
			SnapShotList = SnapShotLinkedList(currentSnapShot);
			HeadP = NULL;
			TailP = NULL;
		}

		fclose(f);
		return SnapShotList;
	}
}



void PrintSnapShot()
{
	struct SnapShot* currentSnapShot = HeadS;
	struct Process* currentProcess;
	struct Dll* currentDLL;

	int P = 1;
	int D = 1;

	while (currentSnapShot != NULL)
	{
		printf("SnapDhot Number: %d\n", currentSnapShot->sampleID);

		currentProcess = currentSnapShot->process;
		P = 1;
		while (currentProcess != NULL)
		{
			printf("\n#%d: Process: %s\n\n", P, currentProcess->processName);

			currentDLL = currentProcess->dll;
			D = 1;
			while (currentDLL != NULL)
			{
				printf("#%d: Dll: %s\n", D, currentDLL->DLLName);
				currentDLL = currentDLL->next;
				D++;
			}

			currentProcess = currentProcess->next;
			P++;
		}

		currentSnapShot = currentSnapShot->next;
	}
}
