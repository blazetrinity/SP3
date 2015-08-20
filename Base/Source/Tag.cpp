#include "Tag.h"


Tag::Tag(void)
{
}


Tag::~Tag(void)
{
}

void Tag::SetEntity(ENTITIES_TAGS entity)
{
	this->m_entity = entity;
}

Tag::ENTITIES_TAGS Tag::GetEntity()
{
	return m_entity;
}