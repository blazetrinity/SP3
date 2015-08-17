#include "Item.h"


CItem::CItem(void) : theMesh(NULL)
{
}


CItem::~CItem(void)
{
	if (theMesh != NULL)
	{
		delete theMesh;
		theMesh = NULL;
	}
}

// Get the pos of this Item
Vector3 CItem::GetPos()
{
	return m_Position;
}
// Set the pos of this Item
void CItem::SetPos(Vector3 newPos)
{
	this->m_Position = newPos;
}
// Get the Mesh
Mesh* CItem::GetMesh(void)
{
	return theMesh;
}
// Set the Mesh
void CItem::SetMesh(Mesh* theMesh)
{
	this->theMesh = theMesh;
}
// Set the Texture ID
void CItem::SetTextureID(const int TextureID)
{
	if ((this->theMesh != NULL) && (TextureID != 0))
	{
		this->theMesh->textureID[0] = TextureID;
	}
}
