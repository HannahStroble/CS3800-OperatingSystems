/*  Author: Josh Wilkerson
 *  Date: Sept. 9, 2018
 *  Brief: This is a basic shell emulation program, allowing the creation of a virtual file/directory structure
 *         and interaction with basic attributes of files/directories.
 */ 


#include<iostream>

#include "shellEmu.h"
#include "EmuError.h"

using namespace std;

int main()
{
    ShellEmu emu;
    bool fatalError = false;
    string cmd;

    cout << "\n======================================================================================\n" 
         << emu.getStartMsg()
         << "\n======================================================================================\n\n";

    do
    {
        try
        {
            cout << emu.getPrompt();
            getline(cin, cmd);

            emu.doCommand(cmd);
        }
        catch(EmuError er)
        {
            cout << "\nERROR:\n" << er.msg << "\n\n";
            fatalError = er.isFatal;
        }
    }while(!fatalError && !emu.readyToExit());

    return 0;
}