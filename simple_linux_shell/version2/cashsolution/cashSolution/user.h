
#include <string>
#include <set>


#ifndef USER_H
#define USER_H

namespace Shell
{
  // basic user class built for future use. 
  // Mostly use to store a name and password atm
  class User
  {
    private:
      std::string uname;
      std::set<std::string> group;
      bool isAdmin;
      std::string pword;
    public:
      // Constructor
      User(std::string name, std::string g, bool a, std::string pass) 
      {
        uname = name;
        group.insert(g);
        isAdmin = a;
        pword = pass;
      }
      // getters
      std::string Username() const { return uname; }
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

      std::set<std::string> GetGroups() const { return group; }
	  
	  // find group in set
	  bool FindGroup(std::string group_name)
	  {
		  for(auto part : group)
		  {
			  if(part == group_name)
			  {  return true; }
		  }
		  return false;
	  }
	  
      bool IsAdmin() const { return isAdmin; }
      // verify password bc not gonna expose password
      bool VerifyPassword(std::string pass) const { return pword == pass; }
	  
	  // setter
	  void AddGroup(std::string group_name)
	  {
			group.insert(group_name); 
	  }

		void RmGroup(std::string group_name)
	  {
			bool result = group.find(group_name) != group.end();
			if(result)
			{ group.erase(group_name); }
	  }
  
  };
} // 

#endif
