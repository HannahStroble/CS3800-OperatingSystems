#include "Permissions.h"

Permissions::Permissions() : STRING_ORDER{ALL, GROUP, OWNER}, PERM_STRING{"r", "w", "x"}
{
    //initialize the permissions to all be false
    for(int m = 0; m < NUM_MODES; ++m)
    {
        for(int p = 0; p < NUM_PERM; ++p)
        {
            m_perm[m][p] = 0;
        }
    }

    //set up our bit masks for the permissions. These are just integers that are being expressed a different
    //way, specifically in binary. Writing a number starting with 0b tells the compiler you are getting ready
    //to give it a binary number. So 0b11001 will be binary number 11001, which will set the int to 25.
    m_mask[READ] = 0b100;
    m_mask[WRITE] = 0b010;
    m_mask[EXE] = 0b001;
}

string Permissions::getPermString() const
{
    string ret;

    //walk through our permissions, outputting either the associated string (if true) or a '-' if false
    for(int m = 0; m < NUM_MODES; ++m)
    {
        for(int p = 0; p < NUM_PERM; ++p)
        {
            if(m_perm[STRING_ORDER[m]][p])
            {
                ret += PERM_STRING[p];
            }
            else
            {
                ret += "-";
            }
        }
    }

    return ret;
}

void Permissions::updatePerm(const string& numStr)
{
    //error check
    if(numStr.length() != NUM_MODES)
    {
        EmuError er;
        er.msg = "Invalid permission string provided (\"" + numStr + "\")";

        throw(er);
    }

    string digitString;
    int digit;

    //our string is 3 digits, one for each permission group, look at each digit
    for(int m = 0; m < NUM_MODES; ++m)
    {
        //error check
        if(!validPermissionDigit(numStr[m]))
        {
            EmuError er;
            er.msg = "Invalid numeric permission provided, all digits in numeric permissions must be numbers between 0 and 7";
        }

        //get the numeric version of the digit
        digitString = numStr[m];
        digit = stoi(digitString);

        //use our bit masks and bitwise and's to tell if we should set the permission or not
        for(int p = 0; p < NUM_PERM; ++p)
        {
            if(digit & m_mask[p])
            {
                m_perm[m][p] = true;
            }
            else
            {
                m_perm[m][p] = false;
            }
        }
    }
}

bool Permissions::validPermissionDigit(const char d)
{
    //make sure our permission characters are what they are supposed to be. this may seem a bit cumbersome
    //(and there may be a better way), but this works for such a limited set of cases.
    switch(d)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
            return true;
    }
    return false;
}

Permissions& Permissions::operator=(const Permissions& permObj)
{
    for(int m = 0; m < NUM_MODES; ++m)
    {
        for(int p = 0; p < NUM_PERM; ++p)
        {
            this->m_perm[m][p] = permObj.m_perm[m][p];
        }
    }

    return *this;
}