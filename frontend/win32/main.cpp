#include <iostream>

#include "Win32Server.h"

/*
 *
 */
int main()
{
    
    XWin32::Win32Server * xserver = new XWin32::Win32Server();
    if(xserver->Init(0,0) != 0) {
        MessageBoxW(0, L"Could not initalize the server", 0, MB_OK);
        return 0;
    }
    xserver->Run();
    xserver->Cleanup();
    delete xserver;
    return 0;
}