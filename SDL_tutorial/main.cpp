#include "SDL.h"
#include "SDL_image.h"
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int CLIP_MOUSEOVER = 0;
const int CLIP_UNHIT = 1;
const int CLIP_WATER_HIT = 2;
const int CLIP_SHIP_HIT = 3;

SDL_Surface *buttonSheet = NULL;
SDL_Surface *screen = NULL;

SDL_Event event;

SDL_Rect clips[4];

class Button
{
    private:
    SDL_Rect box;

    SDL_Rect* clip;

	//original state of the button
	//states
	//1 = water
	//2 = water hit
	//3 = ship
	//4 = ship hit
	int state;



    public:
    //Initialize the variables
    Button( int x, int y, int w, int h , int state);

    //Handles events and set the button's sprite region
    void handle_events();

    //Shows the button on the screen
    void show();

	int getState()
	{
		return state;
	};

	void writeState(int s)
	{
		state = s;
	};
};

SDL_Surface *load_image(std::string filename)
{
	SDL_Surface*loadedImage = NULL;

	SDL_Surface* optimizedImage = NULL;

	loadedImage = IMG_Load(filename.c_str());

	if (loadedImage!=NULL)
	{
		optimizedImage = SDL_DisplayFormat( loadedImage );

      
        SDL_FreeSurface( loadedImage );

              if( optimizedImage != NULL )
        {
       
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
	}

	return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Button Test", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the button sprite sheet
    buttonSheet = load_image( "Untitled.png" );

    //If there was a problem in loading the button sprite sheet
    if( buttonSheet == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    SDL_FreeSurface( buttonSheet );

    SDL_Quit();
}

void set_clips()
{
    //green
    clips[ CLIP_MOUSEOVER ].x = 640;
    clips[ CLIP_MOUSEOVER ].y = 0;
    clips[ CLIP_MOUSEOVER ].w = 320;
    clips[ CLIP_MOUSEOVER ].h = 240;

    //blue
	clips[ CLIP_UNHIT ].x = 320;
    clips[ CLIP_UNHIT ].y = 240;
    clips[ CLIP_UNHIT ].w = 320;
    clips[ CLIP_UNHIT ].h = 240;

	//black
    clips[ CLIP_WATER_HIT].x = 0;
    clips[ CLIP_WATER_HIT ].y = 0;
    clips[ CLIP_WATER_HIT ].w = 320;
    clips[ CLIP_WATER_HIT ].h = 240;

	//red
  	clips[ CLIP_SHIP_HIT ].x = 320;
    clips[ CLIP_SHIP_HIT ].y = 0;
    clips[ CLIP_SHIP_HIT ].w = 320;
    clips[ CLIP_SHIP_HIT ].h = 240;

}

Button::Button( int x, int y, int w, int h, int s)
{
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;

	state = s;

	clip = &clips[CLIP_UNHIT];
}

void Button::handle_events()
{
    //The mouse offsets
    int x = 0, y = 0;

    //If the mouse moved
    if( event.type == SDL_MOUSEMOTION )
    {
        //Get the mouse offsets
        x = event.motion.x;
        y = event.motion.y;

        //If the mouse is over the button
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            //Set the button sprite
            clip = &clips[ CLIP_MOUSEOVER ];
        }
        //If not
        else
        {
			if (state ==1)
			{
				clip = &clips[ CLIP_UNHIT ];
			}
			if (state ==2)
			{
				clip = &clips[ CLIP_WATER_HIT ];
			}
			if (state ==3)
			{
				clip = &clips[ CLIP_UNHIT ];
			}
			if (state ==4)
			{
				clip = &clips[ CLIP_SHIP_HIT ];
			}
            
        }
    }
    //If a mouse button was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
				if (state ==1)
				{
					state = 2;
					clip = &clips[ CLIP_WATER_HIT ];
				}
				if (state ==3)
				{
					state = 4;
					clip = &clips[ CLIP_SHIP_HIT ];
				}
            }
        }
    }
    //If a mouse button was released
    if( event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set the button sprite
                clip = &clips[ CLIP_WATER_HIT ];
            }
        }
    }
}

void Button::show()
{
    //Show the button
    apply_surface( box.x, box.y, buttonSheet, screen, clip );
}



int main (int argc, char* args[])
{
	bool quit = false;

	if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }
	set_clips();

	Button one( 0, 0, 320, 240, 1);
	Button two( 0, 240, 320, 240, 1);
	Button three( 320, 0, 320, 240, 3);
	Button four( 320, 240, 320, 240, 3);
	

	
    while( quit == false )
    {
        //If there's events to handle
        if( SDL_PollEvent( &event ) )
        {
			 one.handle_events();
			 two.handle_events();
			 three.handle_events();
			 four.handle_events();
           

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }


       
        //Show the button
        one.show();
		two.show();
		three.show();
		four.show();
		

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    //Clean up
    clean_up();

	

	return 0;
}