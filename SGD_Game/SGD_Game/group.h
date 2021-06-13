#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <SDL.h>
#include "dataGroupType.h"

#ifndef GROUP_H
#define GROUP_H

class Group{
public:
	DataGroupType type; //describes its name, type and other rules

	Group(){
	}

	Group(DataGroupType type){
		this->type = type;
	}
	//how many bits of data in here
	virtual int numberOfDataInGroup() = 0;
	//convert strings int needed type
	virtual void addToGroup(std::string str) = 0;

	virtual void draw() = 0;
};

#endif