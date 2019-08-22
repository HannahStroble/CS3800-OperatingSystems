#include <vector>
#include <iostream>
#include "node.h"
#include <limits>
#include <sstream>
#include <map>
#include <set>
#include <string>
#include <string.h>
#include <stdio.h>
#include <bits/stdc++.h>

#ifndef COMPUTER_H
#define COMPUTER_H
namespace Shell // 
{
  const std::string CMDS[] = {"", "ls", "pwd", "exit", "mkdir", "touch", "cd", "rm", "rmdir", "chmod"};

  // Computer class 
  // Represents the OS who controls the file System.
  class Computer
  {
    // Private vars
    private:
      // The root File
      Node* rootFile;
      // The current Directory the System is looking at
      Node* curDir;
      // The current logged in user.
      User* curUser;
      // The name of this computer
      std::string computerName;
	  // List of users.
	  std::map<std::string, User*> users;
	  // list of groups
	  std::set<std::string> groups;

    // Public functions
    public:

      // The constructor taking in the computer name
      Computer(std::string name)
      {
        // set the name and call the default constuctor
        computerName = name;
        Computer();
      }
      // Deconstructor, make sure we don't have memory leaks :)
      ~Computer()
      {
		 // Delete the list of users
		 for(auto user : users)
        {
          delete user.second;
        }
		  
        delete curUser;
        // Delete the root : let its deconstructor handle deleting
        // the rest of the file system.
        delete rootFile;
        // Make sure we clear the pointers to prevent some accidental
        // pointer to mem we don't own.
        rootFile = nullptr;
        curDir = nullptr;
        curUser = nullptr;
      }
      // Main constructor - does the heavy lifting
      Computer()
      {
        // No user to start off with, need to login.
        curUser = nullptr;
        // Create the root of the file system.
        rootFile = new Node("", true, nullptr, 0, "root", "root");
        // Set the root's parent to itself - makes it auto handle ../ on root. 
        // simple hack to make my life easier down the line.
        rootFile->parent = rootFile;
        // Make the root user.
        curUser = new User("root", "root", true, "toor");
        // set the computer name.
        computerName = "computer";
        // move the current location to the root - this will change depending
        // on who logs in
        curDir = rootFile;
        // create home and root directory
        rootFile->AddChild(new Node("root", true, rootFile));
        rootFile->AddChild(new Node("home", true, rootFile));
		// add list of users
		users = std::map<std::string, User*>();
		users.emplace(curUser->Username(), curUser).second;
		// add to groups
		groups.insert("users");
      }
	  
	  
	  // Add user
	   bool AddUser(User* add_user)
      {
        bool succeed = false;
        succeed = users.emplace(add_user->Username(), add_user).second;
        if(!succeed)
          delete add_user;
        return succeed;
      }

    // find file in current directory
    bool FindFileInCurDir(std::string file)
    {
      // iterate through all the files in current directory
      for(auto object : curDir->Children())
      {
        // make sure it is not a directory
        if(object.second->isDir)
        { std::cout << object.second->Name() << " is a directory.\n"; }
        else if(object.second->Name() == file)
        {
          return true;
        }
      }
      return false;
    }

    // 
	  
	  // remove user
	  bool rmUser(std::string username)
	  {
		  if(FindUser(username))
		  {
			  users.erase(username);
			  return true;
		  }
		  return false;
	  }
	  
	  // Switch current user
	  bool SwitchUser(User* sw_user)
	  {
		  if(FindUser(sw_user->Username()))
		  {
			  curUser = sw_user;
			  return true;
		  }
		  return false;
	  };
	  
	  // Find users
	  bool FindUser(std::string user_name)
	  {
		  // find the list of users
		 for(auto user : users)
        {
           if(user.second->Username()== user_name)
		   {
			   return true;
		   }
        }
		return false;
	  };
	  
	  // get user
	  User* GetUser(std::string user_name)
	  {
		  // check if user is there
		  if(FindUser(user_name))
		  {
			auto found = users.find(user_name);
			return found->second;
		  }
		  //this needs a check
		  return curUser;
	  }

    // remove user from group
    bool RmGroupFromUser(User* user, std::string group)
    {
      if(FindUser(user->Username()))
      {
        user->RmGroup(group);
        return true;
      }
      return false;
    }

    // add user to group
    bool AddGroupToUser(User* user, std::string group)
    {
      if(FindUser(user->Username()))
      {
        user->AddGroup(group);
        return true;
      }
      return false;
    }
	  
	  // print users in list
	  void printUsers()
	  {
		  for(auto user : users)
		  {
			  std::cout << user.second->Username() << "\t";
		  }
		  std::cout << "\n";
	  }

    // return if the overall group exists
    bool FindSystemGroup(std::string name)
    {
      bool result = groups.find(name) != groups.end();
      if(result)
      { return true; }
      return false;
    }
	  
	  // Add groups to system groups
	  bool AddSystemGroups(std::string name)
	  {
		  bool result = groups.find(name) != groups.end();
		  if(!result)
		  {
			  groups.insert(name);
			  return true;
		  }
		  return false;
	  }
	  
	  // print user groups
	  bool printUserGroups(std::string username)
	  {
		  if(FindUser(username))
		  {
			  std::cout << curUser->Group() << "\n";
			  return true;
		  }
		  return false;
	  }

    // add user to existing group
    bool addUsertoExistingGroup(User* user, std::string group)
    {
      // check if group exists
      bool result = groups.find(group) != groups.end();
      if(result)
      {
  
            //if group exists then add it to the specified user
            user->AddGroup(group);
            return true; 
      }
      else { std::cout << group << " does not exist. Could not be added.\n"; }
      return false;
    }
	  
	  // add to user groups
	  bool addUserGroups(std::string username, std::string groupname)
	  {
		  if(FindUser(username))
		  {
			  curUser->AddGroup(groupname);
			  return true; 
		  }
		  return false;
	  }

    Node* FindFileObject(std::string filename)
    {
      // iterate through objects in current directory
      for(auto file : curDir->Children())
      {
        // if the file is found then return that file
        if(file.second->Name() == filename)
        {
          return file.second;
        }
      }
      // if no file was found then return a nullptr
      std::cout << "Error: File not found in current directory\n";
      return nullptr;
    }

    // find group permissions
    bool CheckGroupPerms(Node* object, char permRequest)
    {
      // check if object is not owned by the current user
      if(object->User() != curUser->Username())
      {
        // find the objects user owner
        for (auto user:users)
        {
          // check to see if user is the owner
          if(object->User() == user.second->Username())
          {
            // check to see if this user is in any of the same groups as the owner
            for(auto objGroup : object->GetGroups())
            {
              for(auto usrGroup : user.second->GetGroups())
              {
                // if the object is in any of the same groups
                if(usrGroup == objGroup)
                {
                  // check if the object has the right group permissions
                  std::string perm = object->PermsStr();

                  // if the permissions for groups are correct then continue
									if((perm[4] == permRequest) || (perm[5] == permRequest) || (perm[6] == permRequest))
									{  return true; }
                }
                // if no groups are similar then no permission is granted
                else { return false; }
              }
            }
          }
        }
      }
      //if no return statement was hit then no permission granted
      return false;
    }
	  
	  // Check permission of file
	  bool CheckFilePerms(Node* object, char permRequest)
	  {
			// check to see if the desired perms are available
			std::string perm = object->PermsStr();

			  // check if is the owner
			  if(object->User() == curUser->Username())
			  {
				  // check to see if the desired perms are available for admin
					if((perm[1] == permRequest) || (perm[2] == permRequest) || (perm[3] == permRequest))
						{  return true; }
					
				  // otherwise permission is not granted
				  else
				  { return false; }
			  }
			  // check if current user is not owner
			  else if(object->User() != curUser->Username())
			  {
          // check group permissions
				  if(CheckGroupPerms(object, permRequest))
          {
            return true;
          }
          // else check general permissions
          else
          {
            if((perm[7] == permRequest) || (perm[8] == permRequest) || (perm[9] == permRequest))
						  { return true; }
            // if not in general permissions then no permission granted
            else { return false; }
          }
			  }
        // if you got here and did not hit a return statement, no perms granted
        return false;
	  	}

      // Running the computer. Handles all operations from here.
      void run()
      {
        // Start the console.
        console();
      }
      
    // Private functions
    private:
      // Console handles taking in input and outputting correct response.
      // is what emulates running the shell.
      void console()
      {
        // control var for machine
        bool looping = true;
        // storages variables
        std::string dir;
        std::string input;
        // While we haven't quit
        while(looping)
        {
          // Gets current working directory
          dir = pwd();
          //looks for user's name in wd
          int pos = dir.find(curUser->Username());
          // if found, use ~ insead the complete path.
          if(pos > -1)
            dir = "~" + dir.substr(pos + curUser->Username().length());
          // output the status line. 
          std::cout << curUser->Username() << "@" << computerName << ":" << dir
               << "#" << " ";
          // Get input from the user.
          std::getline(std::cin, input);
          // Parse it and handle it.
          looping = parser(input);
        }
      }
      // Parses input. returns true if the console should continue.
      bool parser(std::string input)
      {
        // storage vars for easier manipulation of data.
        std::stringstream stream(input);
        std::string command;
        std::vector<std::string> args;
        // Using a string stream to quickly break up input on spaces.
        stream >> command;
        // get the rest of the input
        std::string temp;
        // store it as an argument
        while(stream >> temp)
        {
          args.push_back(temp);
        }
        // return the results of commands func
        return commands(command, args);
      }

      // attempts to run a command using the command and the args for it.
      // returns true if console should keep running.
      bool commands(std::string command, std::vector<std::string> args)
      {
        // Handles ls command
        if(command == "ls")
        {
	  // check read permissions of current directory
	  if(CheckFilePerms(curDir, 'r'))
	  {
          // if command has args
          if(args.size() > 0)
          {
            // if it has only the correct arg.
            if(args.size() == 1 && args[0] == "-l") 
            {
				// check permissions
				
              // display output
              for(auto childPair :curDir->Children())
              {
                Node* child = childPair.second;
                std::cout << child->PermsStr() << " " << child->NumDirs()
                          << " " << child->User() << " " << child->Group() << " "
                          << child->Size() << " " << child->TimeStr() << " "
                          // Adds blue color if it is a dir
                          << (child->isDir ? "\033[34m" : "")
                          << child->name << "\033[0m" << std::endl;
              }
            }
            else
            {
              // otherwise invalid useage
              std::cout << "Invalid use - For help use: help ls\n";
            }
            
          }
          // no args
          else
          {
            // display simple output
            for(auto child : curDir->Children())
            {
              std::cout << (child.second->IsDir() ? "\033[34m" : "") 
                        << child.first << "\033[0m ";
            }
            // adds a new line at the end only if there was something to print
            if(curDir->Children().size() > 0)
              std::cout << std::endl;
          }
          }
	else { std::cout << "ls: Invalid permissions\n"; }
        }
        // Handles pwd command
        else if(command == "pwd")
        {
          // outputs the current directory
          std::cout << pwd() << std::endl;
        }
        // Handles the exit command
        else if(command == "exit")
        {
          // return false to signal ending
          return false;
        }
        // Handles mkdir command
        else if(command == "mkdir")
        {
          // if there are no arguments
          if(args.size() == 0)
          {
            // error
            std::cout << "Invalid use - For help use: help mkdir\n";
          }
          // else 
          else
          {
            if(CheckFilePerms(curDir, 'w'))
            {
              // iterate over arguments
              for(std::string arg : args)
              {
                // Attempt to add new directory if fails output such a message.
                if(!curDir->AddChild(new Node(arg, true, curDir)))
                {
                  std::cout << "mkdir: cannot create directory '" << arg << "': File exits\n"; 
                }
              }
            }
            else { std::cout << "mkdir: Invalid permissions\n"; }
          }
        }
        // Handles touch command
        else if(command == "touch")
        {
          // if there are no args
          if(args.size() == 0)
          {
            // error
            std::cout << "touch: Invalid use - For help use: help touch\n";
          }
          // otherwise attempt do it
          else
          {
            if(CheckFilePerms(curDir, 'w'))
            {
              // iterate over args
              for(std::string arg : args)
              {
                // try to add and if that fails, update the current timestamp
                if(!curDir->AddChild(new Node(arg, false, curDir)))
                {
                  curDir->children[arg]->UpdateTimeStamp();
                }
              }
            }
            else { std::cout << "touch: Invalid permissions\n"; }
          }
        }
        // Handles cd command
        else if (command == "cd")
        {
          // if no args
          if(args.size() == 0)
          {
            // set current directory to root's home
            curDir = rootFile->children["root"];

            // if their directory doesn't exist anymore, put them at the root.
            if(curDir == nullptr)
              curDir = rootFile;
          }
          // if there is args and it is more than one
          else if(args.size() > 1)
          {
            // error
            std::cout << "cd: too many arguments - For help use: help cd\n";
          }
          // else if only one arg
          else
          {
            // attempt to find the file.
            Node* file = findFile(args[0]);
            // if file exists and is not a directory error
            if(file != nullptr)
              if(!file->isDir)
                std::cout << "cd: " << args[0] << " Not a directory\n";
              else
              {
                // check folder permissions
                if(CheckFilePerms(file, 'x'))
                {
                  // else set curDir to it
                  curDir = file;
                }
                else { std::cout << "cd: Invalid permissions\n"; }
              }
            // else file isn't real
            else std::cout << "cd: Directory does not exist\n";
          }
        }
        // Handles rm command
        else if (command == "rm")
        {
          // if no args
          if(args.size() == 0)
          {
            // error
            std::cout << "rm: Invalid use - For help use: help rm\n";
          }
          // else if there are args
          else
          {
              // iterate over them
              for(auto arg : args)
              {
                // try to find the arg
                Node* file = findFile(arg);
                // if it doesn't exist, error
                if(file == nullptr)
                {
                  std::cout << "rm: File '" << arg << "' not found\n";
                }
                // if file is a directory
                else if(file->isDir)
                {
                  // error
                  std::cout << "rm: cannot remove '" << arg 
                          << "': Is a directory\n";
                }
                // else is valid so delete
                else
                {
                  // check permissions of current dir and file
                  if((CheckFilePerms(file, 'w')) && (CheckFilePerms(curDir, 'w')))
                  {
                    file->parent->DeleteChild(file);
                    delete file;
                  }
                  else { std::cout << "rm: Invalid permissions for file or folder\n"; }
                }
              }
	          }
          }
        // Handles rmdir command
        else if (command == "rmdir")
        {
          // if there are no args
          if(args.size() == 0)
          {
            // error
            std::cout << "rmdir: Invalid use - For help use: help rmdir\n";
          }
          // has args
          else
          {
              // iterate over args
              for(auto arg : args)
              {
                  //find file
                  Node* file = FindFileObject(arg);
                  // if not found
                  if(file == nullptr)
                  {
                    // error
                    std::cout << "rmdir: File '" << arg << "' not found\n";
                  }
                  // if file is not a directory
                  else if(!file->isDir)
                  {
                    // error
                    std::cout << "rmdir: failed to remove '" << arg 
                          << "': Not a directory\n";
                  }
                  // if there is stuff in the file,
                  else if(file->children.size() > 0)
                  {
                    // error
                    std::cout << "rmdir: failed to remove '" << file->Name() 
                          << "': Directory not empty\n";
                  }
                  // else delete files
                  else
                  {
                    // if we try to delete this directory, move back a directory
                    if(file == curDir)
                      curDir = file->parent;
                    // delete the file if it isn't the root.
                    if(file != rootFile)
                    {
                      if((CheckFilePerms(curDir, 'w')) && (CheckFilePerms(file, 'w')))
                      {
                        file->parent->DeleteChild(file);
                        delete file;
                      }
                      else { std::cout << "rmdir: Invalid permissions for parent or subject folder\n"; }
                    }
                    // else error
                    else std::cout << "rmdir: Permission Denied\n";
                  }
              }
          }
        }
        // Handle chmod command
        else if(command == "chmod")
        {
          // int for conversion
          int permInt;
          // if there are more than 2 args
          if(args.size() > 2)
          {
            // error
            std::cout << 
              "chmod: Too many arguments - For help use: help chmod\n";
          }
          // if args is less 2 
          else if(args.size() < 2)
          {
            // error
            std::cout << 
              "chmod: Not enough arguments - For help use: help chmod\n";
          }
          else
          {
            try
            {
              // convert the number to an int
              permInt = std::stoi(args[0]);
              // make sure it is a valid 
              if(permInt > 777 || permInt < 0)
              {
                // error
                std::cout <<
                  "chmod: Invalid permission number\n";
              }
              else 
              {
                // iterate over args
                for(auto arg : args)
                {
                  // if is the first ignore
                  if(arg == args[0]) continue;
                  // try to find file
                  Node* file = findFile(arg);
                  // if file doesn't exist
                  if(file == nullptr)
                  {
                    // error 
                    std::cout << "File '" << arg << "' does not exist\n";
                  }
                  // else
                  else
                  {
                    // check premissions to set permissions hahaha
                    if ((CheckFilePerms(curDir, 'w')) && (CheckFilePerms(file, 'w')))
                    {
                      // break up the digit
                      file->perms = 
                      {
                        permInt / 100 % 10, 
                        permInt / 10 % 10, 
                        permInt % 10
                      };
                    }
                    else { std::cout << "chmod: Invalid permissions from file or folder\n"; }
                  }
                }
              }
            }
            catch(const std::exception&)
            {
              std::cout << "chmod: " << args[0] << " invalid Permissions\n";
            }
          }
        }
		// Add user
		else if(command == "useradd")
		{
			if(args.size() == 0 || args[0] == "")
			{ std::cout << "useradd: Not enough parameters\n"; }
			else if(args.size() == 1)
			{
				if(FindUser(args[0]))
				{
					std::cout << "useradd: User already exists\n";
				}
				else
				{
					//add user to default user group and to user map
					User* fresh_meat = new User(args[0], "users", true, "toor");
					if(!AddUser(fresh_meat))
						std::cout << "useradd: Error adding user. User not added.\n";
				}
			}
			else if(args.size() > 3)
				{ std::cout << "useradd: Too many parameters\n"; }
			else if(args.size() == 3)
			{
				if (args[0] == "-G")
				{
          // add user
          //add user to default user group and to user map
					User* fresh_meat = new User(args[2], "users", true, "toor");
					if(!AddUser(fresh_meat))
						std::cout << "useradd: Error adding user. User not added.\n";

          // parse the comma out of the string
					std::string groups = args[1];
          std::stringstream check(groups);
          std::string group;
          while(getline(check, group, ','))
          {
            if(!AddGroupToUser(fresh_meat, group))
            { std::cout << "useradd: Unable to add group to user\n"; }
          }
				}
				else
				{
					// if the flag is not valid
					std::cout << "useradd: Invalid flag\n";
				}
			}
			else { std::cout << "useradd: Invalid parameters\n"; }
			
		}
		
		// remove user
		else if(command == "userdel")
		{
			if(args.size() == 0 || args[0] == "")
				{ std::cout << "userdel: Not enough parameters\n"; }
			else if(args.size() > 3)
				{ std::cout << "userdel: Too many parameters\n"; }
			else if(args.size() == 1)
			{
				if(!rmUser(args[0]))
				{ std::cout << "userdel: User not found. User was not removed\n"; }
			}
      else if(args.size() == 3)
      {
        if(args[0] == "-G")
        {
          if(FindUser(args[2]))
          {
            User* user = GetUser(args[2]);
            // parse the comma out of the string
					std::string groups = args[1];
          std::stringstream check(groups);
          std::string group;
          while(getline(check, group, ','))
          {
            if(!RmGroupFromUser(user, group))
            { std::cout<< "userdel: Group not removed from user\n"; }
          }
          }
        }
      }
			else
				{ std::cout << "userdel: Invalid input\n"; }
		}
		
		// Switch user
		else if(command == "switchto")
		{
			if(args.size() < 1)
			{ std::cout << "switchto: Not enough parameters\n"; }
			else if(args.size() > 2)
			{
				std::cout << "switchto: Too many parameters\n";
			}
			else if (args.size() == 1)
			{
				
				if(FindUser(args[0]))
				{
					SwitchUser(GetUser(args[0]));
				}
				else { std::cout << "switchto: User not found\n"; }
			}
			else { std::cout << "switchto: Invalid command\n"; }
		}

    // change owner of file
    else if(command == "chown")
    {
      if(args.size() == 0)
				{ std::cout << "chown: Not enough parameters\n"; }
      else if (args.size() > 2)
      	{ std::cout << "chown: Too many parameters\n"; }
      else if (args.size() == 2)
      {
        if(FindUser(args[0]))
        {
          Node* file = FindFileObject(args[1]);
          if(file != nullptr)
          {
            file->SetOwner(args[0]);
          }
          else { std::cout << "chown: file not found\n"; }
        }
        else { std::cout << "chown: User not found\n"; }
      }
      else { std::cout << "chown: Invalid input\n"; }
    }

    // change group of file
    else if(command == "chgrp")
    {
      if(args.size() == 0)
				{ std::cout << "chgrp: Not enough parameters\n"; }
      else if (args.size() > 2)
      	{ std::cout << "chgrp: Too many parameters\n"; }
      else if (args.size() == 2)
      {
        Node* file = FindFileObject(args[1]);
        if(file != nullptr)
        {
          if(FindSystemGroup(args[0]))
          {
            file->AddGroup(args[0]);
          }
          else { std::cout << "chgrp: group does not exist\n"; }
        }
        else { std::cout << "chgrp: File not found\n"; }
      }
      else { std::cout << "chgrp: Invalid input\n"; }
    }

		// read file
		else if(command == "cat")
		{
			if(args.size() == 0)
				{ std::cout << "cat: Not enough parameters\n"; }
      else if (args.size() > 1)
      	{ std::cout << "cat: Too many parameters\n"; }
			else if(args.size() == 1)
			{
        Node* file = FindFileObject(args[0]);
        if(file != nullptr)
        {
				  if(CheckFilePerms(file, 'r'))				
				    { std::cout << args[0] << " file contents are on display.\n"; }
          else { std::cout << "cat: invalid permissions\n"; }
        }
        else { std::cout << "cat: File not found\n"; }
			}
			else { std::cout << "cat: Invalid input\n"; }
		}
		
		// execute command
		else if (command.substr(0,2) == "./" )
		{
			if(args.size() > 0)
			{ std::cout << "./(execute): Too many parameters\n"; }
			else if(args.size() == 0 && command.size() > 2)
			{
        if(CheckFilePerms(FindFileObject(command.substr(2,command.size())), 'x'))
				{
				std::cout << command.substr(2,command.size()) <<" file has been executed...\n";
				}
        else{ std::cout << "./(execute): Invalid permissions\n"; }
			}
			else { std::cout << "./(execute): Invalid command\n"; }
			
		}
		
		// Add to system groups
		else if (command == "groupadd")
		{
			if(args.size() == 0 || args[0] == "")
			{ std::cout << "groupadd: Too few params.\n"; }
			else if(args.size() > 1)
			{ std::cout << "groupadd: Too many params.\n"; }
			else if(args.size() == 1)
			{
				if(!AddSystemGroups(args[0]))
					{ std::cout << "groupadd: Group already exists.\n"; }
			}
			else
				{ std::cout << "groupadd: Invalid input.\n"; }
		}
		
		// Add to User groups
		else if(command == "usermod")
		{
			if(args.size() == 0 || args[0] == "")
			{ std::cout << "usermod: Too few params.\n"; }
			else if(args.size() > 3)
			{ std::cout << "usermod: Too many params.\n"; }
			else if(args.size() == 3 && args[0] == "-a" && args[1] == "-G")
			{

        // parse the comma out of the string
					std::string groups = args[2];
          std::stringstream check(groups);
          std::string group;
          while(getline(check, group, ','))
          {
            // add to the group, this will also check if group exists
            if(!addUsertoExistingGroup(curUser, group))
              { std::cout << "usermod: Could not add group to user.\n"; }
            else { std::cout << "usermod: Group already exists in user.\n"; }	
          }
			}
			else
				{ std::cout << "usermod: Invalid input.\n"; }
		}
		
		// View user groups
		else if(command == "groups")
		{
			if(args.size() == 0 || args[0] == "")
			{ std::cout << "group: Too few params.\n"; }
			else if(args.size() > 1)
			{ std::cout << "group: Too many params.\n"; }
			else if(args.size() == 1)
			{
				if(!printUserGroups(args[0]))
					{ std::cout << "group: User does not exist\n"; }
			}
			else
				{ std::cout << "group: Invalid input.\n"; }
		}

		
        // Handle help command
        else if(command == "help")
        {
          if(args.size() == 0 || args[0] == "")
          {
            std::cout << "Usage: help cmd : prints help for a given command\n";
            std::cout << "Usage: help -a : prints help for all avaliable commands\n";
          }
          else if(args[0] == "-a")
          {
            for(std::string cmd : CMDS)
            {
              commands("help", std::vector<std::string>{cmd});
            }
            
          }
          else if(args[0] == "ls")
          {
            std::cout << "Usage: ls : prints files in directory\n";
            std::cout << "Usage: ls -l : prints files in directory"
                      << " with extra information\n";
          }
          else if(args[0] == "pwd")
          {
            std::cout << "Usage: pwd : prints working file directory\n"; 
          }
          else if(args[0] == "exit")
          {
            std::cout << "Usage: exit : exits console\n";
          }
          else if(args[0] == "mkdir")
          {
            std::cout << "Usage: mkdir dir... : makes the directories listed\n";
          }
          else if(args[0] == "touch")
          {
            std::cout << "Usage: touch file... :will make a file if "
                      << "doesn't exist\n";
          }
          else if(args[0] == "cd")
          {
            std::cout << "Usage: cd dir : changes current working directory\n";
          }
          else if(args[0] == "rm")
          {
            std::cout << "Usage: rm file... : removes the files listed\n";
          }
          else if(args[0] == "rmdir")
          {
            std::cout << "Usage: rmdir dir... : removes the directories "
                      << "listed\n";
          }
          else if(args[0] == "chmod")
          {
            std::cout << "Usage: chmod ### file/dir... : changes permissions"
                      << " of files/directories listed\n";
          }
          else 
          {
            std::cout << "help: command doesn't exist\n";
          }
        }
        // else handle no command found
        else
        {
          std::cout << "Command '" << command << "' not found.\n";
        }
        
        return true;
      }
      // returns the current working directory
      std::string pwd()
      {
        std::string dir;
        // make a new tracker
        Node* traverse = curDir;
        // check to see if we are on the root.
        if(traverse == rootFile)
        {
          dir = "/";
        }
        // if we are not, work backwards
        while(traverse != rootFile)
        {
          if(traverse->IsDir())
            dir = "/" + traverse->Name() + dir; 

          traverse = traverse->Parent();
        }
        // return directory 
        return dir;
      }
      // Finds the file or not, takes a path and returns a pointer
      // pointer is null if it wasn't found
      Node* findFile(std::string path)
      {
        Node* next = curDir;
        // stream for parsing
        std::stringstream pathStream(path);
        bool succeed = true;
        // while there exists stuff to parse
        while(pathStream.peek() != std::char_traits<char>::eof())
        {
          std::string dir;
          // delimited on /
          getline(pathStream, dir, '/');
          // check if we are looking at the root of the directory
          if(dir == "" && pathStream.peek() == std::char_traits<char>::eof())
          {
            next = rootFile;
          }
          else
          {
            // if find .. then go to parent
            if(dir == "..")
            {
              next = next->parent;
            }
            // if . then stay
            else if(dir == ".")
            {
              // Nothing needed to do.
            }
            // else look for child.
            else
            {
              auto found = next->children.find(dir);
              succeed = found != next->children.end();
              // if looking and didn't find stop
              if(!succeed)
                break;
              else next = next->children[dir];
            }
          }
        }
        // return if we found it or no
        return succeed ? next : nullptr;
      }
  };
}
#endif
