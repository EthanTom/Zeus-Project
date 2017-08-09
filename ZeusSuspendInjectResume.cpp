/* 
* ZeusSuspendInjectResume.cpp : Holds the 
* mechanism required for injecting a dll. Uses
* an assembly shellcode to inject a dll into a process
* and then jump back to the beginning (Old EIP)
*
* Written By : @Ice3man
* Email : EthanTom543@gmail.com
*
* (C) Ice3man
*/

#include "Zeus.h"

/* 
* ZeusSuspendInjectResume: Injects A Dll by stopping
* the remote process and then changing the EIP to point
* to a shellcode written in memory which in turn loads
* a dll.
*
* @Arguments : 
*	@ProcID : Process ID to inject
*	@szDllLoc : Location of the Dll TO Inject
*
* @Return :
*	@Void
*/

unsigned char ShellCode[] =
{

};

int ZeusSuspendInjectResume(int ProcID, char *szDllLoc)
{

}
