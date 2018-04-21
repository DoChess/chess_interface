#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "./timer.hpp"
//The application time based timer
class Player
{
    public:
		Player();
		Timer timer;
		short int getFaults();
		void setFault();
		bool lostGamePerFault();

    private:
		short int faults;
};
