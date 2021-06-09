#ifndef TIMECONTROLLER
#define TIMECONTROLLER

#include <iostream>
#include <SDL.h>

class TimeController {
public:
	//
	static const int PLAY_STATE, PAUSE_STATE;

	int timeState;
	Uint32 lastUpdate; //last time on checking how many ticks passed in game
	float dT; //delta time, in seconds, amount of time since the last frame was renderer (fps)

	TimeController();
	void updateTime(); //update lastUpdate and dT
	void pause();
	void resume();
	void reset();

	static TimeController timeController;
};


#endif
