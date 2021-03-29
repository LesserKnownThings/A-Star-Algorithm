#pragma once
#include <SDL.h>

struct ColorRGB
{
	int R;
	int G;
	int B;
};

typedef struct PresetColors
{
	SDL_Color Red{ 255,0,0 };
	SDL_Color Green{ 0,128,0 };
	SDL_Color Blue{ 0,0,255 };
	SDL_Color Yellow{ 255,255,0 };
	SDL_Color Cyan{ 0,255,255 };
	SDL_Color Black{ 0,0,0 };
	SDL_Color White{ 255,255,255 };
	SDL_Color Silver{ 192,192,192 };
	SDL_Color DarkBlue{ 0, 0, 63 };
	SDL_Color NavyBlue{ 0,0,128 };
	SDL_Color DarkSilver{ 64,64,64 };
}PresetRGB;

static PresetRGB Color;