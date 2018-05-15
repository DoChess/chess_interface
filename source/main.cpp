#include "../include/common.hpp"
#include "../include/player.hpp"
#include "../include/interface.hpp"
#include "../shared_memory.cpp"

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
  else if( e.key.keysym.sym == SDLK_f )
  {
    status_of_information = "32";
  }
  else if( e.key.keysym.sym == SDLK_q )
  {
    status_of_information = "33";
  }
  else if( e.key.keysym.sym == SDLK_w )
  {
    status_of_information = "31";
  }
  else if( e.key.keysym.sym == SDLK_e )
  {
    status_of_information = "30";
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
    pair<bool, string> endGame (false, "");

    string information = "Welcome to chess";
    string status_of_information = "None";
    string status_of_information_color_background = "None";

    attach_memory();
    while( !quit )
    {			
      bool el = SDL_PollEvent( &e )!=0;
      if( el, e.type == SDL_QUIT )
      {
        std::cout << "\nSaiu no quit" << '\n';
        quit = true;
      }
      else
      {	
        string a(data);        
        if(a != "None"){
          status_of_information = a;
          strncpy(data, "None", SHM_SIZE);
        }

        //while( SDL_PollEvent( &e ) != 0){
        //status_of_information = get_status_of_information(e, status_of_information);
        if (status_of_information[0]=='1')
        {
          players = interface.controlTime(status_of_information, players, &interface);
          status_of_information = "None";
        }
        else if (status_of_information[0] == '3')
        {
          status_of_information_color_background = status_of_information.substr(0, 2);
          cout << status_of_information_color_background << endl;
          information = status_of_information.erase(0,3);
        }
        //}
      } 

      interface.updateElements( players );

      endGame = interface.isGameOver(players);
      if(endGame.second != "")
      {
        information = endGame.second;
        players = interface.controlTime("15", players, &interface);
        strncpy(data, "15", SHM_SIZE);
      }

      interface.setInformation( information );
      interface.drawBackgroundInterface( status_of_information_color_background );

      interface.renderElements();
      SDL_RenderPresent( interface.gRenderer );
    }
  }
  detach_memory();
  interface.close();

  return 0;
}
