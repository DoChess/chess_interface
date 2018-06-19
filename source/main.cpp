#include "../include/common.hpp"
#include "../include/player.hpp"
#include "../include/interface.hpp"
#include "../shared_memory.cpp"

using namespace std;
string getCurrentPlayer(bool playerLight){
    string player = "Dark";
    if (playerLight)
        player = "White";
    return player;
}

int main( int argc, char* args[] )
{
    Interface interface;
    short qtd_matches = 0;
    string initial_information = "Welcome to chess game";
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
            string player = "White";
            string aux_information = "";

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
                        status_of_information = data_of_control_package;
                        if (status_of_information[0] == '1')
                        {
                            interface.controlTime(status_of_information, &players, &interface);
                            status_of_information = "None";
                        }
                        else if (status_of_information[0] == '3')
                        {
                            aux_information = "";
                            information = "";

                            information_color_background = status_of_information.substr(0, 2);
                            //status_of_information.erase(0,3);
                            if (status_of_information[1] == '0')
                            {
                                interface.isLightCurrentPlayer() ? players.first.setFault() : players.second.setFault();
                                information = getCurrentPlayer(interface.isLightCurrentPlayer()) + string(" player the command: ") + status_of_information.erase(0,3);
                                aux_information = " is a Invalid Movement - Please reapet command";
                            }
                            else if (status_of_information[1] == '1')
                            {
                                information = getCurrentPlayer(interface.isLightCurrentPlayer()) + string(" player the command: " + status_of_information.erase(0,3));
                                aux_information = string(" is running");
                            }
                            else if (status_of_information[1] == '4')
                            {
                                information = getCurrentPlayer(interface.isLightCurrentPlayer()) + string(" player the command: " + status_of_information.erase(0,3));
                                aux_information = " \"Move\" to execute or \"Cancel\" to repeat command";
                            }
                            else if (status_of_information[1] == '5'){
                                information = string("Waiting for the ") + player + "  player\'s command";
                                aux_information = "";
                            }
                            else if (status_of_information[1] == '3'){
                                information = string("Listening for the ") + player +" player\'s command";
                                aux_information = "";
                            }
                            information += aux_information;
                        }
                        char write_data[5] = "None";
                        strncpy(data, write_data, SHM_SIZE);
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
                interface.setSecondInformation( "information" );
                interface.setFirstInformation( "fdsafdsafdsafdsainformation" );
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
