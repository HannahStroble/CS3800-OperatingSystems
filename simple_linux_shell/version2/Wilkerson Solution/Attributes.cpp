#include "Attributes.h"

Attributes& Attributes::operator=(const Attributes& a)
{
    this->modTime = a.modTime;
    this->name = a.name;
    this->owner = a.owner;
    this->ownerGroup = a.ownerGroup;
    this->perm = a.perm;

    return *this;
}

string getLongAttrString(const Attributes& a)
{
    string ret;

    //if the attributes are from a directory object, prefix the permissions string with a 'd'.
    //use a '-' otherwise
    if(a.isDir)
    {
        ret = "d";
    }
    else
    {
        ret = "-";
    }
    ret += a.perm.getPermString() + "   ";

    ret += a.ownerGroup + "   " + a.owner + "   ";

    //get the time string. asctime appends an endline to the string, so lets overwrite that with a space
    ret += asctime(localtime(&a.modTime));
    ret[ret.length()-1] = ' ';
    ret += "  ";

    ret += a.name;

    return ret;
}