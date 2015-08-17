#pragma once

#include "Mesh.h"
#include "Vector3.h"

class CItem
{
public:
	CItem(void);
	~CItem(void);

protected:
	//Position
	Vector3 m_Position;
	// Stores the texture for this Item
	Mesh* theMesh;

public:
	// Get the pos of this Item
	Vector3 GetPos();
	// Set the pos of this Item
	void SetPos(Vector3 newPos);
	// Get the Mesh
	Mesh* GetMesh(void);
	// Set the Mesh
	void SetMesh(Mesh* theMesh);
	// Set the Texture ID
	void SetTextureID(const int TextureID);
};

