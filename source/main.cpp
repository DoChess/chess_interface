#include "../include/common.hpp"
#include "../include/player.hpp"
#include "../include/interface.hpp"

using namespace std;

int main( int argc, char* args[] )
{
	Interface interface;
	if( interface.initInterface() )
	{
		bool quit = false;
		SDL_Event e;
	
		Player lightPlayer, darkPlayer;

		pair<Player, Player> players (lightPlayer, darkPlayer);
		string statusOfInformation = "";
		string information = "Start chess";

		while( !quit )
		{
			while( SDL_PollEvent( &e ) != 0 )
			{
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}
				else if( e.type == SDL_KEYDOWN )
				{
					players = interface.controlTime(e, players, &interface);
				
					if( e.key.keysym.sym == SDLK_q )
					{
						information = "CHESS D2 TO C3 MOVE";
						statusOfInformation = "analysing";
					}
					else if( e.key.keysym.sym == SDLK_w )
					{
						information = "MOVING D2 TO C3";
						statusOfInformation = "valid";
					}
					else if( e.key.keysym.sym == SDLK_e )
					{
						information = "INVALID MOVE";
						statusOfInformation = "invalid";
					} else {
						statusOfInformation = "";
					}
				} 
			}
			// Metodo de comando de voz
			// 
			
			interface.updateElements( players );
			
			if(players.first.lostGamePerFault() or players.second.lostGamePerFault()){
				information = "Player lost per fault";
			} else if (players.first.lostGamePerTime() or players.second.lostGamePerTime()){
				information = "Player lost per time";
			}

			interface.setInformation( information );
			interface.drawBackgroundInterface( statusOfInformation );
			
			interface.renderElements();
			SDL_RenderPresent( interface.gRenderer );

			
		}
	}
	interface.close();

	return 0;
}
