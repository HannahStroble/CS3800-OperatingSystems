/*
Name: Hannah Reinbolt
Date: 2-12-2019
Teacher: Mike Gosnell
Class: CS 3800
Section: 1B
Assignment: Homework 2: Build a mock shell in c++

How to run:
1. Open terminal.
2. Type: g++ -std=c++11 hw1_hmrvg9.cpp
3. Type: ./a.out
4. The shell is now running.

NOTE: This code requires c++ 11 to compile.
*/

// libraries
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <typeinfo>
#include <vector>
#include <tuple>
#include <time.h>
using namespace std;

// main class that will create the folder tree and branching system.
// contains all folder and file information.
class FOLDERS
{
	
	public:
	// public variables
	vector<FOLDERS*> children;
	string perms;
	FOLDERS* parent;
	bool iamroot;
	string name;
	string time;
	vector<tuple<string, string> > files;

	// ################# DEFAULT CONSTRUCTORS ##############
	
	// basic edition, folder flag and name
	FOLDERS(bool boolfile, string foldername)
	{
		// default variable assignments
		iamroot = boolfile;
		name = foldername;
		perms = string("drwxrwxr-x") + string(" 1 ") + string("root") + string(" root ") 
		+ to_string(rand() % 10000 + 100) + string(" ") + timestamp() + string(" ") + string(foldername);
		time = timestamp();
	}

	// constructor that will allow the assignment of parents. Not really needed. 
	FOLDERS(bool boolfile, string foldername, FOLDERS* par)
	{
		// default variable assignments
		iamroot = boolfile;
		name = foldername;
		perms = string("drwxrwxr-x") + string(" 1 ") + string("root") + string(" root ") 
		+ to_string(rand() % 10000 + 100) + string(" ") + timestamp() + string(" ") + string(foldername);
		parent = par;
		time = timestamp();
	}
	
	// Find current time
	string timestamp()
	{
		// find timestamp
		time_t alltime;
		struct tm * timeinfo;
	
		// find local time
		//time(&alltime);
		timeinfo = localtime(&alltime);
		string current_time = asctime(timeinfo);
		
		// get rid of a random newline that is somewhere in here
		current_time.erase(remove(current_time.begin(), current_time.end(), '\n'), current_time.end());
		return current_time;
	}

	// ########## ADD AND REMOVE ITEMS TO FOLDERS ###########
	
	// add parent node
	void add_parent(FOLDERS* p)
	{
		// add parent, in this case will be used in child assignment
		parent = p;
	}

	// add children nodes
	FOLDERS* add_children(FOLDERS* new_child, FOLDERS* parent)
	{
		// add children to your parent folder
		new_child->add_parent(parent);
		children.push_back(new_child);
		return this;
	}

	// add file to node
	FOLDERS* add_file(string filename)
	{
		// create default permissions
		string permission = "";
		
		// default permissions that will be added to file details
		permission = string("-rwxrwxr-x") + string(" 1 ") + string("root") + string(" root ") 
		+ to_string(rand() % 10000 + 100)  + string(" ") + string(timestamp()) + string(" ") + string(filename);
		
		tuple <string, string> result = make_tuple(filename, permission);
		files.push_back(result);
		return this;
	}

	// remove file to node
	FOLDERS* rm_file(string filename)
	{
	for (int j=0; j < files.size(); j++)
		{ 
			if(get<0>(files[j]) == filename)
			{
				//erase files
				files.erase(files.begin() + j);
				return this;
			}	
		}
		return this;
	}

	// remove child folder
	FOLDERS* rm_child(string childname)
	{
	for (int i=0; i < children.size(); i++)
		{
			if(children[i]->name == childname)
			{
				//erase child folder
				children.erase(children.begin() + i);
				return this;
			}
	
		}
		return this;
	}
	
	// add or change permissions
	void add_perms(string p)
	{
		perms = p;
	}


	// ########## PERMISSIONS, MOVEMENT, AND OTHER #############

	// move to child folder
	FOLDERS* move_to_child(string childname, FOLDERS* parent)
	{
		for (int i=0; i < children.size(); i++)
		{
			if(children[i]->name == childname)
			{
				// assign child to parent to move to that folder
				return parent->children[i];
			}	
		}
		return parent;
	}
		
	
	// edit permissions to files and folders
	FOLDERS* chmd(string fname, string permission, bool is_file)
	{
		//send to find permissions string
		string new_perms = permission_finder(permission);
		
		// check if file or folder
		if(is_file == false)
		{
				for (int j=0; j < files.size(); j++)
				{ 
					if(get<0>(files[j]) == fname)
					{
						// create a new permissions tuple and add it to file details
						string result = "";
						string temp = get<1>(files[j]).substr(10);
						result = string("-") + new_perms + temp;
						files.erase(files.begin()+j);
						files.push_back(make_tuple(fname, result));
					}
				}
		}
		// if it is a folder then change permissions here
		else
		{
			for (int i=0; i < children.size(); i++)
			{
				
				if(children[i]->name == fname)
				{
					// update the new permissions to child
					string result = "";
					string temp = children[i]->perms;
					temp = temp.substr(10);
					result = string("d") + new_perms + temp;
					children[i]->add_perms(result);
				}
			}
		}
		//just in case all fails
		return this;
	}
	
	// find permission based upon octal number
	string permission_finder(string number)
	{
			// calculate octal permissions based upon this scale:
			// 4=read, 2=write, 1=execute, 0=no permission
			// each number is a combination of these that relay the chmd permissions.
			
			string result = "";
			string iter = "01234567";
			
			// const char and char are two very different things, this will compare the same type
			// compare and create the right permissions using the octal system
			for(int i=0; i < 3; i++)
			{
				
					if(number[i] == iter[0])
						result = result + string("---");
					else if(number[i] == iter[1])
						result = result + string("--x");
					else if(number[i] == iter[2])
						result = result + string("-w-");
					else if(number[i] == iter[3])
						result = result + string("-wx");
					else if(number[i] == iter[4])
						result = result + string("r--");
					else if(number[i] == iter[5])
						result = result + string("r-x");
					else if(number[i] == iter[6])
						result = result + string("rw-");
					else if(number[i] == iter[7])
						result = result + string("rwx");
			}
			return result;
	}


	// ############ PRINT FUNCTIONS ###################

	// print everything in this directory
	void ls_print()
	{
	// print all folders in current directory
	for (int i=0; i < children.size(); i++)
		{
		cout<<children[i]->name<<" ";	
		}
	// print all files in current directory
	for (int j=0; j < files.size(); j++)
		{ 
		cout << get<0>(files[j]) << " ";	
		}
		cout<<endl;
	}

	// print everything in this directory and permission details
	void lsl_print()
	{
	// print details on folders
	for (int i=0; i < children.size(); i++)
		{
		cout<<children[i]->perms<<endl;	
		}
	// print details on files
	for (int j=0; j < files.size(); j++)
		{ 
		cout << get<1>(files[j]) << endl;	
		}
		cout<<endl;
	}
	
	// print everything in this directory
	bool is_folder(string thingname)
	{
	
	// if a folder then search
	for (int i=0; i < children.size(); i++)
		{
			if(children[i]->name == thingname)
			{
				return true;
			}				
		}
	// if a file then search
	for (int j=0; j < files.size(); j++)
		{ 
			if(get<0>(files[j]) == thingname)
			{
				return false;
			}
		}
		
		// bad practice but return NULL if neither were found
		return NULL;
	}

};

// find current directory
	void pwd(FOLDERS* parent)
	{
		// starting variables
		vector<string> dirs;
		string names = "";
		FOLDERS* temp = parent;
		temp = parent;
		
		// if the parent is root then only add that name to the list
		if(temp->iamroot == true || temp->name == "root")
		{
			names = temp->name;
			dirs.push_back(names);
		}
		// for all other directories, cycle through until root is hit
		else
		{
			names = temp->name;
			dirs.push_back(names);
			while(temp->iamroot != true || temp->name != "root")
			{
				temp = temp->parent;
				names = temp->name;
				dirs.push_back(names);
			}
		}
		//cycle through list backwards to print current directory
		for (int i=dirs.size(); i >= 0; i--)
		{
		cout<<"/"<<dirs[i];	
		}
		cout<<endl;
		
		// release memory
		//delete temp;
	}


// shell welcome screen
void start_shell()
{
	// ANSI escape codes to clear the screen
	cout << "\033[2J\033[1;1H";
	cout<<"Starting shell...\n";
	cout<<"Please type 'help' for available commands.\n";
	cout << "\033[2J\033[1;1H";
	// Sleep 1 cycle
	// is it really working hard?
	// pff no
	// the illusion is magic
	// wooOOooooOooooooo
	sleep(1);	
};

// get user input from terminal
string get_input()
{
	// variables
	string userinput = "";
	
	// Starting line
	cout<<">>> ";
	getline(cin,userinput);
	return userinput;
	
};

// print help screen
void helpme()
{
	cout	<< "###### AVAILABLE COMMANDS ################################################## \n"
		<< "ls ------------------------- : View files and folders\n"
		<< "ls-l ----------------------- : View files and folders with permission details\n"
		<< "pwd  ----------------------- : Print current directory path from root\n"
		<< "cd [folder]  --------------- : Move up a directory to the directed folder\n"
		<< "cd../ ---------------------- : Move back a directory\n"
		<< "chmod [number] [file/folder] : Edit the permissions of a file or folder\n"
		<< "rm [file] ------------------ : Delete file\n"
		<< "touch [file] --------------- : Create a file\n"
		<< "mkdir [folder] ------------- : Create directory/folder\n"
		<< "rmdir [folder] ------------- : Delete directory/folder\n"
		<< "help ----------------------- : Print available commands\n"
		<< "exit/quit ------------------ : Exit the program\n"
		<<endl;
}

// main program that will run the shell
int main() {

// start variables
int i = 0;
string command = "";
start_shell();
FOLDERS* current_dir = new FOLDERS(true, "root");

	// start a forever shell, have it listen for input
	while(i==0)
	{
	// get user input
	command = get_input();

		if(command.length() != 0)
		{
			// exit shell if user types exit			
			if(command == "exit" || command == "quit")
			{
				break;
			}

			// print help screen
			else if(command == "help")
			{
				helpme();
			}

			// make a new directory
			else if(command.substr(0,5) == "mkdir")
			{
				string name = "";
				if(command.length() > 5 && command.substr(7,8) != " ")
				{
					name = command.substr(6);

					// create child folder
					current_dir = current_dir->add_children(new FOLDERS(false, name), current_dir);
				}
			}

			// delete a directory
			else if(command.substr(0,5) == "rmdir")
			{
				string name = "";
				if(command.length() > 5 && command.substr(7,8) != " ")
				{
					name = command.substr(6);

					// remove child folder
					current_dir = current_dir->rm_child(name);
				}
			}

			// create files in directory
			else if(command.substr(0,5) == "touch")
			{
				string name = "";
				if(command.length() > 5 && command.substr(7,8) != " ")
				{
					name = command.substr(6);
					
					// create file
					current_dir = current_dir->add_file(name);
				}
			}

			// remove files in directory
			else if(command.substr(0,2) == "rm")
			{
				string name = "";
				if(command.length() > 2 && command.substr(4,5) != " ")
				{
					name = command.substr(3);
					// remove file
					current_dir = current_dir->rm_file(name);
				}
			}

			// move up a directory
			else if(command.substr(0,5) == "cd../" || command.substr(0,6) == "cd ../")
			{	
				// move to parent folder
				if(current_dir->parent != NULL || current_dir->parent != 0)
				{
					current_dir = current_dir->parent;
				}
			}

			// move down to a child directory
			else if(command.substr(0,2) == "cd")
			{
				string name = "";
				if(command.length() > 2 && command.substr(4,5) != " ")
				{
					name = command.substr(3);
					// move to child folder
					current_dir = current_dir->move_to_child(name, current_dir);
				}
			}
			
			// print current directory
			else if(command == "pwd")
			{
				//print current directory
				pwd(current_dir);
			}
			
			// show files and folders in directory
			else if(command == "ls")
			{
				current_dir->ls_print();
			}
			
			// edit permissions of files or folders
			else if(command.substr(0,5) == "chmod")
			{
				//check if there is more input like a filename or a foldername
				if(command.length() > 10 && command.substr(7,8) != " ")
				{
					// get permission number from user input
					string perms = command.substr(6,3);
					string filename = command.substr(10);
					
					// change permissions on files or folders
					if(current_dir->is_folder(filename) == true)
					{
						current_dir = current_dir->chmd(filename, perms, true);
					}
					else if(current_dir->is_folder(filename) == false)
					{
						current_dir = current_dir->chmd(filename, perms, false);
					}
					else
						cout<<"Folder or file not found!\n";
				}
				
			}
			
			// show files and folders in directory with permission details
			else if(command.substr(0,4) == "ls-l")
			{
				//check if there is more input like a filename or a foldername
				if(command.length() == 4)
				{
					// print all information in directory
					current_dir->lsl_print();
				}
				else if(command.substr(5) == current_dir->name)
				{
					cout<<current_dir->perms<<endl;
				}
				
			}

			// if user input is not a command then send error code
			else
			{
				cout<<"\nError! Invalid input. Try again.\n";
			}

		continue;
		}
		else
		{
			cout<<"\nError! Invalid input. Try again.\n";
		}
}
// clear up memory
delete current_dir;

cout<<"End of Line.\n"<<endl;
return 0;

};

