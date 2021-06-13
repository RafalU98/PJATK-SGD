#ifndef ANIMATION
#define ANIMATION

#include "frame.h"

class Animation{
public:
	std::string name; //name of animation
	std::list<Frame> frames; //list of the frames

	Animation(std::string name = "");

	int getNextFrameNumber(int frameNumber);//returns the next frame number in the list
	Frame *getNextFrame(Frame* frame);
	int getEndFrameNumber(); //returns the last frames number
	Frame* getFrame(int frameNumber); //get frame using frame number

	void loadAnimation(std::ifstream &file, std::list<DataGroupType> &groupTypes);
};

#endif