#ifndef INTERFACE_h
#define INTERFACE_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <bits/stdc++.h>
#include <sstream>
#include <iomanip>
#include "./player.hpp"
#include "./common.hpp"

using namespace std;

class Interface
{
public:
    Interface();
    ~Interface();

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
    
    bool gameHasStarted;

    bool initLibsSDL();
    bool initInterface();
    bool loadMedias();
    void renderElements();
    void updateElements(pair<Player, Player> *players);
    
    void setInformation(string info);
    string getInformation();

    void setStatusGame(string status);
    string getStatusGame();

    void setLightCurrentPlayer(bool currentPlayer);
    bool isLightCurrentPlayer();

    void controlTime(string statusOfInformation, pair<Player, Player> *players,
        Interface* interface);
        
    void drawBackgroundInterface(string statusOfInformation);
    void isGameOver(pair<Player, Player>* players, pair<bool, string>* gameOver);

private:
    string informationGame;
    string statusGame;
    bool lightCurrentPlayer;
};

#endif
