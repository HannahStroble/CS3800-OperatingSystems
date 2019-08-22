#ifndef EMU_ERROR_H
#define EMU_ERROR_H

#include <string>

using namespace std;

//a basic error object for this project
struct EmuError
{
    EmuError() : isFatal(false) {}

    string msg;
    bool isFatal;
};

#endif