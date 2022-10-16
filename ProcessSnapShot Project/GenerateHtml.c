#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <Psapi.h>
#pragma warning(disable : 4996)

#include "GenerateHTML.h"
#include "LogFile.h"
#include "Structs.h"
#include "DictionaryDLL.h"
#include "DictionaryProcess.h"
#include "TakeSnapShot.h"


void generateHtml()
{
	if (HeadS == NULL && HeadD_Dictionary == NULL)
	{
		printf("There are no SnapShots from which HTML files can be produced\n");
		LogError("There are no SnapShots from which HTML files can be produced");

		return;
	}

	HomePageHtml();
	SampleTablePageHtml();
	DllTablePageHtml();

}


void HomePageHtml()
{
	char htmlHomePage[10000];
	
	int numOfDll = NumOfDllInAllSnapShots(HeadD_Dictionary);
	int numOfProcesses = NumOfProcessesInAllSnapShots(HeadP_Dictionary);
	SIZE_T memoryAvgInSnapShots = MemoryAverageInAllSnapShots(HeadS);

	struct SnapShot* currentSnapShot = HeadS;

	struct DLL_Dictionary* currentDllDictionarySnap = HeadD_Dictionary;
	int dllLineTableCounter = 1;

	FILE* f = fopen("My Website\\homePage.html", "w");
	if (f == NULL)
	{
		char* str = strerror(GetLastError());
		LogError(str);

		printf("Error opening the file");

		return;
	}

	sprintf(htmlHomePage, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"/><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>ProcessSnapShot_Project-Tamara</title><link rel=\"stylesheet\" href=\"../My Website/CSS/style1.css\"/></head><body><header><nav><div><span class =\"nav-span\"><div>Dll Count: %d</div><div>Processes Count: %d</div><div>Memory Avg: %llu</div></span></div><ul class=\"nav-ul\"><li><button><img src=\"../My Website/images/home.svg\"/><a href=\"../My Website/homePage.html\">HOME</a></button></li><li><button><img src=\"../My Website/images/girl.svg\"/><a href=\"../My Website/about.html\">ABOUT ME</a></button></li></ul></nav></header><main><div class=\"left-main\"><div class=\"title\"><h3>SAMPLES LIST</h3></div><div><table class=\"left-table\"><thead><tr><th>ID</th><th>Link</th><th>Process Count</th><th>Dll Count</th><th>Menory Avg</th></tr></thead><tbody>", numOfDll, numOfProcesses, memoryAvgInSnapShots);
	
	fputs(htmlHomePage, f);

	while (currentSnapShot != NULL)
	{
		sprintf(htmlHomePage, "<tr><td>%d</td><td class=\"td-link\"><a href=\"../My Website/SampleTablePage%d.html\">Sample%d.html</a></td><td>%d</td><td>%d</td><td>%llu</td></tr>", currentSnapShot->sampleID, currentSnapShot->sampleID, currentSnapShot->sampleID, currentSnapShot->processCount, currentSnapShot->DllCountInSnap, MemoryAverageInSnapShot(currentSnapShot));

		fputs(htmlHomePage, f);

		currentSnapShot = currentSnapShot->next;
	}

	sprintf(htmlHomePage, "</tbody></table></div></div><div class=\"right-main\"><div class=\"title\"><h3>DLL\'S LIST</h3></div><div><table class=\"right-table\"><thead><tr><th class=\"dll-name\">Dll Name</th><th>Link</th></tr></thead><tbody>");
	fputs(htmlHomePage, f);

	while (currentDllDictionarySnap != NULL)
	{
		sprintf(htmlHomePage, "<tr><td>%s</td><td class=\"td-link\"><a href=\"../My Website/DllTablePage%d.html\">DLL%d.html</a></td></tr>", currentDllDictionarySnap->keyDLL, dllLineTableCounter, dllLineTableCounter);

		dllLineTableCounter++;
		fputs(htmlHomePage, f);
		
		currentDllDictionarySnap = currentDllDictionarySnap->next;
	}

	sprintf(htmlHomePage, "</tbody></table></div></div></main></body></html>");
	fputs(htmlHomePage, f);
	fclose(f);
}



void SampleTablePageHtml()
{
	char htmlSampleTablePage[10000];
	char fileName[100];

	struct SnapShot* currentfileSnap = HeadS;
	struct Process* currentProcess = currentfileSnap->process;
	struct Dll* currentDll = currentProcess->dll;

	int trNum = 0;

	while (currentfileSnap != NULL)
	{
		sprintf(fileName, "My Website\\SampleTablePage%d.html", currentfileSnap->sampleID);
		FILE* f = fopen(fileName, "w");
		if (f == NULL)
		{
			char* str = strerror(GetLastError());
			LogError(str);

			printf("Error opening the file");

			return;
		}
	
		SIZE_T processNumTr = ProcessWithHighestWorkingSetSize(currentfileSnap);
		
		sprintf(htmlSampleTablePage, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"/><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>ProcessSnapShot_Project-Tamara</title><link rel=\"stylesheet\" href=\"../My Website/CSS/style2.css\"/></head><body><header><nav><ul class=\"nav-ul\"><li><button><img src=\"../My Website/images/home.svg\"/><a href=\"../My Website/homePage.html\">HOME</a></button></li><li><button><img src=\"../My Website/images/girl.svg\"/><a href=\"../My Website/about.html\">ABOUT ME</a></button></li></ul></nav></header><main><div class=\"top-title\">");
		fputs(htmlSampleTablePage, f);


		sprintf(htmlSampleTablePage, "<h2>SAMPLES LIST NUMBER:%d AT %s</h2>", currentfileSnap->sampleID, currentfileSnap->sampleTime);
		fputs(htmlSampleTablePage, f);

		sprintf(htmlSampleTablePage, "</div><div class=\"table-title\"><h3>PROCESSES LIST</h3></div><div class=\"div-table\"><table class=\"table\"><thead><tr><th>Process Name</th><th>Process ID</th><th>PageFaultCount</th><th>WorkingSetSize</th><th>QuotaPagedPoolUsage</th><th>QuotaPeakPagedPoolUsage</th><th>PagefileUsage</th><th>Dll Count</th><th>Process\'s Dll List</th></tr></thead><tbody>");
		fputs(htmlSampleTablePage, f);

		currentProcess = currentfileSnap->process;;
		while (currentProcess != NULL)
		{
			trNum++;

			if (trNum == processNumTr)
			{
				sprintf(htmlSampleTablePage, "<tr><td>%s</td><td>%lu</td><td>%lu</td><td>%llu<script src=\"https://cdn.lordicon.com/pzdvqjsp.js\"></script><div class=\"warning-icon\"><lord-icon src=\"https://cdn.lordicon.com/wdqztrtx.json\"trigger=\"loop\"colors=\"primary:#f00000\"state=\"hover\"style=\"width: 30px; height: 30px\"></lord-icon></div></td><td>%llu</td><td>%llu</td><td>%llu</td><td>%d</td><td><select><option selected>Dll\'s List</option>", currentProcess->processName, currentProcess->processID, currentProcess->memoryInfo.PageFaultCount, currentProcess->memoryInfo.WorkingSetSize, currentProcess->memoryInfo.QuotaPagedPoolUsage, currentProcess->memoryInfo.QuotaPeakPagedPoolUsage, currentProcess->memoryInfo.PagefileUsage, currentProcess->DLLCount);
				fputs(htmlSampleTablePage, f);
			}
			else
			{
				sprintf(htmlSampleTablePage, "<tr><td>%s</td><td>%lu</td><td>%lu</td><td>%llu</td><td>%llu</td><td>%llu</td><td>%llu</td><td>%d</td><td><select><option selected>Dll\'s List</option>", currentProcess->processName, currentProcess->processID, currentProcess->memoryInfo.PageFaultCount, currentProcess->memoryInfo.WorkingSetSize, currentProcess->memoryInfo.QuotaPagedPoolUsage, currentProcess->memoryInfo.QuotaPeakPagedPoolUsage, currentProcess->memoryInfo.PagefileUsage, currentProcess->DLLCount);
				fputs(htmlSampleTablePage, f);
			}
			
			currentDll = currentProcess->dll;
			while (currentDll != NULL)
			{
				sprintf(htmlSampleTablePage, "<option>%s</option>", currentDll->DLLName);
				fputs(htmlSampleTablePage, f);

				currentDll = currentDll->next;
			}

			sprintf(htmlSampleTablePage, "</select></td></tr>");
			fputs(htmlSampleTablePage, f);


			currentProcess = currentProcess->next;
		}

		sprintf(htmlSampleTablePage, "</tbody></table></div></main></body></html>");
		fputs(htmlSampleTablePage, f);

		fclose(f);

		trNum = 0;
		currentfileSnap = currentfileSnap->next;
	}
}



void DllTablePageHtml()
{
	char htmlDllTablePage[10000];
	char fileName[100];

	struct DLL_Dictionary* currentDllDictionarySnap = HeadD_Dictionary;
	int dllLineTableCounter = 1;

	while (currentDllDictionarySnap != NULL)
	{
		sprintf(fileName, "My Website\\DllTablePage%d.html", dllLineTableCounter);
		FILE* f = fopen(fileName, "w");
		if (f == NULL)
		{
			char* str = strerror(GetLastError());
			LogError(str);

			printf("Error opening the file");

			return;
		}

		sprintf(htmlDllTablePage, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"/><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>ProcessSnapShot_Project-Tamara</title><link rel=\"stylesheet\" href=\"../My Website/CSS/style3.css\"/></head><body><header><nav><ul class=\"nav-ul\"><li><button><img src=\"../My Website/images/home.svg\"/><a href=\"../My Website/homePage.html\">HOME</a></button></li><li><button><img src=\"../My Website/images/girl.svg\"/><a href=\"../My Website/about.html\">ABOUT ME</a></button></li></ul></nav></header><main>	<div class=\"title\">");
		fputs(htmlDllTablePage, f);

		sprintf(htmlDllTablePage, "<h2>%d PROCESSES USED %s</h2>", currentDllDictionarySnap->processCount, currentDllDictionarySnap->keyDLL);
		fputs(htmlDllTablePage, f);

		sprintf(htmlDllTablePage, "</div><div><table class=\"table\"><thead><tr><th>Process Name</th><th>Process ID</th></tr></thead><tbody>");
		fputs(htmlDllTablePage, f);

		while (currentDllDictionarySnap->processDictionary != NULL)
		{
			sprintf(htmlDllTablePage, "<tr><td>%s</td><td>%lu</td></tr>", currentDllDictionarySnap->processDictionary->processName, currentDllDictionarySnap->processDictionary->processID);
			fputs(htmlDllTablePage, f);

			currentDllDictionarySnap->processDictionary = currentDllDictionarySnap->processDictionary->next;
		}

		sprintf(htmlDllTablePage, "</tbody></table></div></main></body></html>");
		fputs(htmlDllTablePage, f);

		fclose(f);

		dllLineTableCounter++;
		currentDllDictionarySnap = currentDllDictionarySnap->next;
	}
}


