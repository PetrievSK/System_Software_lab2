#include <iostream>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

using namespace std;

#define BUF_SIZE 256
TCHAR* szName=new TCHAR[256];
TCHAR* szMsg=new TCHAR[256];

int _tmain()
{
    using namespace std;

    string szNameDir;

    std::cout<<"Enter shared memory name:   ";
    std::cin>>szNameDir;

    strcpy(szName,("Global\\"+szNameDir).c_str());

    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,
                   FALSE,
                   szName);

    if ((hMapFile == NULL) && (GetLastError() == 2))
    {
        hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,
                 NULL,
                 PAGE_READWRITE,
                 0,
                 BUF_SIZE,
                 szName);
    }
    if (hMapFile == NULL)
    {
       _tprintf(TEXT("Could not create file mapping object (%d).\n"),
             GetLastError());
      return 1;
    }

    pBuf = (LPTSTR) MapViewOfFile(hMapFile,
                        FILE_MAP_ALL_ACCESS,
                        0,
                        0,
                        BUF_SIZE);

   if (pBuf == NULL)
   {
      _tprintf(TEXT("Could not map view of file (%d).\n"),
             GetLastError());

       CloseHandle(hMapFile);

      return 1;
   }

   string status;

   while (1)
    {
        std::cout<<"\nPlease choose an action to do:\n\n"
        <<"R>ead message from shared memory;\n"
        <<"W>rite message to shared memory;\n"
        <<"Q>uit.\n\n";
        std::cin>>status;

        if (status=="R")
        {
            std::cout<<"The message says:  "<<pBuf<<std::endl;
        }
        else {
            if (status=="W")
                {
                    memset((void*)pBuf,0,BUF_SIZE);
                    std::cout<<"Enter message text:  ";
                    std::cin >> szMsg;
                    CopyMemory((PVOID)pBuf, szMsg, (_tcslen(szMsg) * sizeof(TCHAR)));
                }
            else {
                if (status=="Q"){
                        exit(1);
                }
                else if ((status!="R")&&(status!="W")&&(status!="Q"))
                    { std::cout<<"Please enter R, W or Q <in any case>\n\n";
                    }}}
        }

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}
