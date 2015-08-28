#include "ItemFactory.h"
#include <stdlib.h>

ItemFactory::ItemFactory(void)
{
}


ItemFactory::~ItemFactory(void)
{
}

Item* ItemFactory::Create(Vector2 position, Vector2 scale, float tileSize, Item::ITEM_TYPE itemType, Mesh* mesh, int level)
{
	Item* newItem = NULL;
	PowerUp* newPowerUp = NULL;
	// Init the Item then return it

	switch(itemType)
	{
	case Item::KEY_ITEM:
		newItem = new Item;
		newItem->Init(position, scale, tileSize, itemType, mesh, level);
		return newItem;
		break;
	case Item::GUN_UPGRADE:
		newItem = new Item;
		newItem->Init(position, scale, tileSize, itemType, mesh, level);
		return newItem;
		break;
	case Item::HEALTH_ITEM:
		newItem = new Item;
		newItem->Init(position, scale, tileSize, itemType, mesh, level);
		return newItem;
		break;
	case Item::FIRE_SPEED_POWER:
		newPowerUp = new PowerUp;
		newPowerUp->Init(position,scale,tileSize,10,0.5,itemType,mesh,level);
		return newPowerUp;
		break;
	case Item::MOVE_SPEED_POWER:
		newPowerUp = new PowerUp;
		newPowerUp->Init(position,scale,tileSize,3,150,itemType,mesh,level);
		return newPowerUp;
		break;
	}
}
