#include "../include/common.hpp"
#include "../include/player.hpp"
#include "../include/interface.hpp"
#include "../source/socket_connection.c"

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
		string information = "Welcome to chess";
		
		init_server();
		while( !quit )
		{			
			if( SDL_PollEvent( &e )!=0, e.type == SDL_QUIT )
			{
				quit = true;
			}
			else
			{
				if(read(SD,rec_buff,300)!=0){
					string a(rec_buff);
					statusOfInformation = a;
					cout << statusOfInformation << " - " << endl;
				}
				
				// if( e.key.keysym.sym == SDLK_f )
				// {
				// 	information = "Movimento irreconhecivel";
				// 	statusOfInformation = "32";
				// }
				// else if( e.key.keysym.sym == SDLK_q )
				// {
				// 	information = "CHESS D2 TO C3 MOVE";
				// 	statusOfInformation = "33";
				// }
				// else if( e.key.keysym.sym == SDLK_w )
				// {
				// 	information = "MOVING D2 TO C3";
				// 	statusOfInformation = "31";
				// }
				// else if( e.key.keysym.sym == SDLK_e )
				// {
				// 	information = "INVALID MOVE";
				// 	statusOfInformation = "30";
				// } else {
				// 	statusOfInformation = "14";
				// }
				cout << statusOfInformation << " - ";
				players = interface.controlTime(e, statusOfInformation, players, &interface);
			} 
				
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
