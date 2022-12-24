#include "print.h"
#include "thrower.h"

#include <string>
using namespace std::literals;

#include <SDL2/SDL.h>

#if _DEBUG
#pragma comment(lib,"SDL2d")
#pragma comment(lib,"SDL2maind")
#else
#pragma comment(lib,"SDL2")
#pragma comment(lib,"SDL2main")
#endif

//https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index2.php


extern "C"
int _cdecl SDL_main(int argc, char *argv[])
try
{
    constexpr int windowWidth{600};
    constexpr int windowHeight{400};

    auto err = SDL_Init( SDL_INIT_VIDEO ) ;

    if(err < 0)
    {
        throw_runtime_error("SDL_Init : "s + SDL_GetError());
    }

    auto window = SDL_CreateWindow( "SDLTest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

    if( window == nullptr )
    {
        throw_runtime_error("SDL_CreateWindow : "s + SDL_GetError());
    }

    auto renderer   = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED  | SDL_RENDERER_PRESENTVSYNC );


    constexpr auto ballSize=10;

    auto ballX  = windowWidth/2;
    auto ballY  = windowWidth/2;

    auto balldX = 2;
    auto balldY = 2;



    bool done = false; 
    int  rgb{};
    while( !done)
    { 
        SDL_Event e{};
        
        while( SDL_PollEvent( &e ) )
        { 
//          print("event : {}\n",e.type);
            if( e.type == SDL_QUIT ) 
            {
                done = true;
            }
        }

		SDL_SetRenderDrawColor( renderer, rgb%256, rgb%256, rgb%256, 0xFF );
		SDL_RenderClear( renderer );


        ballX+=balldX;
        ballY+=balldY;

        if(ballX < 0 || ballX+ballSize > windowWidth)
        {
            balldX=-balldX;
            rgb++;
        }

        if(ballY < 0 || ballY+ballSize > windowHeight)
        {
            balldY=-balldY;
            rgb++;
        }

        SDL_Rect ball = { ballX , ballY, ballSize, ballSize};


        SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
        SDL_RenderFillRect( renderer, &ball );

        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderDrawPoint(renderer, ballX+ballSize/2, ballY+ballSize/2);       // SDL_RenderDrawPoints , SDL_DrawLines


		SDL_RenderPresent( renderer );      // synced with vsync

    }

	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow  (window);
    SDL_Quit();

    return 0;
}
catch(std::exception const &e)
{
    print("{}",e.what());
    return 1;
}


