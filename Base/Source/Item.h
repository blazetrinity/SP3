#ifndef ITEM_H
#define ITEM_H

#include "Mesh.h"
#include "GameObjectIn2D.h"

class Item : public GameObjectIn2D
{
public:
	Item(void);
	virtual ~Item(void);

	enum ITEM_TYPE
	{
		KEY_ITEM = 0,
		GUN_UPGRADE,
		FIRE_SPEED_POWER,
		MOVE_SPEED_POWER,
		HEALTH_ITEM,

		NUM_ITEM_TYPE,
	};

	// Init the item
	virtual void Init(Vector2 position, Vector2 scale, float tileSize, ITEM_TYPE newType, Mesh* newMesh, int level);

	// Get the Mesh
	Mesh* GetMesh(void);
	// Get ITEM type
	ITEM_TYPE GetType();
	// Get Active
	bool GetActive();

	int GetLevel();
	// Set the Mesh
	void SetMesh(Mesh* theMesh);
	// Set Active
	void SetActive(bool newActive);

protected:
	// Stores the texture for this Item
	Mesh* m_mesh;
	// ITEM type
	ITEM_TYPE m_itemType;
	// Item active
	bool m_active;

	int m_level;
};

#endif