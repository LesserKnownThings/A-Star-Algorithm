#pragma once
#include <iostream>
#include <vector>
#include "Colors.h"
#include <list>

/// <summary>
/// This is a node for the grid
/// </summary>
struct Node
{
	bool IsObstacle = false;
	bool IsVisited = false;

	bool IsStartingPosition = false;
	bool IsEndPosition = false;

	float GlobalGoal = INFINITY;
	float LocalGoal = INFINITY;
	int X;
	int Y;
	std::vector<Node*> Neighbours;
	Node* Parent = nullptr;
};

class Grid
{
public:
	Grid(int width, int height, int nodeSize, int nodeDistance);
	Grid();

private:
	int NodeSize;
	struct Coordinates
	{
		int X;
		int Y;
	};
	int NodeDistance;

public:
	std::vector<Node> Nodes;
	int GetNodeSize() { return NodeSize; }
	int GetNodeDistance() { return NodeDistance; }
	Coordinates GridSize;
	bool IsOverNode(Node clickedNode, int mouseX, int mouseY);	
	int StartPositionCheck(const std::vector<Node> nodes);
	int EndPositionCheck(const std::vector<Node> nodes);


public:
	Node* StartPosition = nullptr;
	Node* EndPosition = nullptr;



	//A* calculations
public:
	void CalculateAStar();
};

