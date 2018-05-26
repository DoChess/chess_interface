#include "../include/common.hpp"
#include "../include/player.hpp"
#include "../include/interface.hpp"
//#include <cwchar>
#include "../shared_memory.cpp"

using namespace std;

int main( int argc, char* args[] )
{
  Interface interface;

  if( interface.initInterface() )
  {
    Player lightPlayer, darkPlayer;
    pair<Player, Player> players (lightPlayer, darkPlayer);

    pair<bool, string> endGame (false, "None");

    string information = "Welcome to chess";
    string status_of_information = "None";
    string information_color_background = "None";

    bool quit = false;
    SDL_Event e;

    attach_memory();
    while( !quit )
    {			
      bool el = SDL_PollEvent( &e )!=0;
      if( el, e.type == SDL_QUIT )
      {
        std::cout << "\nWindown closed\n\n";
        quit = true;
      }
      else
      {	
        string a(data);        
        if(a != "None"){
          char teste[5] = "None";
          status_of_information = a;
          strncpy(data, teste, SHM_SIZE);
        }
        if (status_of_information[0] == '1')
        {
          interface.controlTime(status_of_information, &players, &interface);
          status_of_information = "None";
        }
        else if (status_of_information[0] == '3')
        {
          information_color_background = status_of_information.substr(0, 2);
          information = status_of_information.erase(0,3);
        }
      } 

      interface.updateElements( &players );

      interface.isGameOver( &players, &endGame );
      if(endGame.second != "None")
      {
        information = endGame.second;
        interface.controlTime("15", &players, &interface);
        strncpy(data, "15", SHM_SIZE);
      }

      interface.setInformation( information );
      interface.drawBackgroundInterface( information_color_background );

      interface.renderElements();
      SDL_RenderPresent( interface.gRenderer );
    }
  }
  detach_memory();

  return 0;
}
