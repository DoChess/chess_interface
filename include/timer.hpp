#ifndef TIMER_h
#define TIMER_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <bits/stdc++.h>
#include <sstream>
#include <iomanip>

class Timer
{
    public:
		//Initializes variables
		Timer();
	
		void start();
		void stop();
		void pause();
		void unpause();

		bool isStarted();
		bool isPaused();
        std::string showCurrentTime();
        std::string statusTimer();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};

#endif