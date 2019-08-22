#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include<string>
#include<ctime>

#include"Permissions.h"

using namespace std;

//a data struct to manage the information that is commonly stored for both files and directories
struct Attributes
{
    //initialize all attributes at construction
    Attributes() : ownerGroup("root_group"), owner("root")
    {
        time(&modTime);
        perm.updatePerm("007");
        isDir = false;
    }
    ~Attributes() {}

    string name;
    Permissions perm;
    bool isDir;
    time_t modTime;
    string ownerGroup;
    string owner;

    //since these will be used in std::vector's it is good to provide an assignment operator
    //(sometimes vectors get cranky about the automatic ones)
    Attributes& operator=(const Attributes& a);
};

//a helper function to get the long listing information from an Attribute set
string getLongAttrString(const Attributes& a);

#endif