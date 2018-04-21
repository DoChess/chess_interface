//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include "../include/common.hpp"
#include "../include/player.hpp"
#include "../include/interface.hpp"

using namespace std;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font* gFont = NULL;
TTF_Font* gFontTimer = NULL;

SDL_Texture* loadTexture( std::string path );
SDL_Texture* gTexture = NULL;

//Scene textures
LTexture playerTimeDarkTexture;
LTexture playerTimeLightTexture;
LTexture playerFailuresDarkTexture;
LTexture playerFailuresLightTexture;
LTexture gStatusGameTexture;
LTexture gInfoTexture;

bool init();
bool loadMedia();
void close();

void renderTexts(pair<Player, Player> players, Interface* interface ,SDL_Color textColor){
	if( !playerTimeDarkTexture.loadFromRenderedText( players.first.timer.showCurrentTime(), textColor, gRenderer, gFontTimer ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerTimeLightTexture.loadFromRenderedText( players.second.timer.showCurrentTime(), textColor, gRenderer, gFontTimer ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerFailuresDarkTexture.loadFromRenderedText( players.first.getFaults(), textColor, gRenderer, gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerFailuresLightTexture.loadFromRenderedText( players.second.getFaults(), textColor, gRenderer, gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !gStatusGameTexture.loadFromRenderedText( interface->getStatusGame(), textColor, gRenderer, gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !gInfoTexture.loadFromRenderedText( interface->getInformation(), textColor, gRenderer, gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}
}

void renderElements(){
	//Render textures
	gInfoTexture.render( ( SCREEN_WIDTH - gInfoTexture.getWidth() ) / 2, ((SCREEN_HEIGHT - gInfoTexture.getHeight()) / 4), gRenderer );
	gStatusGameTexture.render( (SCREEN_WIDTH - gStatusGameTexture.getWidth()) / 2, (SCREEN_HEIGHT - gStatusGameTexture.getHeight()) / 2, gRenderer );
	playerTimeDarkTexture.render( (SCREEN_WIDTH - playerTimeDarkTexture.getWidth()) / 8 , (((SCREEN_HEIGHT - playerTimeDarkTexture.getHeight())) / 2) + ((SCREEN_HEIGHT - playerTimeDarkTexture.getHeight()) / 4), gRenderer );
	playerTimeLightTexture.render( ((SCREEN_WIDTH - playerTimeLightTexture.getWidth()) / 2) + ((SCREEN_WIDTH - playerTimeLightTexture.getWidth()) / 3) , (((SCREEN_HEIGHT - playerTimeLightTexture.getHeight())) / 2) + ((SCREEN_HEIGHT - playerTimeLightTexture.getHeight()) / 4), gRenderer );
	playerFailuresDarkTexture.render( (SCREEN_WIDTH - playerFailuresDarkTexture.getWidth()) / 8 , SCREEN_HEIGHT - playerFailuresDarkTexture.getWidth(), gRenderer );
	playerFailuresLightTexture.render( SCREEN_WIDTH - playerFailuresDarkTexture.getWidth(), SCREEN_HEIGHT - playerFailuresDarkTexture.getWidth(), gRenderer );

}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize graphic interface!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 255 };

			//The application timer
			Player lightPlayer, darkPlayer;
			Interface interface;

			//In memory text stream
			string light, dark;
			pair<Player, Player> players (lightPlayer, darkPlayer);


			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					//Reset start time on return keypress
					else if( e.type == SDL_KEYDOWN )
					{
						players = interface.controlTime(e, players, textColor, &interface);
						lightPlayer = players.first;
						darkPlayer = players.second;
					} 
				}
				renderTexts(players, &interface, textColor);
				
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				renderElements();
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

SDL_Texture* loadTexture( std::string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFontTimer = TTF_OpenFont( "../assets/font/lazy.ttf", 70 );
	gFont = TTF_OpenFont( "../assets/font/lemon.ttf", 30 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Set text color as black
		SDL_Color textColor = { 0, 0, 0, 255 };

		//Load stop prompt texture
		if( !gStatusGameTexture.loadFromRenderedText( "stop", textColor, gRenderer, gFont ) )
		{
			printf( "Unable to render start/stop prompt texture!\n" );
			success = false;
		}

		gTexture = loadTexture( "../assets/imgs/black_king.png" );
		if( gTexture == NULL )
		{
			printf( "Failed to load texture image!\n" );
			success = false;
		}

		//Load pause prompt texture
		if( !gInfoTexture.loadFromRenderedText( "Chess Game", textColor, gRenderer, gFont ) )
		{
			printf( "Unable to render information texture!\n" );
			success = false;
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	playerTimeDarkTexture.free();
	playerTimeLightTexture.free();
	gStatusGameTexture.free();

	SDL_DestroyTexture( gTexture );
    gTexture = NULL;

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
