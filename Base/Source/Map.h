#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "Vector2.h"

using namespace std;

class CMap
{
public:
	CMap(void);
	~CMap(void);

	void Init(const int theScreen_Height, const int theScreen_Width, const int theNumOfTiles_Height, const int theNumOfTiles_Width, const int theMap_Height, const int theMap_Width, const int theTileSize = 32);
	bool LoadMap(const string mapName);
	Vector2 Update(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, Vector2 playerPos);

	int GetNumOfTiles_Height(void); //Get the number of tiles for height of the screen
	int GetNumOfTiles_Width(void); //Get the number of tiles for width of the screen
	int GetTileSize(void);

	int GetNumOfTiles_MapHeight(); //Get the number of tiles for height of the map
	int GetNumOfTiles_MapWidth(); //Get the number of tiles for width of the map

	void SetmapOffset(Vector2 newmapOffset);
	void SettileOffset(Vector2 newtileOffset);
	void SetmapFineOffset(Vector2 newmapFineOffset);

	Vector2 GetmapOffset();
	Vector2 GettileOffset();
	Vector2 GetmapFineOffset();

	int GetScreenWidth();
	int GetScreenHeight();

	vector<vector<int> > theScreenMap;

private:
	int theScreen_Height;
	int theScreen_Width;
	int theNumOfTiles_Height; //Number of tiles in the screen's height
	int theNumOfTiles_Width; //Number of tile in the screen's width
	int theTileSize;

	int theMap_Height; //The map's height
	int theMap_Width; //The map's width
	int theNumOfTiles_MapHeight; //Number of tiles in the map's height
	int theNumOfTiles_MapWidth; //Number of tiles in the map's width

	//Codes for Scrolling
	Vector2 mapOffset;
	Vector2 tileOffset;
	Vector2 mapFineOffset;

	bool LoadFile(const string mapName);
};
