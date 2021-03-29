#include "Grid.h"

/// <summary>
/// Constructor for the grid with the size
/// </summary>
/// <param name="width">Grid width</param>
/// <param name="height">Grid height</param>
/// <param name="nodeSize">The size of the node(it's a square so all segments are equal)</param>
/// <param name="nodeDistance">The distances between the nodes</param>
Grid::Grid(int width, int height, int nodeSize, int nodeDistance) : NodeSize(nodeSize), NodeDistance(nodeDistance), GridSize({ width,height }), StartPosition(nullptr), EndPosition(nullptr)
{
	int currentX = nodeDistance;
	int currentY = nodeDistance;

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			Node CustomNode;
			CustomNode.IsVisited = false;
			CustomNode.IsObstacle = false;
			CustomNode.X = currentX;
			CustomNode.Y = currentY;
			Nodes.push_back(CustomNode);

			currentX += nodeSize + nodeDistance;
		}
		currentY += nodeSize + nodeDistance;
		currentX = nodeDistance;
	}

}

/// <summary>
/// Default constructor for the Grid
/// </summary>
Grid::Grid() :GridSize({0,0}),NodeDistance(0),NodeSize(0)
{

}

/// <summary>
/// Check if mouse clicked over the node
/// </summary>
/// <param name="clickedNode">The node in the iteration</param>
/// <param name="mouseX">The mouse x position</param>
/// <param name="mouseY">The mouse y position</param>
/// <returns></returns>
bool Grid::IsOverNode(Node clickedNode, int mouseX, int mouseY)
{
	return mouseX > clickedNode.X && mouseX < clickedNode.X + NodeSize && mouseY > clickedNode.Y && mouseY < clickedNode.Y + NodeSize;
}

/// <summary>
/// Used to verify if the Start position is set
/// </summary>
/// <param name="nodes"></param>
/// <returns>Returns the start position index if it exists and -1 if not</returns>
int Grid::StartPositionCheck(const std::vector<Node> nodes)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].IsStartingPosition)
			return i;
	}

	return -1;
}

/// <summary>
/// Used to verify if the End position was set
/// </summary>
/// <param name="nodes"></param>
/// <returns>Return the index if the end position is set and -1 if not</returns>
int Grid::EndPositionCheck(const std::vector<Node> nodes)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].IsEndPosition)
			return i;
	}

	return -1;
}

/// <summary>
/// Calculates the direction from the start node to the end node
/// </summary>
void Grid::CalculateAStar()
{
	if (StartPosition == nullptr || EndPosition == nullptr)
		return;

	/// <summary>
	/// Making sure to reset the the Nodes
	/// </summary>
	for (int x = 0; x < GridSize.X; x++)	
		for (int y = 0; y < GridSize.Y; y++)
		{
			int NodeIndex = y * GridSize.X + x;
			Nodes[NodeIndex].IsVisited = false;
			Nodes[NodeIndex].GlobalGoal = INFINITY;
			Nodes[NodeIndex].LocalGoal = INFINITY;
			Nodes[NodeIndex].Parent = nullptr;
		}
	

	//Calulcates the distance using pythagoras theorem
	auto Distance = [](Node* a, Node* b)
	{
		int AMath = (a->X - b->X) * (a->X - b->X);
		int BMath = (a->Y - b->Y) * (a->Y - b->Y);
		return sqrtf(AMath + BMath);
	};

	/// <summary>
	/// This calculates the heuristic for the Node
	/// </summary>
	auto Heuristic = [Distance](Node* a, Node* b)
	{
		return Distance(a, b);
	};

	/// <summary>
	/// Making a virtual Node using the Starting position
	/// </summary>
	Node* CurrentNode = StartPosition;
	StartPosition->LocalGoal = 0.0f;
	/// <summary>
	/// Setting up the heuristic on the current node
	/// </summary>
	StartPosition->GlobalGoal = Heuristic(StartPosition, EndPosition);

	/// <summary>
	/// Creating a list for the nodes that we need to test
	/// </summary>
	std::list<Node*> NodesToTest;
	NodesToTest.push_back(StartPosition);


	while (!NodesToTest.empty())
	{
		/// <summary>
		/// Sorting the list in ascending order
		/// </summary>
		NodesToTest.sort([](const Node* first, const Node* second) {return first->GlobalGoal < second->GlobalGoal; });

		/// <summary>
		/// Remove any node that was visited
		/// </summary>
		while (!NodesToTest.empty() && NodesToTest.front()->IsVisited)
			NodesToTest.pop_front();

		if (NodesToTest.empty())
			break;

		/// <summary>
		/// Assigning a new node to be tested from the front of the list since they're set in ascending order
		/// </summary>
		CurrentNode = NodesToTest.front();
		CurrentNode->IsVisited = true;

		/// <summary>
		/// Iterating the neighbours of the current node
		/// </summary>

		for (auto Neighbour : CurrentNode->Neighbours)
		{
			/// <summary>
			/// Adding the current node to the testing list if it's not an obstacle and not visited
			/// </summary>
			if (!Neighbour->IsVisited && !Neighbour->IsObstacle)
				NodesToTest.push_back(Neighbour);

			/// <summary>
			/// Used to test if there's a smaller distance
			/// </summary>
			float PossibleLower = CurrentNode->GlobalGoal + Distance(CurrentNode, Neighbour);

			
			if (PossibleLower < Neighbour->LocalGoal)
			{
				Neighbour->Parent = CurrentNode;
				Neighbour->LocalGoal = PossibleLower;

				Neighbour->GlobalGoal = Neighbour->LocalGoal + Heuristic(Neighbour, EndPosition);
			}
		}
	}
}

