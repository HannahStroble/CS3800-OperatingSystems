/*
 * Name: Cash
 * Class: Operating Systems
 * Assignment: Shell Emulator
 */

#include <map>
#include <string>
#include <ctime>
#include <array>
#include <chrono>
#include <algorithm>
#include <set>
#include "user.h"

#ifndef NODE_H
#define NODE_H
namespace Shell
{
  // share with computer
  class Computer;
  // constants because I am lazy 
  const std::array<int, 3> DEFAULT_PERM = {7,6,3};
  const std::array<std::string, 12> MONTHS = 
  {
    "Jan", "Feb", "Mar", "Apr", "May", 
    "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

  // Node class representing a file
  // similar stucture to a doubly linked list
  // NOTE: this contains some functions and parameters that were
  // to be a additional functionality incase we continue
  // using this assignment.
  class Node
  {
    private:
      // a link to its parent
      Node* parent;
      // list of children
      std::map<std::string, Node*> children;
      // the name of file
      std::string name;
      // if file is a directory
      bool isDir;
      // user owner
      std::string user; 
      // user group
      std::set<std::string> group;
      // size of file
      int size;
      // time stamp
      tm timeStamp;
      // permissions
      std::array<int, 3> perms;
      // friends with a computer
      friend Computer;
    public:
      // Constructors 
      Node(std::string n, bool dir, Node* p, int s, std::string u, std::string g)
      {
        auto time = std::chrono::system_clock::now(); 
        std::time_t timet = std::chrono::system_clock::to_time_t(time);
        timeStamp = *std::localtime(&timet);
        name = n;
        parent = p;
        children = std::map<std::string, Node*>();
        isDir = dir;
        user = u;
        group.insert(g);
        size = s;
        perms = DEFAULT_PERM;
      }
      Node(std::string n, bool dir, Node* p) : Node(n, dir, p, 1, p->user, p->Group()) { }
      // deconstructor
      ~Node()
      {
        parent = nullptr;
        for(auto node : children)
        {
          delete node.second;
        }
      }
      // updates the timestamp of the node
      void UpdateTimeStamp()
      {
        auto time = std::chrono::system_clock::now(); 
        std::time_t timet= std::chrono::system_clock::to_time_t(time);
        timeStamp = *std::localtime(&timet); 
      }

      // Getters
      // not used really oh well
      Node* Parent() const { return parent; }
      std::map<std::string, Node*> Children() const { return children; }
      std::string Name() const { return name; }
      bool IsDir() const { return isDir; }
      std::string User() const { return user; }
      std::set<std::string> GetGroups() const { return group; }
      std::string Group() const 
	  { 
		// add all the groups together to return in a string
		std::string result = "";
		for(auto part : group)
		{
		  result = result + " " + part;
		}
	  return result;  
	  }
      int Size() const { return size; }
      tm TimeStamp() const { return timeStamp; }
      std::array<int, 3> Perms() const { return perms; }

      // add to group
      bool AddGroup(std::string name)
      {
        bool result = group.find(name) != group.end();
        if(!result)
        {
          group.insert(name);
        }
        return false;
      }

      // gets the string of the permission 
      std::string PermsStr() const 
      {
        std::string str = "";

        str += isDir ? "d" : "-";
        for(auto perm : perms)
        {
          str += perm / 4 > 0 ? "r" : "-";
          str += perm % 4 / 2 > 0 ? "w" : "-";
          str += perm % 4 % 2 == 1 ? "x" : "-";
        }
        return str;
      }
      // convert time object to time
      std::string TimeStr() const
      {
        return MONTHS[timeStamp.tm_mon] + "  " + 
        std::to_string(timeStamp.tm_mday) + " " + 
        std::to_string(timeStamp.tm_hour) + ":" + 
        std::to_string(timeStamp.tm_min);
      }
      // counts the number of directories
      int NumDirs() const
      {
        int count = 0;
        if(!isDir) return 1;
        for(auto child : children)
        {
          if(child.second->isDir)
            count++;
        }
        // adds two for some reason ask linux why.
        return count + 2; 
      }

      // set owner
      void SetOwner(std::string newowner)
      {
        user = newowner;
      }
    
      // Add Child
      bool AddChild(Node* child)
      {
        bool succeed = false;
        child->parent = this;
        if(isDir)
        {
          succeed = children.emplace(child->name, child).second;
        }
        
        if(!succeed)
          delete child;
        return succeed;
      }
    // private funcs
    private:
      // deletes a child returns true if succeeded
      bool DeleteChild(Node* child)
      {
        bool succeed = false;
        auto found = 
          children.find(child->name);

        if(found != children.end() && found->second == child)
        {
          children.erase(found);
          if(child->isDir)
            child->DeleteChildren();
          succeed = true;
        }
        return succeed;
      }
      // Deletes all children of a file.
      void DeleteChildren()
      {
        for(auto node : children)
        {
          if(node.second->isDir)
            node.second->DeleteChildren();
          delete node.second;
        }
        children.empty();
      }
      // moves files not actually implemented
      bool MoveTo(Node* p)
      {
        bool succeed;
        
        succeed = parent->DeleteChild(this);

        if(succeed)
        {
          parent = p;
          succeed = parent->AddChild(this);
        }

        return succeed;
      }
      // public functions
      public:
        // compares two nodes.
        static bool Compare(const Node * const a, const Node * const b)
        {
          return a->name < b->name;
        }
  };

}
#endif
