#ifndef SCENE2D_H
#define SCENE2D_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Minimap.h"
#include "Map.h"
#include "TileMap.h"
#include "PlayerIn2D.h"
#include "EnemyIn2D.h"
#include "Strategy_Patrol.h"

class Scene2D : public Scene
{
public:
	Scene2D();
	~Scene2D();

	virtual void Init();
	virtual void Update(double dt);
	virtual void StartGame();
	// Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	// Update Weapon status
	virtual void UpdateWeaponStatus(const unsigned char key);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size=1.0f, float x=0.0f, float y=0.0f, bool rotate=false);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderBackground();
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size=1.0f, const float x=0.0f, const float y=0.0f, const bool rotate=false);

	void RenderTileMap();
	void UpdateLevel(int checkPosition_X, int checkPosition_Y);
	void UpdateEnemy(double dt);

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_COLOR_TEXTURE0,
		U_COLOR_TEXTURE_ENABLED0,
		U_COLOR_TEXTURE1,
		U_COLOR_TEXTURE_ENABLED1,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_BACKGROUND,
		GEO_TILEMAP,
		GEO_BACKGROUNDPILLAR,
		GEO_CHARACTER_TEST,
		GEO_CHARACTER,
		GEO_GROUNDENEMYRIGHT,
		GEO_GROUNDENEMYLEFT,
		GEO_AIRENEMY,
		GEO_TEXT,
		NUM_GEOMETRY,
	};
	enum TILE_ID
	{
		TILE_BLANK = 0,
		TILE_GROUND,
		TILE_ROOF,
		TILE_LEFTWALL,
		TILE_RIGHTWALL,
		TILE_LADDER,
		TILE_DOOR,
		NUM_TILE,
	};
	enum LEVELS
	{
		LEVEL1 = 1,
		LEVEL2,
		LEVEL3,

		NUM_LEVELS,
	};
	enum WEAPON_ACTION
	{
		WA_NIL = 0,
		WA_FIRE,
		WA_RELOAD,
		WA_CHANGEWEAPON,
		WA_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[2];

	bool bLightEnabled;

	float fps;

	int m_currentLevel;
	bool m_updateMap;

	bool m_invunerable;
	float m_invunerableTimer;
	bool m_renderCharacter;
	float m_renderCharacterTimer;

	// Handle to the tilemaps
	CMap* m_cMap;
	CMap* m_cBoundMap;
	CMap* m_cDoorInteractionMap;
	CMap* m_cEnemyAndItemMap;

	// Hero's handler
	PlayerIn2D* m_player;

	// Enemey's handler
	std::vector<EnemyIn2D*> m_enemyList;
};
#endif