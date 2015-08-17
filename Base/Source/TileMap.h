#pragma once
#include "Mesh.h"

class TileMap : public Mesh
{
public:
	TileMap(void);
	TileMap(const std::string & meshName);
	~TileMap(void);

	virtual void Render();

	void SetTileID(int ID);

private:
	int m_tileID;
};

