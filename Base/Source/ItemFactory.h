#pragma once

#include "PowerUp.h"

class ItemFactory
{
public:
	ItemFactory(void);
	~ItemFactory(void);


	Item* Create(Vector2 position, Vector2 scale, float tileSize, Item::ITEM_TYPE itemType, Mesh* mesh);
};