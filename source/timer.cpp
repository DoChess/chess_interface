#include "../include/common.hpp"
#include "../include/timer.hpp"

using namespace std;

Timer::Timer()
{
    gameStartTicks = 0;
    gamePausedTicks = 0;

    gamePaused = false;
    gameStarted = false;
}

void Timer::start()
{
    gameStarted = true;

    gamePaused = false;

    gameStartTicks = SDL_GetTicks();
	gamePausedTicks = 0;
}

void Timer::stop()
{
    gameStarted = false;

    gamePaused = false;

	gameStartTicks = 0;
	gamePausedTicks = 0;
}

void Timer::pause()
{
    if( gameStarted && !gamePaused )
    {
        gamePaused = true;
        gamePausedTicks = SDL_GetTicks() - gameStartTicks;
		gameStartTicks = 0;
    }
}

void Timer::unpause()
{
    if( gameStarted && gamePaused )
    {
        gamePaused = false;
        gameStartTicks = SDL_GetTicks() - gamePausedTicks;
        gamePausedTicks = 0;
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
	Uint32 time = 0;

    if( gameStarted )
    {
        if( gamePaused )
        {
            time = gamePausedTicks;
        }
        else
        {
            time = SDL_GetTicks() - gameStartTicks;
        }
    }

    return formatTime(time);
}

bool Timer::isStarted()
{
	return gameStarted;
}

bool Timer::isPaused()
{
	return gamePaused && gameStarted;
}