#ifndef GROUPPOS_H
#define GROUPPOS_H

#include "group.h"

class GroupPosition : public Group{
public:
	std::list<SDL_Point> data;

	GroupPosition(DataGroupType type){
		this->type = type;
	}
	int numberOfDataInGroup(){
		return data.size();
	}
	
	void addToGroup(std::string str){
		//hope this works :/
		std::stringstream ss;
		ss << str;
		SDL_Point pt;
		ss >> pt.x >> pt.y ;

		data.push_back(pt);
	}

	void draw(){
		//TODO
	}

};


#endif