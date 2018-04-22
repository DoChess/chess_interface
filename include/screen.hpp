#ifndef SCREEN_h
#define SCREEN_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <bits/stdc++.h>
#include <sstream>
#include <iomanip>
#include "./player.hpp"
#include "./ltexture.hpp"
#include "./interface.hpp"

using namespace std;

class Screen
{
public:
    Screen();

    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    
    TTF_Font* gFont;
    TTF_Font* gFontTimer;
    SDL_Texture* gTexture;

    LTexture playerTimeDarkTexture;
    LTexture playerTimeLightTexture;
    LTexture playerFailuresDarkTexture;
    LTexture playerFailuresLightTexture;
    LTexture gStatusGameTexture;
    LTexture gInfoTexture;
    LTexture gSpriteSheetTexture;
    
    bool init();
    bool loadMedias();
    void close();

    void updateElements(pair<Player, Player> players, Interface* interface);
    void renderElements();

private:
    
};

#endif