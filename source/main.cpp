#include "../include/common.hpp"
#include "../include/player.hpp"
#include "../include/interface.hpp"
#include "../source/socket_connection.c"

using namespace std;

string get_status_of_information(SDL_Event e, string status_of_information){
    if( e.key.keysym.sym == SDLK_s )
    {
       status_of_information = "11";
    }
    else if( e.key.keysym.sym == SDLK_p )
    {
       status_of_information =  status_of_information == "12" ? "12" : "13";
    }
    else if( e.key.keysym.sym == SDLK_c )
    {
       status_of_information = "14";
    }
    return status_of_information;
}

int main( int argc, char* args[] )
{
	Interface interface;
	
	if( interface.initInterface() )
	{
		bool quit = false;
		SDL_Event e;
	
		Player lightPlayer, darkPlayer;

		pair<Player, Player> players (lightPlayer, darkPlayer);
        string information = "";
		string status_of_information = "2Welcome to chess";
        string status_of_information_color_background = "";
		
		//init_server();
		while( !quit )
		{			
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
			else
			{
				/*
				if(read(SD,rec_buff,300)!=0){
					string a(rec_buff);
					status_of_information = a;
					cout << status_of_information << " - " << endl;
				}*/
				while( SDL_PollEvent( &e ) != 0 ){
					status_of_information = get_status_of_information(e, status_of_information);
					if (status_of_information[0] == '3')
					{
						status_of_information_color_background = status_of_information;
					}
					else if (status_of_information[0] == '2')
					{
						information = status_of_information.erase(0,1);
					}
					else if (status_of_information[0]=='1')
					{
						players = interface.controlTime(status_of_information, players, &interface);	
					}
				}
			} 
				
			interface.updateElements( players );
			
			if(players.first.lostGamePerFault() or players.second.lostGamePerFault()){
				information = "Player lost per fault";
			} else if (players.first.lostGamePerTime() or players.second.lostGamePerTime()){
				information = "Player lost per time";
			}

			interface.setInformation( information );
			interface.drawBackgroundInterface( status_of_information );
			
			interface.renderElements();
			SDL_RenderPresent( interface.gRenderer );
		}
	}
	interface.close();

	return 0;
}
