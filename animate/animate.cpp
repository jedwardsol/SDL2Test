#include "include/print.h"
#include "include/thrower.h"
#include "include/sdl++.h"





constexpr int windowWidth{600};
constexpr int windowHeight{400};

constexpr int boardWidth{1000};
constexpr int boardHeight{1000};




extern "C"
int _cdecl SDL_main(int argc, char *argv[])
try
{
    SDL         sdl;
    SDLWindow   window{"Animate",windowWidth, windowHeight};
    SDLRenderer renderer{window};

    SDL_RenderSetLogicalSize(renderer,boardWidth, boardHeight);

    constexpr auto ballSize=20;

    auto ballX  = 5+boardWidth/2.0;
    auto ballY  = boardHeight/2.0;

    auto balldX = 9;
    auto balldY = 9.5;

    SDL_Point   frame[]{ {0,0}, {boardWidth-1,0}, {boardWidth-1,boardHeight-1}, {0,boardHeight-1}, {0,0}};

    bool done = false; 
    int  rgb{32};
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

        if(ballX < 0 || ballX+ballSize > boardWidth)
        {
            balldX=-balldX;
            rgb++;
        }

        if(ballY < 0 || ballY+ballSize > boardHeight)
        {
            balldY=-balldY;
            rgb++;
        }

        SDL_Rect ball( (int)ballX , (int)ballY, ballSize, ballSize);


        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderDrawLines(renderer,frame,5);


        SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
        SDL_RenderFillRect( renderer, &ball );

        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderDrawPoint(renderer, (int)(ballX+ballSize/2), (int)(ballY+ballSize/2));       // SDL_RenderDrawPoints , SDL_DrawLines


		SDL_RenderPresent( renderer );      // synced with vsync
    }

    return 0;
}
catch(std::exception const &e)
{
    print("{}",e.what());
    return 1;
}


