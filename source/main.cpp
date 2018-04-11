//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include "../include/common.hpp"

using namespace std;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font* gFont = NULL;

//Scene textures
LTexture gDarkTimeTexture;
LTexture gLightTimeTexture;
LTexture gPausePromptTexture;
LTexture gStartPromptTexture;
LTexture gInfoTexture;

bool init();
bool loadMedia();
void close();
LTimer controlTime(SDL_Event e, LTimer timer, SDL_Color textColor);
string formatTime(Uint32 mlseconds);


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
			LTimer timer;

			//In memory text stream
			stringstream timeDark;
			stringstream timeLight;
			string fdsa;

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
						timer = controlTime(e, timer, textColor);
					}
				}

				//Set text to be rendered
				timeDark.str( "" );
				timeLight.str( "" );

				fdsa = formatTime(timer.getTicks());
				timeDark << fdsa;
				timeLight << fdsa; 
				
				//Render text
				if( !gDarkTimeTexture.loadFromRenderedText( timeDark.str().c_str(), textColor, gRenderer, gFont ) )
				{
					printf( "Unable to render time texture!\n" );
				}
				if( !gLightTimeTexture.loadFromRenderedText( timeLight.str().c_str(), textColor, gRenderer, gFont ) )
				{
					printf( "Unable to render time texture!\n" );
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				
				
				//Render textures
				gInfoTexture.render( ( SCREEN_WIDTH - gInfoTexture.getWidth() ) / 2, 0, gRenderer );
				gDarkTimeTexture.render( (SCREEN_WIDTH - gDarkTimeTexture.getWidth()) / 8 , (((SCREEN_HEIGHT - gDarkTimeTexture.getHeight())) / 2) + ((SCREEN_HEIGHT - gDarkTimeTexture.getHeight()) / 4), gRenderer );
				gLightTimeTexture.render( ((SCREEN_WIDTH - gLightTimeTexture.getWidth()) / 2) + ((SCREEN_WIDTH - gLightTimeTexture.getWidth()) / 3) , (((SCREEN_HEIGHT - gLightTimeTexture.getHeight())) / 2) + ((SCREEN_HEIGHT - gLightTimeTexture.getHeight()) / 4), gRenderer );
				
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}


string formatTime(Uint32 mlseconds){
	stringstream t;
	int miliseconds = (int) (mlseconds);
	int seconds = (int) (miliseconds / 1000) % 60 ;
	int minutes = (int) ((miliseconds / (1000*60)) % 60);
	int hours   = (int) ((miliseconds / (1000*60*60)) % 24);
	t << hours << " : " << minutes << " : " <<  seconds;
	return t.str();
}


LTimer controlTime(SDL_Event e, LTimer timer, SDL_Color textColor){
	//Start/stop
	if( e.key.keysym.sym == SDLK_s )
	{
		if( timer.isStarted() )
		{
			timer.stop();
			cout << "O jogo foi parado" << '\n';
			if( !gInfoTexture.loadFromRenderedText( "stoped game", textColor, gRenderer, gFont ) )
			{
				printf( "Unable to render time texture!\n" );
			}
		}
		else
		{
			timer.start();
			cout << "O jogo comecou" << '\n';
			if( !gInfoTexture.loadFromRenderedText( "start game", textColor, gRenderer, gFont ) )
			{
				printf( "Unable to render time texture!\n" );
			}
		}
	}
	//Pause/unpause
	else if( e.key.keysym.sym == SDLK_p )
	{
		if( timer.isPaused() )
		{
			timer.unpause();
			cout << "Jogo foi despausado" << '\n';
			if( !gInfoTexture.loadFromRenderedText( "start game", textColor, gRenderer, gFont ) )
			{
				printf( "Unable to render time texture!\n" );
			}
		}
		else
		{
			timer.pause();
			cout << "Jogo foi pausado" << '\n';
			if( !gInfoTexture.loadFromRenderedText( "paused game", textColor, gRenderer, gFont ) )
			{
				printf( "Unable to render time texture!\n" );
			}
		}
	}
	else if( e.key.keysym.sym == SDLK_c )
	{
		timer.pause();
		cout << "Trocou de tempo" << '\n';
		if( !gInfoTexture.loadFromRenderedText( "Change timer for enemy", textColor, gRenderer, gFont ) )
		{
			printf( "Unable to render time texture!\n" );
		}
	}
	return timer;
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

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont( "../assets/font/lazy.ttf", 28 );
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
		if( !gStartPromptTexture.loadFromRenderedText( "Pausado", textColor, gRenderer, gFont ) )
		{
			printf( "Unable to render start/stop prompt texture!\n" );
			success = false;
		}
		
		//Load pause prompt texture
		if( !gPausePromptTexture.loadFromRenderedText( "Rolando", textColor, gRenderer, gFont ) )
		{
			printf( "Unable to render pause/unpause prompt texture!\n" );
			success = false;
		}

		//Load pause prompt texture
		if( !gInfoTexture.loadFromRenderedText( "Chess", textColor, gRenderer, gFont ) )
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
	gDarkTimeTexture.free();
	gLightTimeTexture.free();
	gStartPromptTexture.free();
	gPausePromptTexture.free();

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
