#ifndef PLAYER_h
#define PLAYER_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "./timer.hpp"

class Player
{
  public:
    Player();
    ~Player();
    Timer timer;

    std::string getFaults();
    void setFault();

    bool lostGamePerFault();
    bool lostGamePerTime();

  private:
    short int faults;
};

#endif
