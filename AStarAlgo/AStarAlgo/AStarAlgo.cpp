// AStarAlgo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <string>
#include <iostream>
#include "Renderer.h"

const std::string AppName = "AStar Algo";

int main(int argc, char** args)
{
	
	char NameArray[15];
	strcpy_s(NameArray, AppName.c_str());

	Renderer Render = {900,800, NameArray};

	Render.Start();

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		while (Render.IsRunning)
		{
			Render.Draw();
			Render.Update();
			Render.EventUpdate();
		}
	}

	return 0;
}
