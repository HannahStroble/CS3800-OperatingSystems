#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include<vector>
#include<string>

#include "EmuError.h"

using namespace std;

//object to manage file and directory permissions for us
class Permissions
{
    public:
        Permissions();

        //update the permissions for an object
        void updatePerm(const string& numStr);

        //output the long listing for the permission string
        string getPermString() const;

        //since these will be used in std::vector's it is good to provide an assignment operator
        //(sometimes vectors get cranky about the automatic ones)
        Permissions& operator=(const Permissions& permObj);

    private:
        bool validPermissionDigit(const char d);


        //constants to help make our code seem less magical and generally more robust
        //constants for the permission group indices
        enum ModeIdx
        {
            ALL = 0,
            GROUP = 1,
            OWNER,
            NUM_MODES
        };
        //constants for the permission indices
        enum PermIdx
        {
            READ = 0,
            WRITE = 1,
            EXE,
            NUM_PERM
        };

        //strings associated with each permission (for the long listing output)
        const string PERM_STRING[NUM_PERM];

        //the order permission groups should be displayed
        const ModeIdx STRING_ORDER[NUM_MODES];
        
        //the permissions themselves
        bool m_perm[NUM_MODES][NUM_PERM];

        //bitmasks used to set the permissions (ideally these would be static const members, but lets keep
        //it simple for now)
        int m_mask[NUM_PERM];
};

#endif