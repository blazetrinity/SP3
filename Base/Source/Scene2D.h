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
#include "ItemFactory.h"
#include "TileMap.h"
#include "PlayerIn2D.h"
#include "EnemyIn2D.h"
#include "Strategy_Patrol.h"
#include "Strategy_Chase.h"
#include "Projectile.h"
#include <irrKlang.h>
#include "Text.h"

using namespace irrklang;
using std::vector;

#pragma comment(lib, "irrklang.lib")

class Scene2D : public Scene
{
public:
	Scene2D();
	~Scene2D();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size=1.0f, float x=0.0f, float y=0.0f, bool rotate=false);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderBackground();
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size=1.0f, const float x=0.0f, const float y=0.0f, const bool rotate=false);
	void RenderUI();
	void RenderHUD();
	void RenderTileMap();

	void UpdatePlayer(double dt);
	void UpdateEnemy(double dt);
	void UpdateProjectile(double dt);
	void UpdateItem();
	void UpdateStoryText(double dt);
	void UpdateLevel(int checkPosition_X, int checkPosition_Y);

	bool CheckPlayerHiding();
	
	void SortHighscore(int newscore);

	void InitHighScore();
	void InitGame();
	void ResetGame();
	void GameOver();

	Projectile* FetchProjectile();

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
		GEO_TILEMAP = 0,
		GEO_CHARACTER,
		GEO_ENEMY1,
		GEO_ENEMY2,
		GEO_BOSS,
		GEO_TEXT,
		GEO_BULLET,
		GEO_BOSS_BULLET,
		GEO_ARROW,//arrow for menu
		GEO_PAUSED_ARROW,//arrow for pause
		GEO_MAIN_MENU,
		GEO_CREDITS,
		GEO_PAUSED,
		GEO_GAMEOVER,
		GEO_INSTRUCTIONS,
		GEO_HIGHSCORE,
		GEO_SPEED_POWER_UP,
		GEO_FIRE_RATE_POWER_UP,
		GEO_HEALTH_ITEM,
		GEO_GUN_PART,
		GEO_KEY_ITEM,
		GEO_HEALTH,//HUD
		GEO_LIVES,//HUD
		GEO_HEARTS,//HUD
		GEO_COMPLETED,//HUD
		GEO_SCORE,//HUD
		NUM_GEOMETRY,
	};
	enum TILE_ID
	{
		TILE_BLANK = 0,	
		TILE_TOPLEFT,
		TILE_TOPRIGHT,
		TILE_BOTTOMLEFT,
		TILE_BOTTOMRIGHT,
		TILE_TOP,
		TILE_BOTTOM,
		TILE_LEFT,
		TILE_RIGHT,
		TILE_MIDDLE,
		TILE_HIDE_MIDDLE = 18,
		TILE_DOOR,
		TILE_EDGE,
		NUM_TILE,
	};
	enum ENEMY_AND_ITEM_TILE
	{
		WHITE_GHOST_PATROL_UPDOWN = 2,
		WHITE_GHOST_PATROL_LEFTRIGHT,
		RED_GHOST_PATROL_UPDOWN,
		RED_GHOST_PATROL_LEFTRIGHT,
		BOSS_GHOST,
		KEY_ITEM,
		GUN_UPGRADE,
		FIRE_SPEED_POWER,
		MOVE_SPEED_POWER,
		HEALTH_ITEM,

		NUM_ENEMY_AND_ITEM_TILE,
	};
	enum LEVELS
	{
		LEVEL1 = 1,
		LEVEL2,
		LEVEL3,
		LEVEL4,

		NUM_LEVELS,
	};
	enum SND_SOUNDS
	{
		SND_BLANK = 0,
		SND_MENU,
		SND_BACKGROUND,
		SND_GAMEOVER,
		SND_GHOST_QUEUE,
		SND_FIRE,
		SND_DAMAGE,
		SND_BOSS,
		SND_BOSS_ATTACK,

		TOTAL_SND,
	};
	enum GAME_STATE
	{
		MAIN_MENU = 0,
		CREDITS,
		PAUSED,
		GAMEOVER,
		INSTRUCTIONS,
		SCORE,
		GAME,
		NUM_UI,
	};

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[2];

	bool bLightEnabled;

	float fps;

	// Game element variables
	bool m_levelCompleted;
	int m_currentLevel;
	bool m_updateMap;
	float m_ghostQueueTimer;
	int m_gunCollectibleCount;
	int m_score;
	bool m_spawnGhost;
	bool m_ghostTriggered;
	bool m_resetGame;
	int m_highscore[5];

	bool  m_isPaused;

	// Menu variables
	int  m_menuStatus;
	int  m_menuChoice;

	// Handle to the tilemaps
	CMap* m_cMap;
	CMap* m_cBoundMap;
	CMap* m_cDoorInteractionMap;
	CMap* m_cEnemyAndItemMap;

	// Hero's handler
	PlayerIn2D* m_player;

	// Enemey's handler
	vector<EnemyIn2D*> m_enemyList;

	// Item's Factory
	ItemFactory* m_itemFactory;

	// Item's handler
	vector<Item*> m_itemList;

	// Projectile's handler
	vector<Projectile*> m_projectileList;

	//Text
	Text *TextArray;
    int m_listPosition;
	int m_string_CharacterPosition;
	double m_textTimer;
	string m_renderString;
	unsigned short m_storyLevelTracker;

	// Bullet Animations
	SpriteAnimation* m_bulletAnimation, *m_bossbulletAnimation;

	//The sound engine
	ISoundEngine* m_theSoundEngine;
	ISound* m_backgroundSound;
	ISound* m_eventSound;
	ISoundSource* m_sounds[TOTAL_SND];

	SND_SOUNDS m_currentBackgroundSound, m_currentEventSound;

	AstarPathfind* m_path;
};
#endif