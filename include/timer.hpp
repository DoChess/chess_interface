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
    Timer();

    void start();
    void stop();
    void pause();
    void unpause();

    bool isStarted();
    bool isPaused();

    std::string showCurrentTime();
    std::string formatTime(Uint32 mlseconds);

    Uint32 seconds;
    Uint32 minutes;
    Uint32 hours;

  private:
    Uint32 gameStartTicks;
    Uint32 gamePausedTicks;

    bool gamePaused;
    bool gameStarted;

};

#endif
