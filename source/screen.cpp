#include "../include/screen.hpp"
#include "../include/common.hpp"

Screen::Screen(){
    gWindow = NULL;
    gRenderer = NULL;
    gFont = NULL;
    gFontTimer = NULL;
    gTexture = NULL;
}

bool Screen::init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		this->gWindow = SDL_CreateWindow( "Chess Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( this->gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			this->gRenderer = SDL_CreateRenderer( this->gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( this->gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( this->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

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


bool Screen::loadMedias()
{
	bool success = true;

	this->gFontTimer = TTF_OpenFont( "../assets/font/lazy.ttf", 70 );
	this->gFont = TTF_OpenFont( "../assets/font/lemon.ttf", 30 );
	
	if( this->gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//SDL_Color textColor = { 0, 0, 0, 255 };
        
        /*
		if( !gInfoTexture.loadFromRenderedText( "Chess Game", textColor, this->gRenderer, this->gFont ) )
		{
			printf( "Unable to render information texture!\n" );
			success = false;
		}
		if( !gSpriteSheetTexture.loadFromFile( "../assets/imgs/kings.png", gRenderer ) )
		{
			printf( "Failed to load sprite sheet texture!\n" );
			success = false;
		}
        */
	}

	return success;
}

void Screen::close()
{
    playerTimeDarkTexture.free();
	playerTimeLightTexture.free();
	gStatusGameTexture.free();

	gSpriteSheetTexture.free();


	SDL_DestroyTexture( this->gTexture );
    this->gTexture = NULL;

	TTF_CloseFont( this->gFont );
	this->gFont = NULL;

	TTF_CloseFont( this->gFontTimer );
	this->gFontTimer = NULL;

	SDL_DestroyRenderer( this->gRenderer );
	this->gRenderer = NULL;

	SDL_DestroyWindow( this->gWindow );
	this->gWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Screen::drawRet(){
	int colors[4] = {0x00, 0x00, 0x00, 0x00};
	SDL_Rect fillRect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 };
	SDL_SetRenderDrawColor( this->gRenderer, colors[0], colors[1], colors[2], colors[3] );		
	SDL_RenderFillRect( this->gRenderer, &fillRect );
	
	SDL_Rect fillRect2 = { 1, SCREEN_HEIGHT / SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT/2};
	SDL_SetRenderDrawColor( this->gRenderer, 0xFF, 0x00, 0x00, 0xFF );		
	SDL_RenderFillRect( this->gRenderer, &fillRect2 );
}

void Screen::renderTexts(pair<Player, Player> players, Interface* interface){
	SDL_Color textColorBlack = { 0, 0, 0, 255 };
	SDL_Color textColorWhite = { 255, 255, 255, 255 };
	//SDL_Color textColorRed = { 255, 0, 0, 255 };
	
	if( !playerTimeLightTexture.loadFromRenderedText( players.first.timer.showCurrentTime(), textColorBlack, this->gRenderer, this->gFontTimer ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerTimeDarkTexture.loadFromRenderedText( players.second.timer.showCurrentTime(), textColorWhite, this->gRenderer, this->gFontTimer ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerFailuresDarkTexture.loadFromRenderedText( players.first.getFaults(), textColorWhite, this->gRenderer, this->gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerFailuresLightTexture.loadFromRenderedText( players.second.getFaults(), textColorBlack, this->gRenderer, this->gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !gInfoTexture.loadFromRenderedText( interface->getInformation(), textColorBlack, this->gRenderer, this->gFont ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !gStatusGameTexture.loadFromFile( interface->getStatusGame(), this->gRenderer) )
	{
		printf( "Unable to render start/stop prompt texture!\n" );
	}
}

void Screen::renderElements(){
	gInfoTexture.render( ( SCREEN_WIDTH - gInfoTexture.getWidth() ) / 2, ((SCREEN_HEIGHT - gInfoTexture.getHeight()) / 4), this->gRenderer );
	playerTimeLightTexture.render( (SCREEN_WIDTH - playerTimeDarkTexture.getWidth()) / 8 , (((SCREEN_HEIGHT - playerTimeDarkTexture.getHeight())) / 2) + ((SCREEN_HEIGHT - playerTimeDarkTexture.getHeight()) / 4), this->gRenderer );
	playerTimeDarkTexture.render( ((SCREEN_WIDTH - playerTimeLightTexture.getWidth()) / 2) + ((SCREEN_WIDTH - playerTimeLightTexture.getWidth()) / 3) , (((SCREEN_HEIGHT - playerTimeLightTexture.getHeight())) / 2) + ((SCREEN_HEIGHT - playerTimeLightTexture.getHeight()) / 4), this->gRenderer );
	playerFailuresLightTexture.render( (SCREEN_WIDTH - playerFailuresDarkTexture.getWidth()*4) / 8 , SCREEN_HEIGHT - playerFailuresDarkTexture.getHeight(), this->gRenderer );
	playerFailuresDarkTexture.render( SCREEN_WIDTH - playerFailuresDarkTexture.getWidth()*4, SCREEN_HEIGHT - playerFailuresDarkTexture.getHeight(), this->gRenderer );

	gStatusGameTexture.render( (SCREEN_WIDTH - gStatusGameTexture.getWidth()) / 2, (SCREEN_HEIGHT - gStatusGameTexture.getHeight()) / 2, this->gRenderer );
}
