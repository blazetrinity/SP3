#include "ItemFactory.h"
#include <stdlib.h>

CItemFactory::CItemFactory(void)
{
}


CItemFactory::~CItemFactory(void)
{
}

CItem* CItemFactory::Create(ITEMID id)
{
	CItem* theNewItem = NULL;

	switch(id)
	{
	case HEALTH:
		theNewItem = new CHealthItem;
		break;
	}

	return theNewItem;
}
