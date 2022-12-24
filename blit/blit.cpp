#include "include/print.h"
#include "include/thrower.h"

#include "include/sdl++.h"

#include <array>
#include <string>
using namespace std::literals;


//https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index2.php


constexpr int windowWidth{600};
constexpr int windowHeight{400};

constexpr int boardWidth{60};
constexpr int boardHeight{40};

struct RGBA
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
};

using Row   = std::array<RGBA,boardWidth>;
using Board = std::array<Row,boardHeight>;



void go(Board &pixels, SDL_Texture *texture)
{
    static  int pos{};

    for(auto &row : pixels)
    {
        row.fill( RGBA{0,0,0,0xff} );    
    }
    
    pos = (pos+1) % boardWidth;

    for(auto &row:pixels)
    {
        row[pos] = RGBA{0xff,0,0,0xff};        
    }

///

    int     pitch{};
    void   *destination{};

    if (SDL_LockTexture(texture, nullptr, &destination, &pitch) != 0) 
    {
        throw_runtime_error("SDL_LockTexture : "s + SDL_GetError());
    }

    memcpy(destination, pixels.data(), pitch * pixels.size());

    SDL_UnlockTexture(texture);
}


extern "C"
int _cdecl SDL_main(int argc, char *argv[])
try
{
    SDL         sdl;
    SDLWindow   window{"Blit", windowWidth,windowHeight};
    SDLRenderer renderer{window};

    SDL_RenderSetLogicalSize(renderer,boardWidth, boardHeight);

    auto texture    = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,boardWidth,boardHeight);

    if (texture == nullptr) 
    {
        throw_runtime_error("SDL_CreateTexture : "s + SDL_GetError());
    }


    Board   pixels{};

    bool done = false; 

    while( !done)
    { 
        SDL_Event e{};
        
        while( SDL_PollEvent( &e ) )
        { 
            if( e.type == SDL_QUIT ) 
            {
                done = true;
            }
        }

        go(pixels,texture);
        
        SDL_RenderClear  (renderer);
        SDL_RenderCopy   (renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer );      // synced with vsync
    }

    SDL_DestroyTexture(texture);

    return 0;
}
catch(std::exception const &e)
{
    print("{}",e.what());
    return 1;
}


