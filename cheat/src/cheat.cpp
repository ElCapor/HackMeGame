/*
Write all the cheat code here
*/

#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <tlhelp32.h>

using namespace std;

DWORD GetProcesByName(const char* pName)
 
{
	DWORD pID = 0;
	HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pInfo;
	pInfo.dwSize = sizeof(PROCESSENTRY32);
 
	if (Process32First(snapShot, &pInfo))
	{
		while (Process32Next(snapShot, &pInfo))
		{
			if (_stricmp(pName, pInfo.szExeFile) == 0)
			{
				pID = pInfo.th32ProcessID;
				CloseHandle(snapShot);
				return pID;
			}
		}
	}
	CloseHandle(snapShot);
	return 0;
}

int main()
{

    DWORD ProcessID = GetProcesByName("game.exe");
    std::cout << "ProcessID:" << ProcessID << std::endl;
    if (ProcessID){
        HANDLE Game = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);

        std::cout << "Game:" << Game << std::endl;

        while (true){
            int idk;

            ReadProcessMemory(Game, (LPCVOID)0x29B8FBCCAC0, &idk, sizeof(int), 0);

            std::cout << idk << std::endl;
            Sleep(1000);
        }

    }
    else
    {
        std::cout << "Process not Found!" << std::endl;
    }

    system("pause");
    return 0;
}
