#ifndef SHELL_EMU_H
#define SHELL_EMU_H

#include<iostream>
#include<string>
#include<vector>
#include<ctime>

#include "Dir.h"

using namespace std;

class ShellEmu
{
    public:
        ShellEmu();

        string getStartMsg();
        string getPrompt();

        void doCommand(const string& cmd);

        bool readyToExit() {return m_readyToExit;}


    // Data
    private:
        Dir m_root;
        Dir* m_cwd;

        bool m_readyToExit;


    // Functionality
    private:
        void parseCmd(string cmd, vector<string>& pCmd);

        string getCWDString();
        string getCWDString_helper(Dir* d);

        //command functions
        void do_ls(const vector<string>& pCmd);
        void do_cd(const vector<string>& pCmd);
        void do_mkdir(const vector<string>& pCmd);
        void do_rmdir(const vector<string>& pCmd);
        void do_rm(const vector<string>& pCmd);
        void do_chmod(const vector<string>& pCmd);
        void do_touch(const vector<string>& pCmd);
};

#endif