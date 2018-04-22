#include "../include/common.hpp"
#include "../include/player.hpp"
#include "../include/interface.hpp"
#include "../include/screen.hpp"

using namespace std;

int main( int argc, char* args[] )
{
	Screen screen;
	if( !screen.init() )
	{
		printf( "Failed to initialize graphic interface!\n" );
	}
	else
	{
		if( !screen.loadMedias() )
		{
			printf( "Failed to load medias!\n" );
		}
		else
		{
			bool quit = false;
			SDL_Event e;
		
			Player lightPlayer, darkPlayer;
			Interface interface;

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
				
				screen.updateElements( players, &interface );
				SDL_RenderClear( screen.gRenderer );
				
				interface.setInformation( information );
				interface.drawRet( screen.gRenderer, statusOfInformation );
				
				screen.renderElements();

				SDL_RenderPresent( screen.gRenderer );
			}
		}
	}

	screen.close();

	return 0;
}
