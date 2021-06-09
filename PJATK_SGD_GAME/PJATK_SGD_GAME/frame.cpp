#include "frame.h"

void Frame::Draw(SDL_Texture* spriteSheet, float x, float y) {
	SDL_Rect destination; //destination where we want to draw this
	destination.x = x - offSet.x;
	destination.y = y - offSet.y;
	destination.w = clip.w;
	destination.h = clip.h;

	renderTexture(spriteSheet, Globals::renderer, destination, &clip);
}


void Frame::loadFrame(ifstream& file, list<DataGroupType>& groupTypes) {
	//Ok, we dragged these groupTypes allllll the way here, so we could construct groups on our frame to hold the data in
	GroupBuilder::buildGroups(groupTypes, frameData);


	string buffer;
	//frame - dud data saying this is a frame
	getline(file, buffer);
	//clip
	getline(file, buffer);
	stringstream ss; // good for building strings and reading from strings
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> clip.x >> clip.y >> clip.w >> clip.h; // std::cin >> clip.x >> clip.y >> etc
	//offset/pivot
	getline(file, buffer);
	ss.clear();
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> offSet.x >> offSet.y;
	//duration
	getline(file, buffer);
	ss.clear();
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> duration;
	//index
	getline(file, buffer);
	ss.clear();
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> frameNumber;

	GroupBuilder::loadGroups(file, frameData);
}