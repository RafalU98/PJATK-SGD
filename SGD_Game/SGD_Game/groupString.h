#ifndef GROUPSTRING_H
#define GROUPSTRING_H

#include "group.h"

class GroupString : public Group{
public:
	std::list<std::string> data;

	GroupString(DataGroupType type){
		this->type = type;
	}
	int numberOfDataInGroup(){
		return data.size();
	}
	void addToGroup(std::string str){
		//whitespaces = eww
		if (str[0] == ' '){
			str.erase(0, 1);
		}

		data.push_back(str);
	}

	void draw(){
		//TODO
	}
};


#endif