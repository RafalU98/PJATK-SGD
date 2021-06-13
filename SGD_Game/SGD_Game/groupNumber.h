#ifndef GROUPNUMBER_H
#define GROUPNUMBER_H

#include "group.h"

class GroupNumber : public Group{
public:
	std::list<float> data;

	GroupNumber(DataGroupType type){
		this->type = type;
	}
	int numberOfDataInGroup(){
		return data.size();
	}
	void addToGroup(std::string str){
		//hope this works :/
		std::stringstream ss;
		ss << str;
		float r;
		ss >>r;
		data.push_back(r);
	}

	void draw(){
		//TODO
	}
};

#endif