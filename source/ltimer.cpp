#include "../include/common.hpp"

using namespace std;

Player::Player()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void Player::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void Player::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void Player::pause()
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

void Player::unpause()
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
	stringstream t;
	int miliseconds = (int) (mlseconds);
	int seconds = (int) (miliseconds / 1000) % 60;
	int minutes = (int) ((miliseconds / (1000*60)) % 60);
	int hours   = (int) ((miliseconds / (1000*60*60)) % 24);
	t << setfill('0') << setw(2) << HOURS - hours << ":" << setfill('0') << setw(2) << MINUTES - minutes << ":" << setfill('0') << setw(2) << SECONDS - seconds;
	return t.str();
}

string Player::showCurrentTime()
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

bool Player::isStarted()
{
	//Timer is running and paused or unpaused
    return mStarted;
}

bool Player::isPaused()
{
	//Timer is running and paused
    return mPaused && mStarted;
}

