#ifndef GROUPBUILDER
#define GROUPBUILDER

#include "globals.h"
#include "group.h"
#include "groupBox.h"
#include "groupPosition.h"
#include "GroupNumber.h"
#include "groupString.h"

class GroupBuilder
{
public:
	static const bool savedInGroups;

	//builds a group of datatypes
	static Group* buildGroup(DataGroupType dataType);
	
	//builds a set of groups
	static void buildGroups(std::list<DataGroupType> groupTypes, std::list<Group*> &groups);

	//add a groupstring to group if we need a reference
	static Group* addGroupStringToGroup(std::string name, std::list<Group*> &groups);

	//loads a group out of a file
	static void loadGroups(std::ifstream &file, std::list<Group*> &groups);

	//can find a group based on group name
	static Group* findGroupByName(std::string str, std::list<Group*> &groups);
};

#endif