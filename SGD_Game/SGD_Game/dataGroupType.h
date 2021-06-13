#ifndef DATAGROUPTYPE_H
#define DATAGROUPTYPE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

//describes the data that can be in the dpreadsheet responding file
class DataGroupType{
public:
	std::string groupName; //e.g hitBoxes
	int dataType; //what do we expect to get e.g. string or int
	bool singleItem; // bool if the datatype can only have 1 item 

	//STATIC DATA TYPES
	static const int DATATYPE_STRING = 0, DATATYPE_POSITION = 1, DATATYPE_BOX = 2, DATATYPE_NUMBER = 3;
};

#endif