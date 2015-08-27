#include "ItemFactory.h"
#include <stdlib.h>

ItemFactory::ItemFactory(void)
{
}


ItemFactory::~ItemFactory(void)
{
}

Item* ItemFactory::Create(Vector2 position, Vector2 scale, float tileSize, Item::ITEM_TYPE itemType, Mesh* mesh)
{
	Item* theNewItem = NULL;

	// Init the Item then return it

	switch(itemType)
	{
	case Item::KEY_ITEM:
		break;
	case Item::GUN_UPGRADE:
		break;
	case Item::HEALTH_ITEM:
		break;
	case Item::FIRE_SPEED_POWER:
		break;
	case Item::MOVE_SPEED_POWER:
		break;
	}

	return theNewItem;
}
