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
	Item* theNewItem = new Item;
	PowerUp* theNewPowerUp = NULL;
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
		theNewPowerUp = dynamic_cast<PowerUp*>(theNewItem);
		theNewPowerUp->Init(position,scale,tileSize,10,0.5,itemType,mesh,level);
		break;
	case Item::MOVE_SPEED_POWER:
		theNewPowerUp = dynamic_cast<PowerUp*>(theNewItem);
		theNewPowerUp->Init(position,scale,tileSize,3,150,itemType,mesh,level);
		break;
	}

	return theNewItem;
}
