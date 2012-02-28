#include "SDL.h"
#include "SDL_image.h"
#include <string>

	const int SCREEN_WIDTH = 640 ;
	const int SCREEN_HEIGHT = 480 ; 
	const int SCREEN_BPP = 32 ;

	SDL_Surface *bg = NULL;
	SDL_Surface *foo = NULL;
	SDL_Surface *screen = NULL;

	SDL_Event event;
	
	SDL_Surface *load_image (std::string filename)
	{
		SDL_Surface* loadedImage = NULL;

		SDL_Surface* optimizedImage = NULL;

		loadedImage = IMG_Load(filename.c_str());

		if(loadedImage!= NULL)
		{
			optimizedImage = SDL_DisplayFormat(loadedImage);

			SDL_FreeSurface(loadedImage);
		}

		if(optimizedImage !=NULL)
		{
			Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF);
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);

		}
		return optimizedImage;
	}

	void apply_surface (int x, int y, SDL_Surface* source, SDL_Surface* destination)
	{
		SDL_Rect offset;

		offset.x = x;
		offset.y = y;

		SDL_BlitSurface( source, NULL, destination, &offset);
	}

	bool init()
	{
		if(SDL_Init (SDL_INIT_EVERYTHING) ==-1)
		{
			return 1;
		}

		screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

		if (screen == NULL)
		{
			return 1;
		}

		SDL_WM_SetCaption("Event Test", NULL);

		return true;
	}

	bool load_files()
	{
		bg = load_image ("bg.bmp");

		if(bg == NULL)
		{
			return false;
		}

		foo = load_image ("foo.png");

		if(foo == NULL)
		{
			return false;
		}

		return true;

	}

	void clean_up()
	{
		SDL_FreeSurface (bg);
		SDL_FreeSurface (foo);
		SDL_Quit();
	}

int main (int argc, char* args[])
{

	bool quit = false;

	if (init() == false)
	{
		return 1;
	}

	if (load_files () ==false)
	{
		return 1;
	}

	apply_surface(0, 0, bg, screen);
	apply_surface(0, 0, foo, screen);

	if (SDL_Flip(screen) == -1)
	{
		return 1;
	}

	while(quit == false)
	{
		while(SDL_PollEvent (&event))
		{
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

	}



	clean_up();

	return 0;

}
