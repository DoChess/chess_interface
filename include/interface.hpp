#ifndef INTERFACE_h
#define INTERFACE_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <bits/stdc++.h>
#include <sstream>
#include <iomanip>
#include "./player.hpp"

using namespace std;

class Interface
{
public:
    Interface();
    
    void setInformation(string info);
    string getInformation();

    void setStatusGame(string status);
    string getStatusGame();

    pair<Player, Player> controlTime(SDL_Event e, pair<Player, Player> players, 
        SDL_Color textColor, Interface* interface);
    
private:
    string informationGame;
    string statusGame;
};

#endif