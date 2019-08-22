#include"shellEmu.h"

ShellEmu::ShellEmu()
{
    m_cwd = &m_root;

    m_readyToExit = false;
}

string ShellEmu::getStartMsg()
{
    string ret;

    ret = "Welcome to the Shell Emulator!\n"  
          "Supported commands:\n" 
          "ls <-l>\n\tList contents of current directory (-l provides attribute details of contents)\n\n" 
          "cd DIRECTORY_NAME\n\tChange current working directory\n\n" 
          "pwd\n\tPrint the full path to the current working directory\n\n"
          "mkdir DIRECTORY_NAME\n\tMake a new directory in the current working directory\n\n" 
          "rmdir DIRECTORY_NAME\n\tRemove a directory (and all its contents) from the current working directory\n\n" 
          "rm FILE_NAME\n\tRemove a file from the current working directory\n\n"
          "chmod NUMERIC_PERMISSION_STRING FILE_OR_DIRECTORY_NAME\n\tChange the permissions on a file or directory in the current working directory\n\n"
          "touch FILE_NAME\n\tIf a file exists in the current working directory with the indicated name, update it's modification time, otherwise create it\n\n"
          "touch DIR_NAME\n\tUpdate the modification time of a directory in the current working directory\n\n"
          "exit\n\tQuit the shell emulator\n\n"
          "quit\n\tQuit the shell emulator\n\n";
    
    return ret;
}

string ShellEmu::getPrompt()
{
    return getCWDString() + "  >> ";
}

void ShellEmu::doCommand(const string& cmd)
{
    vector<string> pCmd;

    //break up the command, using spaces as the delimeter
    parseCmd(cmd, pCmd);

    //sanity check
    if(pCmd.size() == 0)
    {
        return;
    }

    //call the appropriate command function based on the root command
    if(pCmd[0] == "ls")
    {
        do_ls(pCmd);
    }
    else if(pCmd[0] == "cd")
    {
        do_cd(pCmd);
    }
    else if(pCmd[0] == "pwd")
    {
        cout << "\n" << getCWDString() << "\n\n";
    }
    else if(pCmd[0] == "mkdir")
    {
        do_mkdir(pCmd);
    }
    else if(pCmd[0] == "rmdir")
    {
        do_rmdir(pCmd);
    }
    else if(pCmd[0] == "rm")
    {
        do_rm(pCmd);
    }
    else if(pCmd[0] == "chmod")
    {
        do_chmod(pCmd);
    }
    else if(pCmd[0] == "touch")
    {
        do_touch(pCmd);
    }
    else if(pCmd[0] == "quit" || pCmd[0] == "exit")
    {
        m_readyToExit = true;
    }
    else
    {
        EmuError er;
        er.msg = "Unrecogonized command (\"" + pCmd[0] + "\")";

        throw(er);
    }
}

void ShellEmu::parseCmd(string cmd, vector<string>& pCmd)
{
    string temp;
    int start, end, len = cmd.length();
    bool done;

    //find where the command starts (i.e., jump over leading spaces)
    start = 0;
    done = false;
    while(!done && start != len)
    {
        if(cmd[start] == ' ')
        {
            ++start;
        }
        else
        {
            done = true;
        }
    }

    //if our command starts at the end of the string, we are done
    if(start == len)
    {
        return;
    }

    //now go over the command and find where the next space is (i.e., find how many characters are in it)
    end = cmd.find(' ', start);
    if(end == string::npos)   //the find function returns string::npos (a constant error value) if it doesnt find the character
    {
        end = len;
    }

    //extract the command substr and add it to our parsed commands vector
    temp = cmd.substr(start, end-start);
    pCmd.push_back(temp);

    //if there is any more left to parse
    if(end != len)
    {
        //get the rest of the string
        temp = cmd.substr(end+1);

        //and do this all over again
        parseCmd(temp, pCmd);
    }
}

string ShellEmu::getCWDString()
{
    //use our recursive helper to walk up the tree and make our string
    return getCWDString_helper(m_cwd) + "/";
}

string ShellEmu::getCWDString_helper(Dir* d)
{
    //if the argument directory is the root, we cant go up any more, just return the string
    if(d->parentDir == NULL)
    {
        return d->attr.name;
    }
    else
    {
        //otherwise, keep recursuing up the tree
        return getCWDString_helper(d->parentDir) + "/" + d->attr.name;
    }
}

void ShellEmu::do_ls(const vector<string>& pCmd)
{
    //if this is basic ls
    if(pCmd.size() == 1)
    {
        //walk through the directories and files, outputting their names
        cout << "\n";
        for(int i = 0, i_end = m_cwd->subDir.size(); i < i_end; ++i)
        {
            cout << m_cwd->subDir[i].attr.name << "\t";
        }
        cout << "\n";
        for(int i = 0, i_end = m_cwd->files.size(); i < i_end; ++i)
        {
            cout << m_cwd->files[i].attr.name << "\t";
        }
        cout << "\n\n";
    }
    else
    {
        //check the options
        if(pCmd[1][0] == '-')
        {
            //if we wanted to support compound options (i.e., multiple options in one string, we would do a loop here)
            //for now lets just assume a single option character per '-'
            if(pCmd[1][1] == 'l')
            {
                //walk through the directories and files, outputting the long listing for them
                cout << "\n\n";
                for(int i = 0, i_end = m_cwd->subDir.size(); i < i_end; ++i)
                {
                    cout << getLongAttrString(m_cwd->subDir[i].attr) << "\n";
                }
                for(int i = 0, i_end = m_cwd->files.size(); i < i_end; ++i)
                {
                    cout << getLongAttrString(m_cwd->files[i].attr) << "\n";
                }
                cout << "\n";
            }
            //else if(some other option char)
            //{
            //
            //}
            else
            {
                EmuError er;
                er.msg = "Unrecognized option provided for ls command (\"" + pCmd[1][1];
                er.msg += "\")";

                throw(er);
            }
        }
        //improperly formatted option, tell the user about it
        else
        {
            EmuError er;
            er.msg = "Invalid option format provided to ls command (\"" + pCmd[1] + "\")";

            throw(er);
        }
    }
}

void ShellEmu::do_cd(const vector<string>& pCmd)
{
    //error check
    if(pCmd.size() == 1)
    {
        EmuError er;
        er.msg = "Must provide target directory name for a cd command";

        throw(er);
    }

    bool found = false;

    //special case: moving up a directory
    if(pCmd[1] == "..")
    {
        //simply move up the parent pointer as long as we arent at root
        if(m_cwd->parentDir != NULL)
        {
            m_cwd = m_cwd->parentDir;
        }
    }
    //special case: moving home
    else if(pCmd[1] == "~")
    {
        //set our pointer to our root
        m_cwd = &m_root;
    }
    //basic cd
    else
    {
        //look through the directories, trying to find the one indicated
        for(int i = 0, i_end = m_cwd->subDir.size(); i < i_end && !found; ++i)
        {
            //if it is found, update cwd and set our flag so we leave the loop and know we found it
            if(m_cwd->subDir[i].attr.name == pCmd[1])
            {
                m_cwd = &m_cwd->subDir[i];
                found = true;
            }
        }

        //if we did not find the directory indicated, let the user know
        if(!found)
        {
            EmuError er;
            er.msg = "Could not change to indicated directory, no such directory found (\"" + pCmd[1] + "\")";

            throw(er);
        }
    }
}

void ShellEmu::do_mkdir(const vector<string>& pCmd)
{
    //error check: no directory name
    if(pCmd.size() == 1)
    {
        EmuError er;
        er.msg = "Must provide directory name to create a new directory";

        throw(er);
    }
    //error check: there is already a directory with the name indicated
    for(int i = 0, i_end = m_cwd->subDir.size(); i < i_end; ++i)
    {
        if(pCmd[1] == m_cwd->subDir[i].attr.name)
        {
            EmuError er;
            er.msg = "New directory name must be unique, current working directory already contains a directory named \"" + pCmd[1] + "\"";

            throw(er);
        }
    }
    //error check: there is already a file with the name indicated (may not be an error, but lets treat is as one for now)
    for(int i = 0, i_end = m_cwd->files.size(); i < i_end; ++i)
    {
        if(pCmd[1] == m_cwd->files[i].attr.name)
        {
            EmuError er;
            er.msg = "New directory name must be unique, current working directory already contains a file named \"" + pCmd[1] + "\"";

            throw(er);
        }
    }

    //make the new directory, set its name and parent pointer, then add it to the sub directories in the cwd
    Dir newDir;
    newDir.attr.name = pCmd[1];
    newDir.parentDir = m_cwd;

    m_cwd->subDir.push_back(newDir);
}

void ShellEmu::do_rmdir(const vector<string>& pCmd)
{
    //error check
    if(pCmd.size() == 1)
    {
        EmuError er;
        er.msg = "Must provide directory name for the directory to remove";

        throw(er);
    }

    //try to find the directory that we are going to remove
    int targetDir = -1;
    for(int i = 0, i_end = m_cwd->subDir.size(); i < i_end && targetDir == -1; ++i)
    {
        if(pCmd[1] == m_cwd->subDir[i].attr.name)
        {
            targetDir = i;
        }
    }
    
    //if we do not find it, let the user know
    if(targetDir == -1)
    {
        EmuError er;
        er.msg = "Could not find directory indicated for removal (\"" + pCmd[1] + "\")";

        throw(er);
    }

    //erase the directory from our sub directory vector
    m_cwd->subDir.erase(m_cwd->subDir.begin() + targetDir);
}

void ShellEmu::do_rm(const vector<string>& pCmd)
{
    //error check
    if(pCmd.size() == 1)
    {
        EmuError er;
        er.msg = "Must provide file name for the file to remove";

        throw(er);
    }

    //try to find the file to remove
    int targetFile = -1;
    for(int i = 0, i_end = m_cwd->files.size(); i < i_end && targetFile == -1; ++i)
    {
        if(pCmd[1] == m_cwd->files[i].attr.name)
        {
            targetFile = i;
        }
    }
    
    //if we do not find the file, let the user know
    if(targetFile == -1)
    {
        EmuError er;
        er.msg = "Could not find file indicated for removal (\"" + pCmd[1] + "\")";

        throw(er);
    }

    //erase the file from our file vector
    m_cwd->files.erase(m_cwd->files.begin() + targetFile);

}

void ShellEmu::do_chmod(const vector<string>& pCmd)
{
    //error check
    if(pCmd.size() != 3)
    {
        EmuError er;
        er.msg = "Must provide three digit permission string and file/directory name to chmod command";

        throw(er);
    }

    Attributes* target = NULL;

    //find the file/directory that we are updating. As soon as target is not NULL, both of these
    //loops fall through
    for(int i = 0, i_end = m_cwd->subDir.size(); i < i_end && target == NULL; ++i)
    {
        if(pCmd[2] == m_cwd->subDir[i].attr.name)
        {
            target = &m_cwd->subDir[i].attr;
        }
    }
    for(int i = 0, i_end = m_cwd->files.size(); i < i_end && target == NULL; ++i)
    {
        if(pCmd[2] == m_cwd->files[i].attr.name)
        {
            target = &m_cwd->files[i].attr;
        }
    }

    //if we could not a file or directory with the name indicated, let the user know
    if(target == NULL)
    {
        EmuError er;
        er.msg = "Could not find file indicated in chmod command (\"" + pCmd[2] + "\")";

        throw(er);
    }

    //update the target's permissions
    target->perm.updatePerm(pCmd[1]);
}

void ShellEmu::do_touch(const vector<string>& pCmd)
{
    //error check
    if(pCmd.size() < 2)
    {
        EmuError er;
        er.msg = "Must provide file/directory name for touch command";

        throw(er);
    }

    Attributes* target = NULL;

    //see if there is a file or directory with the name indicated. once target is not NULL, both
    //of these loops will fall through
    for(int i = 0, i_end = m_cwd->subDir.size(); i < i_end && target == NULL; ++i)
    {
        if(pCmd[1] == m_cwd->subDir[i].attr.name)
        {
            target = &m_cwd->subDir[i].attr;
        }
    }
    for(int i = 0, i_end = m_cwd->files.size(); i < i_end && target == NULL; ++i)
    {
        if(pCmd[1] == m_cwd->files[i].attr.name)
        {
            target = &m_cwd->files[i].attr;
        }
    }

    //if we found an object with the indicated name, update its modification time
    if(target != NULL)
    {
        time(&target->modTime);
    }
    //otherwise, create a new file and add it to our file vector
    else
    {
        File newFile;
        newFile.attr.name = pCmd[1];

        m_cwd->files.push_back(newFile);
    }
}