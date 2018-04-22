#include "../include/interface.hpp"

using namespace std;

Interface::Interface()
{
    informationGame = "Chess Game";
    statusGame = "../assets/imgs/play_white.png";
	lightCurrentPlayer = true;
}

void Interface::setStatusGame(string status)
{
    statusGame = status;
}

string Interface::getStatusGame()
{
    return statusGame;
}

void Interface::setLightCurrentPlayer(bool currentPlayer)
{
    lightCurrentPlayer = currentPlayer;
}

bool Interface::isLightCurrentPlayer()
{
    return lightCurrentPlayer;
}

void Interface::setInformation(string info)
{
    informationGame = info;
}

string Interface::getInformation()
{
    return informationGame;
}

pair<Player, Player> Interface::controlTime(SDL_Event e, 
    pair<Player, Player> players, Interface* interface){
	string statusGameText = "";

	if( e.key.keysym.sym == SDLK_s )
	{
		if( players.second.timer.isStarted() and players.first.timer.isStarted())
		{
			players.second.timer.stop();
			players.first.timer.stop();
			statusGameText = "../assets/imgs/play_black.png";
		}
		else
		{
			players.first.timer.start();
			players.second.timer.start();
			players.second.timer.pause();
	
			statusGameText = "../assets/imgs/play_white.png";
		}
	}
	else if( e.key.keysym.sym == SDLK_p )
	{
		if( players.first.timer.isPaused() and players.second.timer.isPaused() )
		{
			(interface->isLightCurrentPlayer()) ? players.first.timer.unpause():
				players.second.timer.unpause();
			
			statusGameText = "../assets/imgs/pause.png";
		}
		else
		{
			players.first.timer.pause();
			players.second.timer.pause();
			statusGameText = "../assets/imgs/play_white.png";
		}
	}
	else if( e.key.keysym.sym == SDLK_c )
	{

		if (players.second.timer.isPaused())
		{
			players.second.timer.unpause();
			players.first.timer.pause();
			players.first.setFault();

			interface->setLightCurrentPlayer(false);
		} else {
			players.second.timer.pause();
			players.first.timer.unpause();
			players.second.setFault();

			interface->setLightCurrentPlayer(true);
		}

		statusGameText = "../assets/imgs/pause.png";
	}

	interface->setStatusGame(statusGameText);

	return players;
}


void Interface::drawRet(SDL_Renderer* gRenderer, string statusOfInformation){
	SDL_Rect blackRet = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 };
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );		
	SDL_RenderFillRect( gRenderer, &blackRet );
	SDL_Rect whiteRet = { SCREEN_WIDTH / 500, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
	SDL_RenderFillRect( gRenderer, &whiteRet );
	
	int color[4];

	if(statusOfInformation == "analysing"){
		color[3] = 0;
		color[2] = 96;
		color[1] = 255;
		color[0] = 255;
	} else if(statusOfInformation == "valid"){
		color[3] = 13;
		color[2] = 239;
		color[1] = 66;
		color[0] = 255;
	} else if(statusOfInformation == "invalid"){
		color[3] = 220;
		color[2] = 61;
		color[1] = 42;
		color[0] = 255;
	} else {
		color[3] = 255;
		color[2] = 255;
		color[1] = 255;
		color[0] = 255;
	}
	
	SDL_Rect fillRect2 = { 1, SCREEN_HEIGHT / SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT/2};
	SDL_SetRenderDrawColor( gRenderer, color[3], color[2], color[1], color[0] );		
	SDL_RenderFillRect( gRenderer, &fillRect2 );
}
