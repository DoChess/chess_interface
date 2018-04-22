#include "../include/interface.hpp"

using namespace std;

Interface::Interface()
{
    informationGame = "Chess Game";
    statusGame = "Stoped";
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
			statusGameText = "stoped";
		}
		else
		{
			players.first.timer.start();
			players.second.timer.start();
			players.second.timer.pause();
	
			statusGameText = "running";
		}
	}
	else if( e.key.keysym.sym == SDLK_p )
	{
		if( players.first.timer.isPaused() and players.second.timer.isPaused() )
		{
			(interface->isLightCurrentPlayer()) ? players.first.timer.unpause():
				players.second.timer.unpause();
			
			statusGameText = "running";
		}
		else
		{
			players.first.timer.pause();
			players.second.timer.pause();
			statusGameText = "paused";
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

		statusGameText = "running";
	}

	interface->setStatusGame(statusGameText);

	return players;
}
