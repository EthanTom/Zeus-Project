/*
* Zeus Project : An advance injection toolkit
* Written By : @Ice3man
* Email : EthanTom543@gmail.com
*
*
*/

#include "Zeus.h"

/*
* NTDLL API Library For Native function access
*/
#pragma comment(lib, "ntdll.lib")

/*
* Prints the help if not command line is
* specified.
*/
void DisplayHelp(char *procName)
{
	printf("\n");
	printf("\nGeneral : ");
	printf("\n\t-d : Dumps the processes running");
	printf("\n\t-p <PID> : Process ID To Inject Into");
	printf("\n\t-pn <Process Name> : Name of the process to inject");
	printf("\n\t-sD : Enable SeDebugPrivileges (Need Admin)");
	printf("\n\t-l <DLL Location> : Injects the Dll Specified");
	printf("\n\t-h : Display Help");

	printf("\n\nInjection Techniques :");
	printf("\n\t-c : 1) CreateRemoteThread Technique");
	printf("\n\t-r : 2) RtlCreateUserThread Technique");
	printf("\n\t-n : 3) NtCreateThreadEx Technique");
	printf("\n\t-s : 4) SetWindowsHookEx Technique (Dll Must Export 'Proc' Func)");
	printf("\n\t-q : 5) QueueUserAPC Technique");

	printf("\n");
	printf("\nUsage : ");
	printf("\n\t%s -d (To List the Running processes and get PID", procName);
	printf("\n\t%s -p 1111 -l D:\\SomeDll.dll -c ", procName);
	printf("\n");
}

int main(int argc, char *argv[])
{
	int PID;
	char szProcessName[128];
	char *szDllLoc;
	int seDebugFlag, ZMethod = 0;

	printf("\n[*] ZEUS PROJECT : An Advance Injection Toolkit");
	printf("\n[*] Written By : @Ice3man");
	printf("\n[*] Version : 0x00");
	printf("\n[*] Web : http://www.github.com/ethantom543/zeus");
	printf("\n");
	ZeusPrintInfo();

	if (argc < 2)
	{
		printf("\n\n[!] Error, Not Arguments Provided !");
		printf("\n[-] For Help, use the -h argument ");
		ExitProcess(0);
	}

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
		{
			ZeusListProcess();
			ExitProcess(0);
		}

		else if (strcmp(argv[i], "-p") == 0)
		{
			if (i+1 != argc)
			{
				PID = atoi(argv[i+1]);
				printf("\n\n[-] Targeting PID : %d", PID);
				i++;
			}
		}

		else if (strcmp(argv[i] , "-l") == 0 ) 
		{
			if(i+1 != argc)
			{
				szDllLoc = argv[i+1];
				printf("\n[-] Injecting DLL: %s",szDllLoc);
				i++;
			}
		}

		else if (strcmp(argv[i], "-h") == 0)
		{
			DisplayHelp(argv[0]);
		}

		else if (strcmp(argv[i], "-pn") == 0)
		{
			if (i+1 != argc)
			{
				strcpy_s(szProcessName, argv[i+1]);
				PID = ZeusGetProcessIdFromName(szProcessName);
				printf("\n\n[-] Targeting Process : %s\tPID Found : %d", szProcessName, PID);
				i++;
			}
		}

		else if (strcmp(argv[i], "-sD") == 0)
		{
			/*
			* Flag to check if need to elevate privileges
			*/
			seDebugFlag = 1;
		}

		else if (strcmp(argv[i], "-c") == 0)
		{
			/*
			* First Method is CreateRemoteThread
			*/
			printf("\n[-] Target Method : CreateRemoteThread !\n");
			ZMethod = 1;
		}

		else if (strcmp(argv[i], "-r") == 0)
		{
			/*
			* RtlCreateUserThread Method
			*/
			printf("\n[-] Target Method : RtlCreateUserThread !\n");
			ZMethod = 2;
		}

		else if (strcmp(argv[i], "-n") == 0)
		{
			/*
			* NtCreateThreadEx Method
			*/
			printf("\n[-] Target Method : NtCreateThreadEx !\n");
			ZMethod = 3;

		}

		else if (strcmp(argv[i], "-s") == 0)
		{
			/*
			* SetWindowsHookEx Method
			*/
			printf("\n[-] Target Method : SetWindowsHookEx !\n");
			ZMethod = 4;

		}

		else if (strcmp(argv[i], "-q") == 0)
		{
			/*
			* QueueUserAPC Method
			*/
			printf("\n[-] Target Method : QueueUserAPC !\n");
			ZMethod = 5;

		}
	}
	
	// If asked to enable debug privileges
	if (seDebugFlag == 1)
		ZeusSetDebugPrivileges();

	switch(ZMethod)
	{
		/*
		* CreateRemoteThread Method
		*/
		case 1:
			ZeusCreateRemoteThread(PID, szDllLoc);
			ExitProcess(0);

		case 2:
			ZeusRtlCreateUserThread(PID, szDllLoc);
			ExitProcess(0);

		case 3:
			ZeusNtCreateThreadEx(PID, szDllLoc);
			ExitProcess(0);

		case 4:
			ZeusSetWindowsHookEx(PID, szDllLoc);
			ExitProcess(0);

		case 5:
			ZeusQueueUserAPC(PID, szDllLoc);
			ExitProcess(0);
	}

	ExitProcess(0);
}