#ifndef DIR_H
#define DIR_H

#include <string>
#include <vector>

#include "Permissions.h"
#include "File.h"
#include "Attributes.h"

using namespace std;

//a data structure to manage directories
struct Dir
{
    Dir() 
    {
        attr.isDir = true;
    };
    ~Dir() {};

    Dir* parentDir;
    vector<Dir> subDir;

    vector<File> files;

    Attributes attr;

    //since these will be used in std::vector's it is good to provide an assignment operator
    //(sometimes vectors get cranky about the automatic ones)
    Dir& operator=(const Dir& d2);
};

#endif