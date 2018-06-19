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
  string first_information = "-";
  string second_information = "-";
  initial_information = information;


  if( interface.initInterface() )
  {
    while(qtd_matches<3){
      Player lightPlayer, darkPlayer;
      pair<Player, Player> players (lightPlayer, darkPlayer);

      pair<bool, string> endGame (false, "None");

      string status_of_information = "None";
      string information_color_background = "None";
      string player = "White";
      bool start_game = false;

      interface.setLightCurrentPlayer(true);

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

            char none[5] = "None";
            if (status_of_information[0] == '1' and status_of_information!="15")
            {
              if(status_of_information == "11"){
                start_game = true;
                first_information = ".";
                information = string("Waiting for the ");
                second_information = getCurrentPlayer( interface.isLightCurrentPlayer()) + "  player\'s command";
              }
              else if (status_of_information =="17") {
                information = getCurrentPlayer(interface.isLightCurrentPlayer()) + string(" player victory");
                first_information = "-";
                second_information = "Checkmate";
                interface.controlTime("15", &players, &interface);
                endGame.second = "None";
                strncpy(data, none, SHM_SIZE);
                break;
              }
              interface.controlTime(status_of_information, &players, &interface);
              status_of_information = "None";
            }
            else if (status_of_information[0] == '3' and start_game == true)
            {
              information = ".";

              information_color_background = status_of_information.substr(0, 2);
              if (status_of_information[1] == '0')
              {
                interface.isLightCurrentPlayer() ? players.first.setFault() : players.second.setFault();
                first_information = getCurrentPlayer(interface.isLightCurrentPlayer()) + string(" player the command: ");
                information = status_of_information.erase(0,3);
                second_information = " is a Invalid Movement";
              }
              else if (status_of_information[1] == '1')
              {
                first_information = getCurrentPlayer(interface.isLightCurrentPlayer()) + string(" player the command: ");
                information = status_of_information.erase(0,3);
                second_information = string(" is running");
              }
              else if (status_of_information[1] == '4')
              {
                first_information = getCurrentPlayer(interface.isLightCurrentPlayer()) + string(" player the command: ");
                information = status_of_information.erase(0,3);
                second_information = " \"Move\" or \"Repeat\"?";
              }
              else if (status_of_information[1] == '5'){
                information = string("Waiting for the ");
                second_information = getCurrentPlayer( interface.isLightCurrentPlayer()) + "  player\'s command";
                first_information = ".";
              }
              else if (status_of_information[1] == '3'){
                information = string("Listening to the command of the ");
                second_information = getCurrentPlayer( interface.isLightCurrentPlayer()) +" player";
                first_information = ".";
              }
            }
            if(status_of_information != "15")
              strncpy(data, none, SHM_SIZE);
          }
        }

        interface.updateElements( &players );

        interface.isGameOver( &players, &endGame );
        if(endGame.second != "None")
        {
          information = endGame.second;
          first_information = "-";
          second_information = "-";
          interface.controlTime("15", &players, &interface);
          strncpy(data, "15", SHM_SIZE);
          endGame.second = "None";
          break;
        }

        interface.setInformation( information );
        interface.setSecondInformation( second_information );
        interface.setFirstInformation( first_information );
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
