#ifndef SHELL_EMU_FILE_H
#define SHELL_EMU_FILE_H

#include "Attributes.h"

//data structure to manage all info about files
struct File
{
    File()
    {
        attr.isDir = false;
    }
    ~File() {}

    //admittedly, having a single member is a little silly. however, if/when file objects grow
    //to have more substance, we will fill out this class more.

    Attributes attr;
};

#endif