//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include "../include/common.hpp"

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
LTexture gDarkTimeTexture;
LTexture gLightTimeTexture;
LTexture gStatusGameTexture;
LTexture gInfoTexture;

bool init();
bool loadMedia();
void close();
pair<LTimer, LTimer> controlTime(SDL_Event e, pair<LTimer, LTimer> gameTime, SDL_Color textColor);
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
			LTimer timerDark, timerLight;

			//In memory text stream
			stringstream currentTimeDark, currentTimeLight;
			string light, dark;
			pair<LTimer, LTimer> gameTime (timerLight, timerDark);


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
						gameTime = controlTime(e, gameTime, textColor);
						timerLight = gameTime.first;
						timerDark = gameTime.second;
					}
				}

				//Set text to be rendered
				currentTimeDark.str( "" );
				currentTimeLight.str( "" );

				light = formatTime(gameTime.first.getTicks());
				dark = formatTime(gameTime.second.getTicks());

				currentTimeLight << light;
				currentTimeDark << dark;

				//Render text
				if( !gDarkTimeTexture.loadFromRenderedText( currentTimeDark.str().c_str(), textColor, gRenderer, gFontTimer ) )
				{
					printf( "Unable to render time texture!\n" );
				}
				if( !gLightTimeTexture.loadFromRenderedText( currentTimeLight.str().c_str(), textColor, gRenderer, gFontTimer ) )
				{
					printf( "Unable to render time texture!\n" );
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render textures
				gInfoTexture.render( ( SCREEN_WIDTH - gInfoTexture.getWidth() ) / 2, ((SCREEN_HEIGHT - gInfoTexture.getHeight()) / 4), gRenderer );
				gStatusGameTexture.render( (SCREEN_WIDTH - gStatusGameTexture.getWidth()) / 2, (SCREEN_HEIGHT - gStatusGameTexture.getHeight()) / 2, gRenderer );
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

//Colocar essa função dentro da classe timer
string formatTime(Uint32 mlseconds){
	stringstream t;
	int miliseconds = (int) (mlseconds);
	int seconds = (int) (miliseconds / 1000) % 60;
	int minutes = (int) ((miliseconds / (1000*60)) % 60);
	int hours   = (int) ((miliseconds / (1000*60*60)) % 24);
	t << setfill('0') << setw(2) << HOURS - hours << ":" << setfill('0') << setw(2) << MINUTES - minutes << ":" << setfill('0') << setw(2) << SECONDS - seconds;
	return t.str();
}


pair<LTimer, LTimer> controlTime(SDL_Event e, pair<LTimer, LTimer> gameTime, SDL_Color textColor){
	string currentTime = "";
	string statusGameText = "";
	if( e.key.keysym.sym == SDLK_s )
	{
		if( gameTime.second.isStarted() and gameTime.first.isStarted())
		{
			gameTime.second.stop();
			gameTime.first.stop();
			statusGameText = "stoped game";
		}
		else
		{
			gameTime.first.start();
			gameTime.second.start();
			gameTime.first.pause();
			statusGameText = "start game";
		}
		currentTime = "";
	}
	//Pause/unpause
	else if( e.key.keysym.sym == SDLK_p )
	{
		if( gameTime.second.isPaused() and gameTime.second.isPaused() )
		{
			gameTime.second.unpause();
			gameTime.first.unpause();
			statusGameText = "restart game";
		}
		else
		{
			gameTime.first.pause();
			gameTime.second.pause();
			statusGameText = "paused game";
		}
		currentTime = "";
	}
	else if( e.key.keysym.sym == SDLK_c )
	{

		if (gameTime.second.isPaused())
		{
			gameTime.second.unpause();
			gameTime.first.pause();
			currentTime = "light";
		} else {
			gameTime.second.pause();
			gameTime.first.unpause();
			currentTime = "dark";
		}
		statusGameText = "Time of ";

	}
	if( !gInfoTexture.loadFromRenderedText( "chess b1 for c3 move", textColor, gRenderer, gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !gStatusGameTexture.loadFromRenderedText( statusGameText + currentTime, textColor, gRenderer, gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}

	return gameTime;
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
