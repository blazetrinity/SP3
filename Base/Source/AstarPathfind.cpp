#include "AstarPathfind.h"


AstarPathfind::AstarPathfind(void)
{
}


AstarPathfind::~AstarPathfind(void)
{
}

PositionNode AstarPathfind::NodeFromWorldPoint(Vector2 worldPosition)
{
	int checkPosition_X = (int) ((m_map->GetmapOffset().x + worldPosition.x) / m_map->GetTileSize());
	int checkPosition_Y = m_map->GetNumOfTiles_Height() - (int) ceil( (float)(worldPosition.y + m_map->GetTileSize()) / m_map->GetTileSize());

	return grid[checkPosition_Y][checkPosition_X];
}

vector<PositionNode> AstarPathfind::FindPath(Vector2 startPos, Vector2 targetPos)
{
	PositionNode startNode = NodeFromWorldPoint(startPos);
	PositionNode targetNode = NodeFromWorldPoint(targetPos);

	vector<PositionNode> openSet;
	vector<PositionNode> closeSet;

	openSet.push_back(startNode);

	while(openSet.size() > 0)
	{
		PositionNode currentNode = openSet[0];
		
		for(int i = 1; i < openSet.size(); ++i)
		{
			if(openSet[i].GetfCost() < currentNode.GetfCost() || (openSet[i].GetfCost() == currentNode.GetfCost() && openSet[i].GethCost() < currentNode.GethCost()))
			{
				currentNode = openSet[i];
			}
		}

		openSet.erase(std::remove(openSet.begin(), openSet.end(), currentNode), openSet.end());
		closeSet.push_back(currentNode);

		if(currentNode == targetNode)
		{
			return RetracePath(startNode, targetNode);
		}

		vector<PositionNode> neighbours = GetNeighbours(currentNode);

		for(int i = 0; i < neighbours.size(); ++i)
		{
			if(!neighbours[i].GetWalkable() || (std::find(closeSet.begin(), closeSet.end(), neighbours[i]) != closeSet.end()))
			{
				continue;
			}

			int newMovementCostToNeighbour = currentNode.GetgCost() + GetDistance(currentNode, neighbours[i]);

			if(newMovementCostToNeighbour < neighbours[i].GetgCost() || !(std::find(openSet.begin(), openSet.end(), neighbours[i]) != openSet.end()))
			{
				neighbours[i].SetgCost(newMovementCostToNeighbour);
				neighbours[i].SethCost(GetDistance(neighbours[i], targetNode));
				neighbours[i].SetParentNode(&currentNode);

				if(!(std::find(openSet.begin(), openSet.end(), neighbours[i]) != openSet.end()))
				{
					openSet.push_back(neighbours[i]);
				}
			}
		}
	}
}

vector<PositionNode> AstarPathfind::RetracePath(PositionNode startNode, PositionNode endNode)
{
	vector<PositionNode> path;
	PositionNode currentNode = endNode;
	while(!(currentNode == startNode))
	{
		path.push_back(currentNode);
		currentNode = currentNode.GetParentNode();
	}

	std::reverse(path.begin(), path.end());

	return path;
}

void AstarPathfind::GenerateGrid(CMap* map)
{
	this->m_map = map;

	grid.resize(m_map->GetNumOfTiles_MapHeight());

	for(int i = 0; i < m_map->GetNumOfTiles_MapHeight(); ++i)
	{
		grid[i].resize(m_map->GetNumOfTiles_MapWidth());
	}

	for(int i = 0; i < m_map->GetNumOfTiles_MapHeight(); ++i)
	{
		for(int j = 0; j < m_map->GetNumOfTiles_MapWidth(); ++j)
		{
			if(m_map->theScreenMap[i][j] == 0)
			{
				grid[i][j].Init(Vector2(j * m_map->GetTileSize(), (m_map->GetScreenHeight() -  (i * m_map->GetTileSize()))), false, i, j);
			}
			else
			{
				grid[i][j].Init(Vector2(j * m_map->GetTileSize(), (m_map->GetScreenHeight() -  (i * m_map->GetTileSize()))), true, i, j);
			}
		}
	}
}

vector<PositionNode> AstarPathfind::GetNeighbours(PositionNode node)
{
	vector<PositionNode> neighbours;

	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			if(x == 0 && y == 0)
			{
				continue;
			}

			int checkX = node.GetGridX() + x;
			int checkY = node.GetGridY() + y;

			if((checkX >= 0 && checkX < m_map->GetNumOfTiles_MapHeight()) && (checkY >= 0 && checkY < m_map->GetNumOfTiles_MapWidth()))
			{
				neighbours.push_back(grid[checkX][checkY]);
			}
		}
	}

	return neighbours;
}

int AstarPathfind::GetDistance(PositionNode nodeA, PositionNode nodeB)
{
	int dstX = abs(nodeA.GetGridX() - nodeB.GetGridX());
	int dstY = abs(nodeA.GetGridY() - nodeB.GetGridY());
	
	if(dstX < dstY)
	{
		return 14*dstY + 10* (dstX - dstY);
	}

	return 14*dstX + 10* (dstY - dstX);
}