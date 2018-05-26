#include "../include/common.hpp"
#include "../include/timer.hpp"

using namespace std;

Timer::Timer()
{
  gameStartTicks = 0;
  gamePausedTicks = 0;

  gamePaused = false;
  gameStarted = false;

  seconds = 0;
  minutes = 0;
  hours   = 0;
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

bool Timer::isStarted()
{
  return gameStarted;
}

bool Timer::isPaused()
{
  return gamePaused && gameStarted;
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

string Timer::formatTime(Uint32 mlseconds){
  stringstream time;
  int miliseconds = (Uint32) (mlseconds);
  this->seconds = (Uint32) (miliseconds / 1000) % 60;
  this->minutes = (Uint32) ((miliseconds / (1000*60)) % 60);
  this->hours   = (Uint32) ((miliseconds / (1000*60*60)) % 24);

  time << setfill('0') << setw(2) << this->hours << ":" << setfill('0') << setw(2) << this->minutes << ":" << setfill('0') << setw(2) <<this->seconds;
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

  const Uint32 HOUR_IN_MILISECONDS = 3600000;
  const Uint32 MINUTES_IN_MILISECONDS = 60000;
  const Uint32 SECONDS_IN_MILISECONDS = 1000;

  time = HOURS*(HOUR_IN_MILISECONDS) + MINUTES*(MINUTES_IN_MILISECONDS) + SECONDS*(SECONDS_IN_MILISECONDS) - time;

  return formatTime(time);
}

