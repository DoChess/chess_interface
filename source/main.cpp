#include "../include/common.hpp"
#include "../include/player.hpp"
#include "../include/interface.hpp"
#include "../shared_memory.cpp"

using namespace std;

int main( int argc, char* args[] )
{
    Interface interface;
    short qtd_matches = 0;
    string initial_information = "Welcome to chess";
    string information = initial_information;
    initial_information = information;

    if( interface.initInterface() )
    {
        while(qtd_matches<1){
            Player lightPlayer, darkPlayer;
            pair<Player, Player> players (lightPlayer, darkPlayer);

            pair<bool, string> endGame (false, "None");

            string status_of_information = "None";
            string information_color_background = "None";

            bool quit = false;
            SDL_Event e;

            attach_memory();
            while( !quit )
            {
                if( SDL_PollEvent( &e )!=0 && e.type == SDL_QUIT )
                {
                    std::cout << "\nWindown closed\n\n";
                    quit = true;
                }
                else
                {
                    string data_of_control_package(data);
                    if(data_of_control_package != "None"){
                        char data_write[5] = "None";
                        status_of_information = data_of_control_package;
                        strncpy(data, data_write, SHM_SIZE);

                        if (status_of_information[0] == '1')
                        {
                            interface.controlTime(status_of_information, &players, &interface);
                        }
                        else if (status_of_information[0] == '3')
                        {
                            if (status_of_information[1] == '0'){
                                interface.isLightCurrentPlayer() ? players.first.setFault() : players.second.setFault();
                            }
                            information_color_background = status_of_information.substr(0, 2);
                            information = status_of_information.erase(0,3);
                        }
                    }

                }

                interface.updateElements( &players );

                interface.isGameOver( &players, &endGame );
                if(endGame.second != "None")
                {
                    information = endGame.second;
                    interface.controlTime("15", &players, &interface);
                    strncpy(data, "15", SHM_SIZE);
                    endGame.second = "None";
                    break;
                }

                interface.setInformation( information );
                interface.drawBackgroundInterface( information_color_background );

                interface.renderElements();
                SDL_RenderPresent( interface.gRenderer );
            }

            qtd_matches++;
            cout << "Next game" << endl;
            lightPlayer.~Player();
            darkPlayer.~Player();
        }
    }
    detach_memory();

    return 0;
}
