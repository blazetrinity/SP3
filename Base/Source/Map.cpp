#include "Map.h"

CMap::CMap(void)
: theScreen_Height(0)
, theScreen_Width(0)
, theNumOfTiles_Height(0)
, theNumOfTiles_Width(0)
, theMap_Height(0)
, theMap_Width(0)
, theNumOfTiles_MapHeight(0)
, theNumOfTiles_MapWidth(0)
, theTileSize(0)
{
	theScreenMap.clear();
}

CMap::~CMap(void)
{
	theScreenMap.clear();
}

void CMap::Init(const int theScreen_Height, const int theScreen_Width, const int theNumOfTiles_Height, const int theNumOfTiles_Width, const int theMap_Height, const int theMap_Width, int theTileSize)
{
	this->theScreen_Height		= theScreen_Height;
	this->theScreen_Width		= theScreen_Width;
	this->theNumOfTiles_Height	= theNumOfTiles_Height;
	this->theNumOfTiles_Width	= theNumOfTiles_Width;
	this->theMap_Height         = theMap_Height;
	this->theMap_Width          = theMap_Width;
	this->theTileSize			= theTileSize;

	theNumOfTiles_MapHeight = (int) (theMap_Height / theTileSize);
	theNumOfTiles_MapWidth = (int) (theMap_Width / theTileSize);

	theScreenMap.resize(theNumOfTiles_MapHeight);
	for (int i = 0; i < theNumOfTiles_MapHeight; ++i)
		theScreenMap[i].resize(theNumOfTiles_MapWidth);
}

bool CMap::LoadMap(const string mapName)
{
	if (LoadFile(mapName) == true)
	{
		printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
		return true;
	}

	return false;
}

Vector2 CMap::Update(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, Vector2 playerPos)
{
	if(playerPos.x < leftBorder)
	{
		SetmapOffset(Vector2(GetmapOffset().x - (leftBorder - playerPos.x), GetmapOffset().y));
		if(GetmapOffset().x < 0)
		{
			SetmapOffset(Vector2(0, GetmapOffset().y));
		}
		playerPos.x = leftBorder;
	}
	else if(playerPos.x > rightBorder)
	{
		SetmapOffset(Vector2(GetmapOffset().x + (playerPos.x - rightBorder), GetmapOffset().y));
		if(GetmapOffset().x > theScreen_Width)
		{
			SetmapOffset(Vector2(theScreen_Width, GetmapOffset().y));
		}
		playerPos.x = rightBorder;
	}

	if(playerPos.y < topBorder)
	{
		playerPos.y = topBorder;
	}
	else if(playerPos.y > bottomBorder)
	{
		playerPos.y = bottomBorder;
	}

	SettileOffset(Vector2((int) (GetmapOffset().x / GetTileSize()),0));
	if(GettileOffset().x + GetNumOfTiles_Width() > GetNumOfTiles_MapWidth())
	{
		SettileOffset(Vector2(GetNumOfTiles_MapWidth() - GetNumOfTiles_Width(),0));
	}

	return playerPos;
}

bool CMap::LoadFile(const string mapName)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;

	ifstream file(mapName.c_str());
	if(file.is_open())
	{
		int i = 0, k = 0;
		while(file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			if  (theLineCounter>=theNumOfTiles_MapHeight)
				break;

			// If this line is not a comment line, then process it
			if(!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
			{
				int theColumnCounter = 0;

				string token;
				istringstream iss(aLineOfText);
				while(getline(iss, token, ',') && (theColumnCounter<theNumOfTiles_MapWidth))
				{
					theScreenMap[theLineCounter][theColumnCounter++] = atoi(token.c_str());
				}

				theLineCounter++;
			}
		}
	}
	return true;
}

int CMap::GetNumOfTiles_Height(void)
{
	return theNumOfTiles_Height;
}

int CMap::GetNumOfTiles_Width(void)
{
	return theNumOfTiles_Width;
}

int CMap::GetTileSize(void)
{
	return theTileSize;
}

int CMap::GetNumOfTiles_MapHeight()
{
	return theNumOfTiles_MapHeight;
}

int CMap::GetNumOfTiles_MapWidth()
{
	return theNumOfTiles_MapWidth;
}

Vector2 CMap::GetmapOffset()
{
	return mapOffset;
}

Vector2 CMap::GettileOffset()
{
	return tileOffset;
}

Vector2 CMap::GetmapFineOffset()
{
	return mapFineOffset;
}

void CMap::SetmapOffset(Vector2 newmapOffset)
{
	this->mapOffset = newmapOffset;
}
	
void CMap::SettileOffset(Vector2 newtileOffset)
{
	this->tileOffset = newtileOffset;
}
	
void CMap::SetmapFineOffset(Vector2 newmapFineOffset)
{
	this->mapFineOffset = newmapFineOffset;
}

int CMap::GetScreenWidth()
{
	return theScreen_Width;
}
	
int CMap::GetScreenHeight()
{
	return theScreen_Height;
}
