#include "Renderer.h"

/// <summary>
/// Render Constructor
/// </summary>
/// <param name="screenWidth"> Width of the window</param>
/// <param name="screenHeight"> Height of the window</param>
/// <param name="appName"> The name of the app</param>
Renderer::Renderer(int screenWidth, int screenHeight, char* appName):Event()
{
	SDL_CreateWindowAndRenderer(screenWidth, screenHeight, SDL_WINDOW_SHOWN, &Window, &Rend);

	SDL_SetWindowTitle(Window, appName);

	SDL_SetRenderDrawColor(Rend, Color.Black.r, Color.Black.g, Color.Black.b, SDL_ALPHA_OPAQUE);

	SDL_RenderClear(Rend);
}


/// <summary>
/// This runs when the program Starts // It runs only once
/// </summary>
void Renderer::Start()
{
	int GridWidth = 16;
	int GridHeight = 16;
	int NodeSize = 35;
	int NodeDistance = 10;

	CurrentGrid = { GridWidth,GridHeight, NodeSize, NodeDistance};

	
	for(int x=0; x < CurrentGrid.GridSize.X; x++)
		for (int y = 0; y < CurrentGrid.GridSize.Y; y++)
		{
			int DetectedIndex = y * CurrentGrid.GridSize.X + x;

			if (y > 0)
				CurrentGrid.Nodes[DetectedIndex].Neighbours.push_back(&CurrentGrid.Nodes[(y - 1) * CurrentGrid.GridSize.X + x]);
			
			if (y < CurrentGrid.GridSize.Y - 1)
				CurrentGrid.Nodes[DetectedIndex].Neighbours.push_back(&CurrentGrid.Nodes[(y + 1) * CurrentGrid.GridSize.X + x]);

			if(x > 0)
				CurrentGrid.Nodes[DetectedIndex].Neighbours.push_back(&CurrentGrid.Nodes[y * CurrentGrid.GridSize.X + (x - 1)]);

			if(x < CurrentGrid.GridSize.X - 1)
				CurrentGrid.Nodes[DetectedIndex].Neighbours.push_back(&CurrentGrid.Nodes[y * CurrentGrid.GridSize.X + (x + 1)]);

		}

	
}

/// <summary>
/// This runs every fram in the game
/// </summary>
void Renderer::Update()
{	
	CurrentGrid.CalculateAStar();
}

/// <summary>
/// Used to detect events in the game
/// </summary>
void Renderer::EventUpdate()
{
	while (SDL_PollEvent(&Event) != 0)
	{
		switch (Event.type)
		{
		case SDL_QUIT:
			IsRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			int mouseX = 0;
			int mouseY = 0;

			SDL_GetMouseState(&mouseX, &mouseY);

			if (Event.button.button == SDL_BUTTON_LEFT) 
			{			
				int StartSet = CurrentGrid.StartPositionCheck(CurrentGrid.Nodes);
				if (StartSet != -1)
				{
					CurrentGrid.Nodes[StartSet].IsStartingPosition = false;
					CurrentGrid.StartPosition = nullptr;
				}


				for (auto& node : CurrentGrid.Nodes)
				{
					if (CurrentGrid.IsOverNode(node, mouseX, mouseY))
					{
						node.IsStartingPosition = !node.IsStartingPosition;
						CurrentGrid.StartPosition = &node;
					}

				}

			}
			else if(Event.button.button == SDL_BUTTON_RIGHT)
			{
				int EndSet = CurrentGrid.EndPositionCheck(CurrentGrid.Nodes);
				if (EndSet != -1)
				{
					CurrentGrid.Nodes[EndSet].IsEndPosition = false;
					CurrentGrid.EndPosition = nullptr;
				}


				for (auto& node : CurrentGrid.Nodes)
				{
					if (CurrentGrid.IsOverNode(node, mouseX, mouseY))
					{
						node.IsEndPosition = !node.IsEndPosition;
						CurrentGrid.EndPosition = &node;
					}

				}
			}
			else if (Event.button.button == SDL_BUTTON_MIDDLE)
			{
				for (auto& node : CurrentGrid.Nodes)
				{
					if (CurrentGrid.IsOverNode(node, mouseX, mouseY))
					{
						node.IsObstacle = !node.IsObstacle;
					}

				}
			}
			break;
		}
	}
}

/// <summary>
/// Used to Draw in the update function
/// </summary>
void Renderer::Draw()
{
	for (int x = 0; x < CurrentGrid.GridSize.X; x++)
		for (int y = 0; y < CurrentGrid.GridSize.Y; y++)
		{
			int DetectedIndex = y * CurrentGrid.GridSize.X + x;

			for (auto N : CurrentGrid.Nodes[y * CurrentGrid.GridSize.X + x].Neighbours)
			{

				SDL_SetRenderDrawColor(Rend, Color.NavyBlue.r, Color.NavyBlue.g, Color.NavyBlue.b, SDL_ALPHA_OPAQUE);


				SDL_RenderDrawLine(Rend,
					CurrentGrid.Nodes[DetectedIndex].X + CurrentGrid.GetNodeSize() / 2,
					CurrentGrid.Nodes[DetectedIndex].Y + CurrentGrid.GetNodeSize() / 2,
					N->X + CurrentGrid.GetNodeSize() / 2,
					N->Y + CurrentGrid.GetNodeSize() / 2
				);

			}
		}

	for (int i = 0; i < CurrentGrid.Nodes.size(); i++)
	{
		SDL_Rect CustomRect =
		{
			CurrentGrid.Nodes[i].X,
			CurrentGrid.Nodes[i].Y,
			CurrentGrid.GetNodeSize(),
			CurrentGrid.GetNodeSize()
		};



		if (CurrentGrid.Nodes[i].IsObstacle)
		{
			ColorRGB ObstacleColor = { Color.DarkSilver.r, Color.DarkSilver.g, Color.DarkSilver.b };
			FillCustomRect(CustomRect, ObstacleColor);
		}
		else if (CurrentGrid.Nodes[i].IsStartingPosition)
		{
			ColorRGB StartPositionColor = { Color.Green.r, Color.Green.g, Color.Green.b };
			FillCustomRect(CustomRect, StartPositionColor);

		}
		else if (CurrentGrid.Nodes[i].IsEndPosition)
		{
			ColorRGB EndPositionColor = { Color.Red.r, Color.Red.g, Color.Red.b };
			FillCustomRect(CustomRect, EndPositionColor);
		}
		else
		{
			ColorRGB BaseColor = { Color.NavyBlue.r, Color.NavyBlue.g, Color.NavyBlue.b };
			FillCustomRect(CustomRect, BaseColor);
		}

	}

	if (CurrentGrid.EndPosition != nullptr)
	{
		Node* TempNode = CurrentGrid.EndPosition;

		while (TempNode->Parent != nullptr)
		{
			SDL_SetRenderDrawColor(Rend, Color.Yellow.r, Color.Yellow.g, Color.Yellow.b, SDL_ALPHA_OPAQUE);


			SDL_RenderDrawLine(Rend,
				TempNode->X + CurrentGrid.GetNodeSize() / 2,
				TempNode->Y + CurrentGrid.GetNodeSize() / 2,
				TempNode->Parent->X + CurrentGrid.GetNodeSize() / 2,
				TempNode->Parent->Y + CurrentGrid.GetNodeSize() / 2
			);

			TempNode = TempNode->Parent;
		}
	}

	SDL_RenderPresent(Rend);
}

void Renderer::EndProgram()
{
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Renderer::FillCustomRect(SDL_Rect rect, ColorRGB color)
{		
	SDL_SetRenderDrawColor(Rend, color.R, color.G, color.B, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(Rend, &rect);
}
