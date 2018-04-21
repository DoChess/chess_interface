#include "../include/interface.hpp"

using namespace std;

Interface::Interface()
{
    informationGame = "Chess Game";
    statusGame = "Stoped";
}

void Interface::setStatusGame(string status)
{
    statusGame = status;
}

string Interface::getStatusGame()
{
    return statusGame;
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
    pair<Player, Player> players, SDL_Color textColor, Interface* interface){
	string currentTime = "";
	string statusGameText = "";

	if( e.key.keysym.sym == SDLK_s )
	{
		if( players.second.timer.isStarted() and players.first.timer.isStarted())
		{
			players.second.timer.stop();
			players.first.timer.stop();
			statusGameText = "stoped game";
		}
		else
		{
			players.first.timer.start();
			players.second.timer.start();
			players.second.timer.pause();
			currentTime = "light";
			statusGameText = "start game";
			cout << currentTime << " " << players.first.timer.isPaused() << " " << players.second.timer.isPaused() << endl;
		}
		currentTime = "";
	}
	//Pause/unpause
	else if( e.key.keysym.sym == SDLK_p )
	{
		cout << currentTime << " " << players.first.timer.isPaused() << " " << players.second.timer.isPaused() << endl;
		if( players.first.timer.isPaused() and players.second.timer.isPaused() )
		{
			if(currentTime=="light"){
				players.second.timer.unpause();
			} else {
				players.first.timer.unpause();
			}
			statusGameText = "restart game";
		}
		else
		{
			players.first.timer.pause();
			players.second.timer.pause();
			statusGameText = "paused game";
		}
		currentTime = "";
	}
	else if( e.key.keysym.sym == SDLK_c )
	{

		if (players.second.timer.isPaused())
		{
			players.second.timer.unpause();
			players.first.timer.pause();
			currentTime = "dark";
			players.first.setFault();
		} else {
			players.second.timer.pause();
			players.first.timer.unpause();
			currentTime = "light";
			players.second.setFault();
		}
		statusGameText = "Time of ";

	}

	if(players.first.lostGamePerFault())
	{
		statusGameText = "O primeiro perdeu";
	} else if (players.second.lostGamePerFault())
	{
		statusGameText = "O segundo perdeu";
	}
	interface->setStatusGame(statusGameText);

	return players;
}
