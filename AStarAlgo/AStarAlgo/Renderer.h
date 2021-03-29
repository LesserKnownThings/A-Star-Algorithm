#pragma once
#include <iostream>
#include <SDL.h>
#include "Colors.h"
#include "Grid.h"

class Renderer
{
public:
	Renderer(int screenWidth, int screenHeight, char* appName);

private:
	SDL_Surface* Surface = nullptr;
	SDL_Window* Window = nullptr;
	SDL_Renderer* Rend = nullptr;
	SDL_Event Event;
	Grid CurrentGrid;

private:
	void EndProgram();
	void FillCustomRect(SDL_Rect rect, ColorRGB color);

public:
	bool IsRunning = true;

public:
	void Start();
	void Update();
	void Draw();	
	void EventUpdate();
};
