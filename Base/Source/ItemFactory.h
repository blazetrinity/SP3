#pragma once

#include "Item.h"
#include "HealthItem.h"

class CItemFactory
{
public:
	CItemFactory(void);
	~CItemFactory(void);

	enum ITEMID
	{
		HEALTH = 0,

		NUM_ID
	};

	CItem* Create(ITEMID id);
};