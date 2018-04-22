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

SDL_Texture* gTexture = NULL;

//Scene sprites
LTexture gSpriteSheetTexture;

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

void drawRet(){
	SDL_Rect fillRect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 };
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );		
	SDL_RenderFillRect( gRenderer, &fillRect );

	SDL_Rect fillRect2 = { 1, SCREEN_HEIGHT / SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT/2};
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );		
	SDL_RenderFillRect( gRenderer, &fillRect2 );
}

void renderTexts(pair<Player, Player> players, Interface* interface){
	SDL_Color textColorBlack = { 0, 0, 0, 255 };
	SDL_Color textColorWhite = { 255, 255, 255, 255 };
	//SDL_Color textColorRed = { 255, 0, 0, 255 };
	
	if( !playerTimeLightTexture.loadFromRenderedText( players.first.timer.showCurrentTime(), textColorBlack, gRenderer, gFontTimer ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerTimeDarkTexture.loadFromRenderedText( players.second.timer.showCurrentTime(), textColorWhite, gRenderer, gFontTimer ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerFailuresDarkTexture.loadFromRenderedText( players.first.getFaults(), textColorWhite, gRenderer, gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerFailuresLightTexture.loadFromRenderedText( players.second.getFaults(), textColorBlack, gRenderer, gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !gStatusGameTexture.loadFromFile( interface->getStatusGame(), gRenderer) )
	{
		printf( "Unable to render start/stop prompt texture!\n" );
	}
	if( !gInfoTexture.loadFromRenderedText( interface->getInformation(), textColorBlack, gRenderer, gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}
}

void renderElements(){
	//Render textures
	gInfoTexture.render( ( SCREEN_WIDTH - gInfoTexture.getWidth() ) / 2, ((SCREEN_HEIGHT - gInfoTexture.getHeight()) / 4), gRenderer );
	playerTimeLightTexture.render( (SCREEN_WIDTH - playerTimeDarkTexture.getWidth()) / 8 , (((SCREEN_HEIGHT - playerTimeDarkTexture.getHeight())) / 2) + ((SCREEN_HEIGHT - playerTimeDarkTexture.getHeight()) / 4), gRenderer );
	playerTimeDarkTexture.render( ((SCREEN_WIDTH - playerTimeLightTexture.getWidth()) / 2) + ((SCREEN_WIDTH - playerTimeLightTexture.getWidth()) / 3) , (((SCREEN_HEIGHT - playerTimeLightTexture.getHeight())) / 2) + ((SCREEN_HEIGHT - playerTimeLightTexture.getHeight()) / 4), gRenderer );
	playerFailuresLightTexture.render( (SCREEN_WIDTH - playerFailuresDarkTexture.getWidth()*4) / 8 , SCREEN_HEIGHT - playerFailuresDarkTexture.getHeight(), gRenderer );
	playerFailuresDarkTexture.render( SCREEN_WIDTH - playerFailuresDarkTexture.getWidth()*4, SCREEN_HEIGHT - playerFailuresDarkTexture.getHeight(), gRenderer );

	gStatusGameTexture.render( (SCREEN_WIDTH - gStatusGameTexture.getWidth()) / 2, (SCREEN_HEIGHT - gStatusGameTexture.getHeight()) / 2, gRenderer );
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
			bool quit = false;
			SDL_Event e;
		
			Player lightPlayer, darkPlayer;
			Interface interface;

			pair<Player, Player> players (lightPlayer, darkPlayer);

			while( !quit )
			{
				//Handle events on queue
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
				renderTexts(players, &interface);
				
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				drawRet();
				renderElements();

				SDL_RenderPresent( gRenderer );
			}
		}
	}

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
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

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


bool loadMedia()
{
	bool success = true;

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

		//Load pause prompt texture
		if( !gInfoTexture.loadFromRenderedText( "Chess Game", textColor, gRenderer, gFont ) )
		{
			printf( "Unable to render information texture!\n" );
			success = false;
		}
		if( !gSpriteSheetTexture.loadFromFile( "../assets/imgs/kings.png", gRenderer ) )
		{
			printf( "Failed to load sprite sheet texture!\n" );
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

	gSpriteSheetTexture.free();

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
