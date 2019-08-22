#include "Dir.h"

Dir& Dir::operator=(const Dir& d)
{
    this->parentDir = d.parentDir;
    this->attr = d.attr;
    this->files = d.files;
    this->subDir = d.subDir;

    return *this;
}