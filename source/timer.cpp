#include "../include/common.hpp"
#include "../include/timer.hpp"

using namespace std;

Timer::Timer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void Timer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void Timer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
    }
}

void Timer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

string formatTime(Uint32 mlseconds){
	stringstream time;
	int miliseconds = (int) (mlseconds);
	int seconds = (int) (miliseconds / 1000) % 60;
	int minutes = (int) ((miliseconds / (1000*60)) % 60);
	int hours   = (int) ((miliseconds / (1000*60*60)) % 24);
	time << setfill('0') << setw(2) << HOURS - hours << ":" << setfill('0') << setw(2) << MINUTES - minutes << ":" << setfill('0') << setw(2) << SECONDS - seconds;
	return time.str();
}

string Timer::showCurrentTime()
{
	//The actual timer time
	Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return formatTime(time);
}

bool Timer::isStarted()
{
	//Timer is running and paused or unpaused
    return mStarted;
}

bool Timer::isPaused()
{
	//Timer is running and paused
    return mPaused && mStarted;
}

string Timer::statusTimer(){
    string status;
    if(isStarted()){
        if(isPaused()){
            status = "Paused";
        } else {
            status = "Running";
        }
    } else {
        status = "Stoped";
    }
    return status;
}
