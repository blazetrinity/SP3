#ifndef TAG_H
#define TAG_H

class Tag
{
public:
	Tag(void);
	~Tag(void);

	enum ENTITIES_TAGS
	{
		PLAYER = 0,
		ENEMY,
		BOSS,

		NUM_ENTITIES,
	};

	void SetEntity(ENTITIES_TAGS entity);

	ENTITIES_TAGS GetEntity();

protected:
	ENTITIES_TAGS m_entity;
};

#endif