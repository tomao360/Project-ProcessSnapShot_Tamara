//#include <stdlib.h> 
//#include <stdio.h>
//#include <string.h>
//#pragma warning(disable:4996)
//
//
//#include "GenerateHtml.h"
//#include "Structs.h"
//#include "LogFile.h"
//
//FILE* file;
//FILE* fileR;
//int countOfCreateFileIndex2 = 1;
//int countOfCreateFileIndex3 = 1;
//
//
//// To know which file to open
//char* newNameOfFile = 0;
//
//
//int firstTimeInFile = 0;
//int firstTimeHomePage = 0;
//char seperator;
//char nameFile[50] = "";
//
//#define SEPERATOR "</tbody>"
//#define SEPERATOR_TITLE "</p>"
//
//
//char* readFromFile()
//{
//	if (firstTimeInFile == 0 && strcmp(nameFile, "index.html") != 0)
//	{
//		fileR = fopen(nameFile, "r");
//		if (!fileR)
//		{
//			return NULL;
//		}
//	}
//	else if (firstTimeHomePage == 0 && firstTimeInFile == 0)
//	{
//		fileR = fopen(nameFile, "r");
//		if (!fileR)
//		{
//			return NULL;
//		}
//	}
//	else if (strcmp(nameFile, "index.html") == 0)
//	{
//		fileR = fopen("homePage.html", "r");
//		if (!fileR)
//		{
//			//error
//			return;
//		}
//	}
//	else if (strcmp(nameFile, "index2.html") == 0)
//	{
//		fileR = fopen(newNameOfFile, "r");
//		if (!fileR)
//		{
//			//error
//			return;
//		}
//	}
//	else if (strcmp(nameFile, "index3.html") == 0)
//	{
//		fileR = fopen(newNameOfFile, "r");
//		if (!fileR)
//		{
//			//error
//			return;
//		}
//	}
//
//	// Get the file size
//	char* charCount = (char*)malloc(1000);
//	if (!charCount)
//	{
//		return;
//	}
//
//	char* read;
//	int fileSize = 0;
//
//	while ((read = fgets(charCount, 1000, fileR)))
//	{
//		fileSize += strlen(charCount);
//	}
//
//	free(charCount);
//	fclose(fileR);
//	fileSize++;
//	//fileSize += 2;
//
//	// alloc space as file size
//	char* inThefile = (char*)malloc(fileSize);
//	if (!inThefile)
//	{
//		return 1;
//	}
//
//
//	if (firstTimeInFile == 0 && strcmp(nameFile, "index.html") != 0)
//	{
//		fileR = fopen(nameFile, "r");
//		if (!fileR)
//		{
//			return NULL;
//		}
//	}
//	else if (firstTimeHomePage == 0 && firstTimeInFile == 0)
//	{
//		firstTimeHomePage = 1;
//		fileR = fopen(nameFile, "r");
//		if (!fileR)
//		{
//			return NULL;
//		}
//	}
//	else if (strcmp(nameFile, "index.html") == 0)
//	{
//		fileR = fopen("homePage.html", "r");
//		if (!fileR)
//		{
//			//error
//			return 0;
//		}
//	}
//	else if (strcmp(nameFile, "index2.html") == 0)
//	{
//		fileR = fopen(newNameOfFile, "r");
//		if (!fileR)
//		{
//			//error
//			return 0;
//		}
//	}
//	else if (strcmp(nameFile, "index3.html") == 0)
//	{
//		fileR = fopen(newNameOfFile, "r");
//		if (!fileR)
//		{
//			//error
//			return 0;
//		}
//	}
//
//	int readPosition = 0;
//	char charToRead;
//
//	while ((charToRead = fgetc(fileR)) != EOF)
//	{
//		inThefile[readPosition] = charToRead;
//		readPosition++;
//	}
//	inThefile[readPosition] = NULL;
//
//
//	fclose(fileR);
//
//	return inThefile;
//}
//
//
//char* displaySnapShotInTable(struct SnapShot* snapShot_html, char* NameProcessHtml, int countDLLInSnapHTML, SIZE_T sizeOfMemoryHTML) 
//{
//
//	char* lineInTable = (char*)malloc(sizeof(MAX_PATH));
//	lineInTable[0] = NULL;
//	if (!lineInTable)
//	{
//		//error
//		return 1;
//	}
//	sprintf(lineInTable, "<tr>\n<td> %d </td>\n<td><a href =\"%s\" >Sample%d</a></td>\n<td> %d </td>\n<td> %d </td>\n<td> %d </td>\n</tr>\n", snapShot_html->sampleID, NameProcessHtml, snapShot_html->sampleID, snapShot_html->processCount, countDLLInSnapHTML, sizeOfMemoryHTML);
//	return lineInTable;
//
//
//}
//
//char* displayDLLSInProcess(struct Process* htmlProcess) 
//{
//	struct Dll* DLLOption = htmlProcess->dll;
//	char* lineInTable = (char*)malloc(sizeof(struct Dll) + 200);
//	char* origin = lineInTable;
//	if (!lineInTable)
//	{
//		//error
//		return 1;
//	}
//	char* allTheOptions = (char*)malloc(sizeof(struct Dll) * htmlProcess->DLLCount + 10000);
//	if (!allTheOptions)
//	{
//		//error
//		return 1;
//	}
//	allTheOptions[0] = NULL;
//	while (DLLOption != NULL)
//	{
//
//		sprintf(lineInTable, "<option> %s </option>\n", DLLOption->DLLName);
//		DLLOption = DLLOption->next;
//		strcat(allTheOptions, lineInTable);
//
//
//	}
//
//	free(origin);
//	return allTheOptions;
//}
//
//char* displayProcessesInTable(struct SnapShot* snapShot_html) 
//{
//
//	struct Process* process = snapShot_html->process;
//	char* lineInTable = (char*)malloc(sizeof(struct Process) * 1000);
//	if (!lineInTable)
//	{
//		//error
//		return 1;
//	}
//	char* allTheProcesses = (char*)malloc(sizeof(struct Process) * snapShot_html->processCount + 1000000);
//	if (!allTheProcesses)
//	{
//		//error
//		return 1;
//	}
//	allTheProcesses[0] = NULL;
//	char* optionOfDLL = NULL;
//	while (process != NULL)
//	{
//		optionOfDLL = displayDLLSInProcess(process);
//		lineInTable[0] = NULL;
//		sprintf(lineInTable, "<tr>\n<td> %s </td>\n<td> %d </td>\n<td> %d </td>\n<td> %d </td>\n<td> %d </td>\n<td> %d </td>\n<td> %d </td>\n<td> %d </td>\n<td><select>\n<option selected>dll's list</option>\n%s</select></td>\n</tr>", process->processName, process->processID, process->memoryInfo.PageFaultCount, process->memoryInfo.WorkingSetSize, process->memoryInfo.QuotaPagedPoolUsage, process->memoryInfo.QuotaPeakPagedPoolUsage, process->memoryInfo.PagefileUsage, process->DLLCount, optionOfDLL);
//		process = process->next;
//		strcat(allTheProcesses, lineInTable);
//
//	}
//	free(lineInTable);
//	free(optionOfDLL);
//
//	return allTheProcesses;
//}
//
//
//char* displayProcessesInTitle(struct SnapShot* snapShot_html) 
//{
//
//	char* lineInTable = (char*)malloc(100);
//	lineInTable[0] = NULL;
//	if (!lineInTable)
//	{
//		//error
//		return 1;
//	}
//	sprintf(lineInTable, "<h1> Samples List Number : %d At %s </h1>", snapShot_html->sampleID, snapShot_html->sampleTime);
//	
//	return lineInTable;
//}
//
//
////char* dynamicNav()
////{
////	char* dataNav = (char*)malloc(100);
////	dataNav[0] = NULL;
////	if (!dataNav)
////	{
////		//error
////		return 1;
////	}
////	sprintf(dataNav, "\n<div> Dll's cnt: %d </div>\n<div> Processes cnt: %d </div>\n<div> Memory avg: %d </div>\n");
////
////	return dataNav;
////}
//
//
//char* dynamicHtml(char* nameOfFile, char* addString, char* topHtml)
//{
//	strcpy(nameFile, nameOfFile);
//	char* dynamicTable = addString;
//	char* htmlFileTemplate = readFromFile();
//	firstTimeInFile = 1;
//
//	char* found = strstr(htmlFileTemplate, SEPERATOR);
//	int len = found - htmlFileTemplate;
//	char* newFileSpace;
//
//	if (topHtml == NULL)
//	{
//		newFileSpace = (char*)malloc(strlen(htmlFileTemplate) + 1 + strlen(dynamicTable));
//		strncpy(newFileSpace, htmlFileTemplate, len);
//		newFileSpace[len] = NULL;
//	}
//	else
//	{
//		newFileSpace = (char*)malloc(strlen(found) + strlen(topHtml) + 1 + strlen(dynamicTable));
//		strcpy(newFileSpace, topHtml);
//		free(topHtml);
//	}
//
//	strcat(newFileSpace, dynamicTable);
//	strcat(newFileSpace, found);
//
//	char* nameForFile = saveInToFileHTML(newFileSpace);
//
//
//	free(htmlFileTemplate);
//	//free(dynamicTable);
//
//
//	return nameForFile;
//}
//
//
//
//char* dynamicTitleHtml(char* nameOfFile, char* addString, char* title) {
//
//	newNameOfFile = createNewName(nameOfFile);
//	strcpy(nameFile, nameOfFile);
//	char* dynamicTitle = title;
//	char* htmlFileTemplate = readFromFile();
//
//	char* found = strstr(htmlFileTemplate, SEPERATOR_TITLE);
//	int len = found - htmlFileTemplate;
//
//	char* newFileSpace = (char*)malloc(strlen(htmlFileTemplate) + 1 + strlen(dynamicTitle));
//	if (!newFileSpace)
//	{
//		//error
//		return;
//	}
//
//	strncpy(newFileSpace, htmlFileTemplate, len);
//	newFileSpace[len] = NULL;
//	strcat(newFileSpace, dynamicTitle);
//	char* found_2 = strstr(htmlFileTemplate, SEPERATOR);
//	int len_2 = found_2 - found;
//	strncat(newFileSpace, found, len_2);
//	newFileSpace[len_2 + len + strlen(dynamicTitle)] = NULL;
//	char* linkName = dynamicHtml(nameOfFile, addString, newFileSpace);
//
//
//	//free(dynamicTitle);
//	free(htmlFileTemplate);
//
//	return linkName;
//}
//
//
//
//char* saveInToFileHTML(char* newFile)
//{
//	//Enter the function PhotoCopying
//	if (strcmp(nameFile, "index.html") == 0)
//	{
//		file = fopen("homePage.html", "w");
//		if (!file)
//		{
//			//error - The file (PhotoCopying.html) did not open
//			return 1;
//		}
//
//		//The file (newName) was opened successfully
//		fputs(newFile, file);
//		fclose(file);
//	}
//	else if (strcmp(nameFile, "index2.html") == 0)
//	{
//		file = fopen(newNameOfFile, "w");
//		if (!file)
//		{
//			//error - The file (PhotoCopying.html) did not open
//			return 1;
//		}
//
//		//The file (newName) was opened successfully
//		fputs(newFile, file);
//		fclose(file);
//	}
//	else if (strcmp(nameFile, "index3.html") == 0)
//	{
//		file = fopen(newNameOfFile, "w");
//		if (!file)
//		{
//			//error - The file (PhotoCopying.html) did not open
//			return 1;
//		}
//
//		//The file (newName) was opened successfully
//		fputs(newFile, file);
//		fclose(file);
//	}
//	else
//	{
//		//no exist file
//		printf("no exist file\n");
//		return 1;
//	}
//
//	free(newFile);
//
//	return newNameOfFile;
//	//The file (PhotoCopying.html) is closed, the function PhotoCopying is finished
//}
//
//
//char* createNewName(char* nameOfOriginFile) {
//
//	char* newNameFile = (char*)malloc(30);
//
//	if (strcmp(nameOfOriginFile, "index2.html") == 0)
//	{
//		sprintf(newNameFile, "samplePage_%d.html", countOfCreateFileIndex2);
//		countOfCreateFileIndex2++;
//
//		return newNameFile;
//	}
//	else if (strcmp(nameOfOriginFile, "index3.html") == 0)
//	{
//		sprintf(newNameFile, "useDll_%d.html", countOfCreateFileIndex3);
//		countOfCreateFileIndex3++;
//
//		return newNameFile;
//	}
//	else
//	{
//		return NULL;
//	}
//}
//
//
////struct SnapShot* addingDll(struct SnapShot* snapShotHead)
////{
////	struct Process* currentProcess = snapShotHead->process;
////	struct Dll* currentDll;
////
////	int numberOfDll = 0;
////
////	while (currentProcess != NULL)
////	{
////		currentDll = currentProcess->dll;
////
////		while (currentDll != NULL)
////		{
////			numberOfDll++;
////			currentDll = currentDll->next;
////		}
////	}
////
////	return numberOfDll;
////}
//
//
//struct SnapShot* memoryAverage(struct SnapShot* snapShotHead)
//{
//	struct Process* currentProcess = snapShotHead->process;
//
//	SIZE_T sum = 0;
//	long avg;
//
//	while (currentProcess != NULL)
//	{
//		sum += currentProcess->memoryInfo.WorkingSetSize;
//		currentProcess = currentProcess->next;
//	}
//
//	avg = sum / snapShotHead->processCount;
//
//	return avg;
//}



///////////////////////////////////////
//firstTimeInFile = 0;
//char* temporaryHTML = displayProcessesInTable(SnapShot);
//char* temporaryTitle = displayProcessesInTitle(SnapShot);
//char* linkToSample = dynamicTitleHtml("index2.html", temporaryHTML, temporaryTitle);
////free(temporaryTitle);
//free(temporaryHTML);
//firstTimeInFile = 0;
//char* dynamicLineHtml = displaySnapShotInTable(SnapShot, linkToSample, SnapShot->DllCountInSnap, memoryAverage(SnapShot));
//free(linkToSample);
//char* nameF = dynamicHtml("index.html", dynamicLineHtml, NULL);
////free(nameF);
////free(dynamicLineHtml);
////////////////////////////////////////////