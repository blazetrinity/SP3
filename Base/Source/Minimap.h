#pragma once
#include <vector>
#include "Avatar.h"

class CMinimap
{
public:
	CMinimap(void);
	virtual ~CMinimap(void);

	//Set the background mesh to this class
	bool SetBackground(Mesh* aBackground);
	//Get the background mesh from this class
	Mesh* GetBackground();
	//Set the border mesh to this class
	bool SetBorder(Mesh* aBorder);
	//Get the border mesh from this class
	Mesh* GetBorder();
	//Set the avatar mesh to this class
	bool SetAvater(Mesh* anAvatar);
	//Get the avatar mesh from this class
	Mesh* GetAvatar();

	//Set angle of avatar
	bool SetAngle(const int angle);
	//Get angle
	int GetAngle();
	//Set position of avatar in minimap
	bool SetPosition(const int x, const int y);
	//Get position x of avatar in minimap
	int GetPosition_x();
	//Get position y of avatar in minimap
	int GetPosition_y();
	//Set size of minimap (for calculation of avatar in minimap)
	bool SetSize(const int size_x, const int size_y);
	//Get size of minimap (for calculation of avatar in minimap)
	int GetSize_x();
	//Get size of minimap (for calculation of avatar in minimap)
	int GetSize_y();

	Avatar* GetPlayer();
	void SetPlayer(Avatar* newPlayer);

	std::vector<Avatar*> GetEnemyList();
	void AddEnemyList(Avatar* newEnemy);

private:
	Mesh* m_cMinimap_Background;
	Mesh* m_cMinimap_Border;
		
	Avatar* Player;
	std::vector<Avatar*> EnemyList;

	//Rotation from First Angle
	int angle;
	//Offset in the minimap
	int x,y;
	//Minimap size
	int size_x, size_y;
};

