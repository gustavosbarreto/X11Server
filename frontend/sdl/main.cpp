#ifdef WIN32
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#endif

#include "SDL.h"

/* Function:        main
 * Description:     Entry point.
 */
int main(int argc, char **argv)
{
    /* initialize SDL */
    if (SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        fprintf( stderr, "Video initialization failed: %s\n",
            SDL_GetError( ) );
        SDL_Quit( );
    }

    /* Create the window */
    SDL_Surface * screen = SDL_SetVideoMode(640,480, 32, SDL_DOUBLEBUF);
    if(screen == NULL)
    {
        fprintf(stderr, "Could not create the SDL surface");
        SDL_Quit();
    }

    SDL_Event event;
    while(1)
    {
        SDL_FillRect(screen, NULL, 1000);
        //SDL_BlitSurface(picture, NULL, screen, &pictureLocation);
        SDL_Flip(screen);

        if( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                 case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) 
                    {
                      case SDLK_ESCAPE:
                      {
                        SDL_Quit();
                        exit(-1);
                          return -1;
                      }
                    }
            }
        }



    }
    SDL_Quit( );

    return 0;
}