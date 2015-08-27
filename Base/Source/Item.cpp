#include "Item.h"


Item::Item(void) : m_mesh(NULL)
{
}


Item::~Item(void)
{
	if (m_mesh != NULL)
	{
		delete m_mesh;
		m_mesh = NULL;
	}
}

// Init the item
void Item::Init(Vector2 position, Vector2 scale, float tileSize, ITEM_TYPE newType, Mesh* newMesh)
{
	GameObjectIn2D::Init(position, scale, tileSize);
	this->m_active = true;
	this->m_itemType = newType;
	this->m_mesh = newMesh;
}

// Get the Mesh
Mesh* Item::GetMesh(void)
{
	return m_mesh;
}

// Get ITEM type
Item::ITEM_TYPE Item::GetType()
{
	return m_itemType;
}

// Get Active
bool Item::GetActive()
{
	return m_active;
}

// Set the Mesh
void Item::SetMesh(Mesh* newMesh)
{
	this->m_mesh = newMesh;
}

// Set Active
void Item::SetActive(bool newActive)
{
	this->m_active = newActive;
}
