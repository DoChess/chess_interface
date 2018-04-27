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
		string statusOfInformation = "valid";
		string information = "CHESS D2 TO C3 MOVE";

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
				} 
			}
			// Metodo de comando de voz
			// 
			interface.updateElements( players );
			
			interface.setInformation( information );
			interface.drawBackgroundInterface( statusOfInformation );
			
			interface.renderElements();
			SDL_RenderPresent( interface.gRenderer );
		}
	}
	interface.close();

	return 0;
}
