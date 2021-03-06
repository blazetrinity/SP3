#include "Scene2D.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

static const int MAXTEXT = 69;
static const float MAXGHOSTQUEUETIMER = 10.0f;
static const int WHITEGHOSTHEALTH = 30;
static const int REDGHOSTHEALTH = 60;
static const int BOSSHEALTH = 200;
static const int PLAYERHEALTH = 100;
static const int GUN_UPGRADE_REQUIREMENT = 5;
static const float SKILLUPGRADE = 10;
static const float HEALAMOUNT = 30;

Scene2D::Scene2D()
	: m_cMap(NULL)
{
}

Scene2D::~Scene2D()
{
}

void Scene2D::Init()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Text.fragmentshader" );

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED0] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE0] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	// Init Camera
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Init meshes
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID[0] = LoadTGA("Image//text.tga");

	//UI
	meshList[GEO_MAIN_MENU] = MeshBuilder::Generate2DMesh("MAIN_MENU", Color(1, 1, 1), 0.0f, 0.0f, 1024.0f, 800.0f);
	meshList[GEO_MAIN_MENU]->textureID[0] = LoadTGA("Image//menu_main.tga");
	meshList[GEO_PAUSED] = MeshBuilder::Generate2DMesh("PAUSED", Color(1, 1, 1), 0.0f, 0.0f,1024.0f, 800.0f);
	meshList[GEO_PAUSED]->textureID[0] = LoadTGA("Image//menu_paused.tga");
	meshList[GEO_GAMEOVER] = MeshBuilder::Generate2DMesh("WIN", Color(1, 1, 1), 0.0f, 0.0f, 1024.0f, 800.0f);
	meshList[GEO_GAMEOVER]->textureID[0] = LoadTGA("Image//menu_gameover.tga");
	meshList[GEO_CREDITS] = MeshBuilder::Generate2DMesh("CREDITS", Color(1, 1, 1), 0.0f, 0.0f, 1024.0f, 800.0f);
	meshList[GEO_CREDITS]->textureID[0] = LoadTGA("Image//menu_credits.tga");
	meshList[GEO_INSTRUCTIONS] = MeshBuilder::Generate2DMesh("INSTRUCTIONS", Color(1, 1, 1), 0.0f, 0.0f, 1024.0f, 800.0f);
	meshList[GEO_INSTRUCTIONS]->textureID[0] = LoadTGA("Image//menu_instructions.tga");
	meshList[GEO_HIGHSCORE] = MeshBuilder::Generate2DMesh("HIGHSCORE", Color(1, 1, 1), 0.0f, 0.0f, 1024.0f, 800.0f);
	meshList[GEO_HIGHSCORE]->textureID[0] = LoadTGA("Image//menu_highscore.tga");

	meshList[GEO_ARROW] = MeshBuilder::Generate2DMesh("GEO_ARROW", Color(1, 1, 1), 0.0f, 0.0f, 1, 1);
	meshList[GEO_ARROW]->textureID[0] = LoadTGA("Image//arrow.tga");
	meshList[GEO_PAUSED_ARROW] = MeshBuilder::Generate2DMesh("GEO_PAUSED_ARROW", Color(1, 1, 1), 0.0f, 0.0f, 1, 1);
	meshList[GEO_PAUSED_ARROW]->textureID[0] = LoadTGA("Image//paused_arrow.tga");
	meshList[GEO_SPEED_POWER_UP] = MeshBuilder::Generate2DMesh("GEO_SPEED_POWER_UP", Color(1, 1, 1), 0.0f, 0.0f, 1, 1);
	meshList[GEO_SPEED_POWER_UP]->textureID[0] = LoadTGA("Image//speed_up.tga");
	meshList[GEO_FIRE_RATE_POWER_UP] = MeshBuilder::Generate2DMesh("GEO_FIRE_RATE_POWER_UP", Color(1, 1, 1), 0.0f, 0.0f, 1, 1);
	meshList[GEO_FIRE_RATE_POWER_UP]->textureID[0] = LoadTGA("Image//fire_rate_up.tga");
	meshList[GEO_HEALTH_ITEM] = MeshBuilder::Generate2DMesh("GEO_HEALTH_ITEM", Color(1, 1, 1), 0.0f, 0.0f, 1, 1);
	meshList[GEO_HEALTH_ITEM]->textureID[0] = LoadTGA("Image//health_item.tga");
	meshList[GEO_GUN_PART] = MeshBuilder::Generate2DMesh("GEO_GUN_PART", Color(1, 1, 1), 0.0f, 0.0f, 1, 1);
	meshList[GEO_GUN_PART]->textureID[0] = LoadTGA("Image//gun_item.tga");
	meshList[GEO_KEY_ITEM] =MeshBuilder::Generate2DMesh("GEO_KEY_ITEM", Color(1, 1, 1), 0.0f, 0.0f, 1, 1);
	meshList[GEO_KEY_ITEM]->textureID[0] = LoadTGA("Image//key_item.tga");

	// Load the ground mesh and texture
	meshList[GEO_ENEMY1] = MeshBuilder::GenerateSpriteAnimation("GEO_ENEMY1", 4, 3);
	meshList[GEO_ENEMY1]->textureID[0] = LoadTGA("Image//enemy1_sprite.tga");
	meshList[GEO_ENEMY2] = MeshBuilder::GenerateSpriteAnimation("GEO_ENEMY2", 4, 3);
	meshList[GEO_ENEMY2]->textureID[0] = LoadTGA("Image//enemy2_sprite.tga");
	meshList[GEO_BOSS] = MeshBuilder::GenerateSpriteAnimation("GEO_BOSS", 1, 3);
	meshList[GEO_BOSS]->textureID[0] = LoadTGA("Image//boss.tga");

	meshList[GEO_CHARACTER] = MeshBuilder::GenerateSpriteAnimation("GEO_CHARACTER_TEST", 4, 3);
	meshList[GEO_CHARACTER]->textureID[0] = LoadTGA("Image//character_sprite.tga");

	
	meshList[GEO_TILEMAP] = MeshBuilder::GenerateTileMap("GEO_TILEMAP", 4, 6);
	meshList[GEO_TILEMAP]->textureID[0] = LoadTGA("Image//map_tileset.tga");

	meshList[GEO_BULLET] = MeshBuilder::GenerateSpriteAnimation("GEO_BULLET", 1, 5);
	meshList[GEO_BULLET]->textureID[0] = LoadTGA("Image//bullet.tga");

	meshList[GEO_BOSS_BULLET] = MeshBuilder::GenerateSpriteAnimation("GEO_BOSS_BULLET", 1, 6);
	meshList[GEO_BOSS_BULLET]->textureID[0] = LoadTGA("Image//boss_bullet.tga");

	m_bulletAnimation = dynamic_cast<SpriteAnimation*>(meshList[GEO_BULLET]);
	m_bulletAnimation->m_anim = new Animation;
	m_bulletAnimation->m_anim->Set(0, 4, 0, 0.5);

	m_bossbulletAnimation = dynamic_cast<SpriteAnimation*>(meshList[GEO_BOSS_BULLET]);
	m_bossbulletAnimation->m_anim = new Animation;
	m_bossbulletAnimation->m_anim->Set(0, 5, 0, 0.1);

	meshList[GEO_HEALTH] = MeshBuilder::Generate2DMesh("GEO_HEALTH", Color(1, 1, 1), 0, 0, 3, 1);
	meshList[GEO_HEALTH]->textureID[0] = LoadTGA("Image//HUD_health.tga");
	meshList[GEO_LIVES] = MeshBuilder::Generate2DMesh("GEO_HEALTH", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_LIVES]->textureID[0] = LoadTGA("Image//HUD_lives.tga");
	meshList[GEO_HEARTS] = MeshBuilder::Generate2DMesh("GEO_HEALTH", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_HEARTS]->textureID[0] = LoadTGA("Image//HUD_hearts.tga");
	meshList[GEO_COMPLETED] = MeshBuilder::Generate2DMesh("GEO_COMPLETED", Color(1, 1, 1), 0, 0, 5, 1);
	meshList[GEO_COMPLETED]->textureID[0] = LoadTGA("Image//HUD_levelcompleted.tga");
	meshList[GEO_SCORE] = MeshBuilder::Generate2DMesh("GEO_COMPLETED", Color(1, 1, 1), 0, 0, 2, 1);
	meshList[GEO_SCORE]->textureID[0] = LoadTGA("Image//HUD_score.tga");

	InitGame();
	InitHighScore();

	m_isPaused = false;
	m_menuStatus = 0;
	m_menuChoice = 1;

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	bLightEnabled = true;
}
void Scene2D::InitHighScore()
{
	const int size = 5;
	std::string value[size];
	std::ifstream is("Highscore.txt");
	std::string::size_type sz;

	int i = 0;

	while(!is.eof() && i < size)
	{
		is >> value[i++];
	}
	is.close();

	for(int j = 0; j < size; ++j)
	{
		m_highscore[j] = std::stoi(value[j], &sz);
	}

}
void Scene2D::InitGame()
{
	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(800, 1024, 25, 32, 800 ,1024);
	m_cMap->LoadMap( "Csv//level1_visual.csv" );

	m_cBoundMap = new CMap();
	m_cBoundMap->Init(800, 1024, 25, 32, 800 ,1024);
	m_cBoundMap->LoadMap( "Csv//level1_bound.csv" );

	m_cDoorInteractionMap = new CMap();
	m_cDoorInteractionMap->Init(800, 1024, 25, 32, 800 ,1024);
	m_cDoorInteractionMap->LoadMap( "Csv//level1_door.csv" );

	m_path = new AstarPathfind;
	m_path->GenerateGrid(m_cBoundMap);

	//Init Enemy Ai position and animations
	m_cEnemyAndItemMap = new CMap();
	m_cEnemyAndItemMap->Init(800, 1024, 25, 32, 800, 1024);

	m_itemFactory = new ItemFactory;
	Skill* skill = NULL;

	for(int i = 1; i < NUM_LEVELS; ++i)
	{
		switch(i)
		{
		case 1:	m_cEnemyAndItemMap->LoadMap( "Csv//level1EnemyAndItem.csv" );
			break;
		case 2:	m_cEnemyAndItemMap->LoadMap( "Csv//level2EnemyAndItem.csv" );
			break;
		case 3:	m_cEnemyAndItemMap->LoadMap( "Csv//level3EnemyAndItem.csv" );
			break;
		case 4: m_cEnemyAndItemMap->LoadMap( "Csv//level4EnemyAndItem.csv" );
			break;
		}

		SpriteAnimation *BossSpriteAnimation = (dynamic_cast<SpriteAnimation*>(meshList[GEO_BOSS]));
		SpriteAnimation *whiteGhostSpriteAnimation = (dynamic_cast<SpriteAnimation*>(meshList[GEO_ENEMY2]));
		SpriteAnimation *redGhostSpriteAnimation = (dynamic_cast<SpriteAnimation*>(meshList[GEO_ENEMY1]));

		for(int j = 0; j < m_cEnemyAndItemMap->GetNumOfTiles_MapHeight(); j ++)
		{
			for(int k = 0; k < m_cEnemyAndItemMap->GetNumOfTiles_MapWidth(); k ++)
			{
				if(m_cEnemyAndItemMap->theScreenMap[j][k] == WHITE_GHOST_PATROL_UPDOWN)
				{
					SpriteAnimation* newSpriteAnimation = whiteGhostSpriteAnimation;

					EnemyIn2D* theEnemy = new EnemyIn2D;

					skill = new Skill();
					skill->Init(1.0f,10.f, 1.f, 1.f, false, Tag::ENEMY);

					theEnemy->Init(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32), 10, i, newSpriteAnimation, EnemyIn2D::WHITE_GHOST_PATROL_UPDOWN, 1, skill, WHITEGHOSTHEALTH);

					theEnemy->SetAnimation(EnemyIn2D::IDLE_RIGHT, 7, 7, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_LEFT, 4, 4, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_UP, 10, 10, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_DOWN, 1, 1, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::WALK_RIGHT, 6, 8, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_LEFT, 3, 5, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_UP, 9, 11, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_DOWN, 0, 2, 0, 0.5);
					theEnemy->ChangeAnimation(EnemyIn2D::IDLE_RIGHT);

					theEnemy->SetStrategy(EnemyIn2D::PATROL_STRATEGY, m_path);
					m_enemyList.push_back(theEnemy);
				}

				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == WHITE_GHOST_PATROL_LEFTRIGHT)
				{
					SpriteAnimation* newSpriteAnimation = whiteGhostSpriteAnimation;

					EnemyIn2D* theEnemy = new EnemyIn2D;

					skill = new Skill();
					skill->Init(1.0f, 10.f, 1.f, 1.f, false, Tag::ENEMY);

					theEnemy->Init(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32), 10, i, newSpriteAnimation, EnemyIn2D::WHITE_GHOST_PATROL_LEFTRIGHT, 1, skill, WHITEGHOSTHEALTH);

					theEnemy->SetAnimation(EnemyIn2D::IDLE_RIGHT, 7, 7, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_LEFT, 4, 4, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_UP, 10, 10, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_DOWN, 1, 1, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::WALK_RIGHT, 6, 8, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_LEFT, 3, 5, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_UP, 9, 11, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_DOWN, 0, 2, 0, 0.5);
					theEnemy->ChangeAnimation(EnemyIn2D::IDLE_RIGHT);

					theEnemy->SetStrategy(EnemyIn2D::PATROL_STRATEGY, m_path);
					m_enemyList.push_back(theEnemy);
				}

				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == RED_GHOST_PATROL_UPDOWN)
				{
					SpriteAnimation* newSpriteAnimation = redGhostSpriteAnimation;

					EnemyIn2D* theEnemy = new EnemyIn2D;

					skill = new Skill();
					skill->Init(1.0f,20.f, 1.f, 1.f, false, Tag::ENEMY);

					theEnemy->Init(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32), 10, i, newSpriteAnimation, EnemyIn2D::RED_GHOST_PATROL_UPDOWN, 1, skill, REDGHOSTHEALTH);

					theEnemy->SetAnimation(EnemyIn2D::IDLE_RIGHT, 7, 7, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_LEFT, 4, 4, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_UP, 10, 10, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_DOWN, 1, 1, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::WALK_RIGHT, 6, 8, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_LEFT, 3, 5, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_UP, 9, 11, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_DOWN, 0, 2, 0, 0.5);
					theEnemy->ChangeAnimation(EnemyIn2D::IDLE_RIGHT);

					theEnemy->SetStrategy(EnemyIn2D::PATROL_STRATEGY, m_path);
					m_enemyList.push_back(theEnemy);
				}

				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == RED_GHOST_PATROL_LEFTRIGHT)
				{
					SpriteAnimation* newSpriteAnimation = redGhostSpriteAnimation;

					EnemyIn2D* theEnemy = new EnemyIn2D;

					skill = new Skill();
					skill->Init(1.0f,20.f, 1.f, 1.f, false, Tag::ENEMY);

					theEnemy->Init(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32), 10, i, newSpriteAnimation, EnemyIn2D::RED_GHOST_PATROL_LEFTRIGHT, 1, skill, REDGHOSTHEALTH);

					theEnemy->SetAnimation(EnemyIn2D::IDLE_RIGHT, 7, 7, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_LEFT, 4, 4, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_UP, 10, 10, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_DOWN, 1, 1, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::WALK_RIGHT, 6, 8, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_LEFT, 3, 5, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_UP, 9, 11, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_DOWN, 0, 2, 0, 0.5);
					theEnemy->ChangeAnimation(EnemyIn2D::IDLE_RIGHT);

					theEnemy->SetStrategy(EnemyIn2D::PATROL_STRATEGY, m_path);
					m_enemyList.push_back(theEnemy);
				}
				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == BOSS_GHOST)
				{
					SpriteAnimation* newSpriteAnimation = BossSpriteAnimation;

					EnemyIn2D* theEnemy = new EnemyIn2D;

					skill = new Skill();
					skill->Init(5.0f, 20.f, 2.5f, 125.f, true, Tag::BOSS);

					theEnemy->Init(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(64, 64), 10, i, newSpriteAnimation, EnemyIn2D::BOSS_GHOST, 1, skill, BOSSHEALTH);

					theEnemy->SetAnimation(EnemyIn2D::WALK_LEFT, 0, 2, 0, 0.7);
					theEnemy->ChangeAnimation(EnemyIn2D::WALK_LEFT);

					theEnemy->SetStrategy(EnemyIn2D::PATROL_STRATEGY, m_path);
					theEnemy->SetActive(true);
					m_enemyList.push_back(theEnemy);
				}
				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == FIRE_SPEED_POWER)
				{
					m_itemList.push_back(m_itemFactory->Create(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32),1,Item::FIRE_SPEED_POWER,meshList[GEO_FIRE_RATE_POWER_UP],i));
				}
				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == MOVE_SPEED_POWER)
				{
					m_itemList.push_back(m_itemFactory->Create(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32),1,Item::MOVE_SPEED_POWER,meshList[GEO_SPEED_POWER_UP],i));
				}
				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == GUN_UPGRADE)
				{
					m_itemList.push_back(m_itemFactory->Create(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32),1,Item::GUN_UPGRADE,meshList[GEO_GUN_PART],i));
				}
				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == HEALTH_ITEM)
				{
					m_itemList.push_back(m_itemFactory->Create(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32),1,Item::HEALTH_ITEM,meshList[GEO_HEALTH_ITEM],i));
				}
				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == KEY_ITEM)
				{
					m_itemList.push_back(m_itemFactory->Create(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32),1,Item::KEY_ITEM,meshList[GEO_KEY_ITEM],i));
				}
			}
		}
	}

	// Init Player position and animation
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_CHARACTER]);
	skill = new Skill();
	skill->Init(5.0f, 20.f, 1.0f, 200.f, true, Tag::PLAYER);

	m_player = new PlayerIn2D();
	m_player->Init(Vector2(64, 64), Vector2(32, 32), 10, 1, skill, PLAYERHEALTH);
	m_player->SetMesh(sa);
	m_player->SetAnimation(PlayerIn2D::IDLE_RIGHT, 7, 7, 0, 1);
	m_player->SetAnimation(PlayerIn2D::IDLE_LEFT, 4, 4, 0, 1);
	m_player->SetAnimation(PlayerIn2D::IDLE_UP, 10, 10, 0, 1);
	m_player->SetAnimation(PlayerIn2D::IDLE_DOWN, 1, 1, 0, 1);
	m_player->SetAnimation(PlayerIn2D::WALK_RIGHT, 6, 8, 0, 0.5);
	m_player->SetAnimation(PlayerIn2D::WALK_LEFT, 3, 5, 0, 0.5);
	m_player->SetAnimation(PlayerIn2D::WALK_UP, 9, 11, 0, 0.5);
	m_player->SetAnimation(PlayerIn2D::WALK_DOWN, 0, 2, 0, 0.5);
	m_player->ChangeAnimation(PlayerIn2D::IDLE_RIGHT);

	//Init Sounds
	m_theSoundEngine = createIrrKlangDevice();
	m_backgroundSound = NULL;
	m_eventSound= NULL;
	m_sounds[SND_BACKGROUND] = m_theSoundEngine->addSoundSourceFromFile("wav//background.wav");
	m_sounds[SND_MENU] = m_theSoundEngine->addSoundSourceFromFile("wav//menu.wav");
	m_sounds[SND_GAMEOVER] = m_theSoundEngine->addSoundSourceFromFile("wav//weed.wav");
	m_sounds[SND_GHOST_QUEUE] = m_theSoundEngine->addSoundSourceFromFile("wav//ghost_queue.wav");
	m_sounds[SND_FIRE] = m_theSoundEngine->addSoundSourceFromFile("wav//fire.wav");
	m_sounds[SND_DAMAGE] = m_theSoundEngine->addSoundSourceFromFile("wav//damage.wav");
	m_sounds[SND_BOSS] = m_theSoundEngine->addSoundSourceFromFile("wav//boss.wav");
	m_sounds[SND_BOSS_ATTACK] = m_theSoundEngine->addSoundSourceFromFile("wav//boss_attack.wav");

	m_backgroundSound = m_theSoundEngine->play2D(m_sounds[SND_MENU], true, false, true);
	m_backgroundSound->setVolume(0.1f);

	m_currentBackgroundSound = SND_MENU;
	m_currentEventSound = SND_BLANK;

	// Init game element variables
	m_levelCompleted = false;
	m_currentLevel = 1;
	m_updateMap = false; 
	m_ghostQueueTimer = MAXGHOSTQUEUETIMER / m_currentLevel;
	m_gunCollectibleCount = 0;
	m_spawnGhost = false;
	m_ghostTriggered = false;
	m_resetGame = false;

	// Init Text
	TextArray = new Text;
	TextArray->Init("Story.txt");
	m_listPosition = 0;
	m_string_CharacterPosition = 0;
	m_textTimer = 0.0f;
	m_storyLevelTracker = 1;
	m_renderString = "";
	m_score = 0;
}

void Scene2D::ResetGame()
{
	m_cMap->LoadMap( "Csv//level1_visual.csv" );
	m_cBoundMap->LoadMap( "Csv//level1_bound.csv" );
	m_cDoorInteractionMap->LoadMap( "Csv//level1_door.csv" );

	// Init game element variables
	m_levelCompleted = false;
	m_currentLevel = 1;
	m_updateMap = false; 
	m_ghostQueueTimer = MAXGHOSTQUEUETIMER / m_currentLevel;
	m_gunCollectibleCount = 0;
	m_spawnGhost = false;
	m_ghostTriggered = false;
	m_resetGame = false;
	m_score = 0;
	m_path->InitGrid(m_cBoundMap);

	m_listPosition = 0;
	m_string_CharacterPosition = 0;
	m_textTimer = 0.0f;
	m_storyLevelTracker = 1;
	m_renderString = "";
	m_score = 0;

	for(vector<EnemyIn2D*>::iterator it = m_enemyList.begin(); it != m_enemyList.end(); ++it)
	{
		EnemyIn2D* enemy = (EnemyIn2D*)*it;

		enemy->ResetEnemy(m_path);

		if(enemy->GetEnemyType() == EnemyIn2D::RED_GHOST_PATROL_LEFTRIGHT || enemy->GetEnemyType() == EnemyIn2D::RED_GHOST_PATROL_UPDOWN)
		{
			enemy->SetHealth(REDGHOSTHEALTH);
		}
		else if(enemy->GetEnemyType() == EnemyIn2D::WHITE_GHOST_PATROL_LEFTRIGHT || enemy->GetEnemyType() == EnemyIn2D::WHITE_GHOST_PATROL_UPDOWN)
		{
			enemy->SetHealth(WHITEGHOSTHEALTH);
		}
		else
		{
			enemy->SetHealth(BOSSHEALTH);
			enemy->SetActive(true);
		}
	}

	for(vector<Projectile*>::iterator it = m_projectileList.begin(); it != m_projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)*it;
		
		projectile->SetActive(false);
	}

	for(vector<Item*>::iterator it = m_itemList.begin(); it != m_itemList.end(); ++it)
	{
		Item* item = (Item*)*it;

		item->SetActive(true);

		if(item->GetType() == Item::FIRE_SPEED_POWER || Item::MOVE_SPEED_POWER)
		{
			PowerUp* powerup = dynamic_cast<PowerUp*>(item);

			if(powerup != NULL)
			{
				powerup->ResetLifeTime();
			}
		}
	}

	Skill* skill = new Skill();
	skill->Init(5.0f, 20.f, 1.0f, 200.f, true, Tag::PLAYER);

	m_player->Init(Vector2(64, 64), Vector2(32, 32), 10, 1, skill, PLAYERHEALTH);
	m_player->ChangeAnimation(PlayerIn2D::IDLE_RIGHT);
}

void Scene2D::GameOver()
{
	if(m_currentBackgroundSound != SND_GAMEOVER)
	{
		if(m_currentBackgroundSound != SND_BLANK)
		{
			m_backgroundSound->stop();
			m_backgroundSound->drop();
			m_backgroundSound = NULL;
		}

		m_backgroundSound = m_theSoundEngine->play2D(m_sounds[SND_GAMEOVER], false, false, true);
		m_currentBackgroundSound = SND_GAMEOVER;
	}
	SortHighscore(m_score);
	m_menuStatus = GAMEOVER;
	m_resetGame = true;
}

void Scene2D::SortHighscore(int newscore)
{
	const int tempsize = 6;
	int templist[tempsize];

	for(int i = 0; i < (tempsize - 1); ++i)
	{
		templist[i] = m_highscore[i];
	}

	templist[(tempsize - 1)] = newscore;

	for(int iter = (tempsize - 1); iter > 0; --iter)
	{
		for(int index = (tempsize - iter); index > 0; --index)
		{
			if(templist[index] > templist[(index - 1)])
			{
				int temp = templist[index];
				templist[index] = templist[(index - 1)];
				templist[(index - 1)] = temp;
			}
		}
	}

	for(int j = 0; j < tempsize - 1; ++j)
	{
		m_highscore[j] = templist[j];
	}

	std::ofstream os("Highscore.txt");

	int k = 0;
	while(k < (tempsize - 1))
	{
		os << m_highscore[k] << "\n";
		++k;
	}
	os.close();
}

void Scene2D::Update(double dt)
{
	if(m_menuStatus == MAIN_MENU)
	{
		int oldChoice = m_menuChoice;
		if(Application::IsKeyPressed(VK_DOWN))
		{
			if(m_menuChoice < 4 )
			{
				m_menuChoice++;
				Sleep(150);

			}
		}

		if(Application::IsKeyPressed(VK_UP)) 
		{
			if(m_menuChoice > 1)
			{
				m_menuChoice--;
				Sleep(150);
			}
		}

		if(Application::IsKeyPressed(VK_RETURN)) 
		{
			if(m_menuChoice == 1)
			{
				m_menuStatus = GAME;

				if(m_currentBackgroundSound != SND_BACKGROUND)
				{
					if(m_currentBackgroundSound != SND_BLANK)
					{
						m_backgroundSound->stop();
						m_backgroundSound->drop();
						m_backgroundSound = NULL;
					}

					m_backgroundSound = m_theSoundEngine->play2D(m_sounds[SND_BACKGROUND], false, false, true);
					m_backgroundSound->setVolume(0.5f);
					m_currentBackgroundSound = SND_BACKGROUND;
				}

				if(m_resetGame)
				{
					ResetGame();
				}
			}
			if(m_menuChoice == 2) 
			{
				m_menuStatus = INSTRUCTIONS;		
			}
			if(m_menuChoice == 3) 
			{
				m_menuStatus = CREDITS;	
			}
			if(m_menuChoice == 4) 
			{
				m_menuStatus = SCORE;						
			}
		}		
	}

	if(m_menuStatus == SCORE)
	{
		Sleep(150);
		if (Application::IsKeyPressed(VK_RETURN)) 
		{
			m_menuStatus = MAIN_MENU;
			Sleep(150);
		}
	}

	if(m_menuStatus == CREDITS)
	{
		Sleep(150);
		if (Application::IsKeyPressed(VK_RETURN)) 
		{
			m_menuStatus = MAIN_MENU;
			Sleep(150);
		}
	}

	if(m_menuStatus == INSTRUCTIONS)
	{
		Sleep(150);
		if (Application::IsKeyPressed(VK_RETURN)) 
		{
			m_menuStatus = MAIN_MENU;
			Sleep(150);
		}
	}

	if(m_menuStatus == GAMEOVER)
	{
		if (Application::IsKeyPressed(VK_RETURN)) 
		{
			m_menuStatus = MAIN_MENU;
			Sleep(150);

			if(m_currentBackgroundSound != SND_MENU)
			{
				if(m_currentBackgroundSound != SND_BLANK)
				{
					m_backgroundSound->stop();
					m_backgroundSound->drop();
					m_backgroundSound = NULL;
				}

				m_backgroundSound = m_theSoundEngine->play2D(m_sounds[SND_MENU], false, false, true);
				m_backgroundSound->setVolume(0.1f);
				m_currentBackgroundSound = SND_MENU;
			}
		}
	}

	if(m_menuStatus == GAME)
	{
		if(Application::IsKeyPressed('P') && m_isPaused == false) 
		{
			m_isPaused = true;
			m_menuStatus = PAUSED;
		}
	}

	if(m_menuStatus == PAUSED)
	{
		int oldChoice = m_menuChoice;
		if (Application::IsKeyPressed(VK_DOWN))
		{
			if (m_menuChoice < 2 )
			{
				m_menuChoice++;
			}
		}

		if (Application::IsKeyPressed(VK_UP)) 
		{	
			if (m_menuChoice > 1)
			{
				m_menuChoice--;
			}
		}

		if (Application::IsKeyPressed(VK_RETURN)) 
		{
			if (m_menuChoice == 1) 
			{
				m_menuStatus = GAME;
				m_isPaused = false;
			}
			if (m_menuChoice == 2) 
			{
				m_menuStatus = MAIN_MENU;
				m_menuChoice = 1;
				Sleep(150);
				m_isPaused = false;
				m_resetGame = true;

				if(m_currentBackgroundSound != SND_MENU)
				{
					if(m_currentBackgroundSound != SND_BLANK)
					{
						m_backgroundSound->stop();
						m_backgroundSound->drop();
						m_backgroundSound = NULL;
					}

					m_backgroundSound = m_theSoundEngine->play2D(m_sounds[SND_MENU], false, false, true);
					m_backgroundSound->setVolume(0.1f);
					m_currentBackgroundSound = SND_MENU;
				}
			}
		}
	}

	if( m_menuStatus == GAME)
	{
		if(m_isPaused == false)
		{
			if(Application::IsKeyPressed('1'))
				glEnable(GL_CULL_FACE);
			if(Application::IsKeyPressed('2'))
				glDisable(GL_CULL_FACE);
			if(Application::IsKeyPressed('3'))
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if(Application::IsKeyPressed('4'))
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


			if(m_currentEventSound != SND_BLANK)
			{
				if(m_eventSound->isFinished() == true)
				{
					m_eventSound->stop();
					m_eventSound->drop();
					m_eventSound = NULL;
					m_currentEventSound = SND_BLANK;
				}
			}

			//Player Movement
			UpdatePlayer(dt);

			//Ai Update and collision with character
			UpdateEnemy(dt);

			//Text Update
			UpdateStoryText(dt);

			//Projectile Update
			UpdateProjectile(dt);

			//Item Update
			UpdateItem();

			if(m_gunCollectibleCount == GUN_UPGRADE_REQUIREMENT)
			{
				m_player->GetSkill()->UpgradeDamage(SKILLUPGRADE);
				m_gunCollectibleCount = 0;
			}

			if(m_levelCompleted)
			{
				// Update the hero
				int checkPosition_X = (int) ((m_cMap->GetmapOffset().x + m_player->GetPosition().x) / m_cMap->GetTileSize());
				int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float)(m_player->GetPosition().y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());

				//Check if player is going into the next level
				UpdateLevel(checkPosition_X, checkPosition_Y);
			}


			//Up ghost trigger
			if(!m_ghostTriggered && !m_levelCompleted && m_currentLevel != LEVEL4)
			{
				m_ghostQueueTimer -= (float)dt;

				if(m_ghostQueueTimer <= 0)
				{
					if(m_currentBackgroundSound != SND_GHOST_QUEUE)
					{
						m_backgroundSound->stop();
						m_backgroundSound->drop();
						m_backgroundSound = NULL;
						m_backgroundSound = m_theSoundEngine->play2D(m_sounds[SND_GHOST_QUEUE], false, false, true);
						m_currentBackgroundSound = SND_GHOST_QUEUE;
					}

					if(m_backgroundSound->isFinished() == true)
					{
						// Check if player hiding
						if(!CheckPlayerHiding())
						{
							m_spawnGhost = true;
						}

						m_ghostQueueTimer = MAXGHOSTQUEUETIMER / m_currentLevel;
						m_backgroundSound->stop();
						m_backgroundSound->drop();
						m_backgroundSound = NULL;
						m_backgroundSound = m_theSoundEngine->play2D(m_sounds[SND_BACKGROUND], true, false, true);
						m_currentBackgroundSound = SND_BACKGROUND;
					}
				}

				if(m_spawnGhost)
				{
					for(vector<EnemyIn2D*>::iterator it = m_enemyList.begin(); it != m_enemyList.end(); ++it)
					{
						EnemyIn2D* enemy = (EnemyIn2D*)*it;
						if(enemy->GetActive() == false && enemy->GetCurrentLevel() == m_currentLevel && enemy->GetEnemyType() != EnemyIn2D::BOSS_GHOST)
						{
							enemy->SetActive(true);
						}
					}

					m_spawnGhost = false;
					m_ghostTriggered = true;
				}
			}

			//Scroll the Door Interaction Map
			m_cDoorInteractionMap->Update(m_cDoorInteractionMap->GetTileSize(), (m_cDoorInteractionMap->GetScreenWidth() - (2 * m_cDoorInteractionMap->GetTileSize())), m_cDoorInteractionMap->GetTileSize(), (m_cDoorInteractionMap->GetScreenHeight() -(2 * m_cDoorInteractionMap->GetTileSize())), m_player->GetPosition());
			//Scroll the Bound Map 
			m_cBoundMap->Update(m_cBoundMap->GetTileSize(), (m_cBoundMap->GetScreenWidth() - (2 * m_cBoundMap->GetTileSize())), m_cBoundMap->GetTileSize(), (m_cBoundMap->GetScreenHeight() -(2 * m_cBoundMap->GetTileSize())), m_player->GetPosition());
			//Scroll the Visual Map and update player position to scroll box's edge
			m_player->SetPosition(m_cMap->Update(m_cMap->GetTileSize(), (m_cMap->GetScreenWidth() - (2 * m_cMap->GetTileSize())), m_cMap->GetTileSize(), (m_cMap->GetScreenHeight() - (2 * m_cMap->GetTileSize())), m_player->GetPosition()));
			fps = (float)(1.f / dt);
		}
	}
}
void Scene2D::UpdatePlayer(double dt)
{
	//Player Movement
	if(Application::IsKeyPressed('W'))
	{
		m_player->MoveUpDown(true);
		m_player->ChangeAnimation(PlayerIn2D::WALK_UP);
		m_player->GetMesh()->Update(dt);
	}
	else if(Application::IsKeyPressed('S'))
	{
		m_player->MoveUpDown(false);
		m_player->ChangeAnimation(PlayerIn2D::WALK_DOWN);
		m_player->GetMesh()->Update(dt);
	}
	else if(Application::IsKeyPressed('A'))
	{
		m_player->MoveLeftRight(true);
		m_player->ChangeAnimation(PlayerIn2D::WALK_LEFT);
		m_player->GetMesh()->Update(dt);
	}
	else if(Application::IsKeyPressed('D'))
	{
		m_player->MoveLeftRight(false);
		m_player->ChangeAnimation(PlayerIn2D::WALK_RIGHT);
		m_player->GetMesh()->Update(dt);
	}
	if(Application::IsKeyPressed(VK_SPACE))
	{
		if(m_player->Attack())
		{
			Projectile* projectile = FetchProjectile();

			projectile->Init(m_player->GetSkill(),m_player->GetPosition(), m_player->GetScale(), m_player->GetFacingNormal(), m_bulletAnimation, m_currentLevel, 1);

			if(m_currentEventSound != SND_FIRE)
			{
				if(m_currentEventSound != SND_BLANK)
				{
					m_eventSound->stop();
					m_eventSound->drop();
					m_eventSound = NULL;
				}

				m_eventSound = m_theSoundEngine->play2D(m_sounds[SND_FIRE], false, false, true);
				m_currentEventSound = SND_FIRE;
			}
		}
	}

	//Update the player velocity and position
	m_player->Update(m_cBoundMap, dt, true);

}

void Scene2D::UpdateStoryText(double dt)
{
	// control key spam
	m_textTimer += dt;
	// array list
	if(m_listPosition < MAXTEXT)
	{
		if(m_string_CharacterPosition == 0)
		{
			m_storyLevelTracker = TextArray->getTextCharacter(m_listPosition,(TextArray->getStringsize(m_listPosition)-1));
		}
		if((m_storyLevelTracker-48) == m_currentLevel)
		{
			// check if string size have reached the end
			if(m_string_CharacterPosition == (TextArray->getStringsize(m_listPosition)-1))
			{
				// text control
				if(Application::IsKeyPressed('L') && m_textTimer > 0.15)
				{
					m_string_CharacterPosition = 0;
					m_listPosition++;
					m_renderString.clear();
					m_textTimer = 0.0f;
				}
			}
			//constant typewriter effect & string size must not reached max
			if(m_textTimer > NULL && m_string_CharacterPosition != (TextArray->getStringsize(m_listPosition)-1))
			{
				m_renderString += TextArray->getTextCharacter(m_listPosition,m_string_CharacterPosition);
				m_textTimer = 0.0f;
				m_string_CharacterPosition++;
			}
		}
		else if((m_storyLevelTracker-48) > m_currentLevel)
		{
			m_renderString.clear();
		}
		else
		{
			m_renderString.clear();
			m_listPosition++;
			m_string_CharacterPosition = 0;
		}
	}
}

void Scene2D::UpdateEnemy(double dt)
{
	static int i = 0;
	for(vector<EnemyIn2D*>::iterator it = m_enemyList.begin(); it !=  m_enemyList.end(); ++it)
	{
		EnemyIn2D *enemy = (EnemyIn2D *) *it;

		// If enemy is in current level
		if(enemy->GetActive() && enemy->GetCurrentLevel() == m_currentLevel)
		{
			if(((enemy->GetPosition().x - m_cMap->GetmapOffset().x) >= 0) && ((enemy->GetPosition().x - m_cMap->GetmapOffset().x) < m_cMap->GetScreenWidth()) && ((enemy->GetPosition().y >= 0) && (enemy->GetPosition().y < m_cMap->GetScreenHeight())))
			{
				if(enemy->GetEnemyType() != EnemyIn2D::BOSS_GHOST && enemy->GetCurrentStrategy() == EnemyIn2D::PATROL_STRATEGY && ((enemy->GetPosition() - m_player->GetPosition()).Length()) < (m_cMap->GetTileSize() * 4))
				{
					static Vector2 positiveAngle, negativeAngle;
					positiveAngle.Set(90, 0);
					negativeAngle.Set(270, 180);

					// Check if player is in front of enemy
					float Dist1 = (enemy->GetPosition() - m_player->GetPosition()).Length();
					float Dist2 = (enemy->GetPosition() - enemy->GetViewPosition()).Length();
					Vector2 enemyToPlayerDir = (m_player->GetPosition() - enemy->GetPosition()).Normalized();
					float angleOfPlayerFromEnemy = atan2(enemyToPlayerDir.y, enemyToPlayerDir.x);
					float angleOffset = 15.f;

					if(Dist1 < Dist2)
					{
						if(enemy->GetFacingNormal().x > 0)
						{
							if(angleOfPlayerFromEnemy < positiveAngle.x + angleOffset || angleOfPlayerFromEnemy < positiveAngle.x - angleOffset)
							{
								enemy->SetStrategy(EnemyIn2D::CHASE_STRATEGY, m_path);
								enemy->SetCollision(false);
							}
						}
						else if(enemy->GetFacingNormal().x < 0)
						{
							if(angleOfPlayerFromEnemy < negativeAngle.x + angleOffset || angleOfPlayerFromEnemy < negativeAngle.x - angleOffset)
							{
								enemy->SetStrategy(EnemyIn2D::CHASE_STRATEGY, m_path);
								enemy->SetCollision(false);
							}
						}
						else if(enemy->GetFacingNormal().y > 0)
						{
							if(angleOfPlayerFromEnemy < positiveAngle.y + angleOffset || angleOfPlayerFromEnemy < positiveAngle.y - angleOffset)
							{
								enemy->SetStrategy(EnemyIn2D::CHASE_STRATEGY, m_path);
								enemy->SetCollision(false);
							}
						}
						else if(enemy->GetFacingNormal().y < 0)
						{
							if(angleOfPlayerFromEnemy < negativeAngle.y + angleOffset || angleOfPlayerFromEnemy < negativeAngle.y - angleOffset)
							{
								enemy->SetStrategy(EnemyIn2D::CHASE_STRATEGY, m_path);
								enemy->SetCollision(false);
							}
						}
					}
				}

				// Check if player collide with enemy
				if(m_player->CollideWith(enemy))
				{
					// Player takes damage
					if(m_player->TakeDamage(enemy->GetSkill()->GetDamage()))
					{
						if(m_currentEventSound != SND_DAMAGE)
						{
							if(m_currentEventSound != SND_BLANK)
							{
								m_eventSound->stop();
								m_eventSound->drop();
								m_eventSound = NULL;
							}

							m_eventSound = m_theSoundEngine->play2D(m_sounds[SND_DAMAGE], false, false, true);
							m_currentEventSound = SND_DAMAGE;
						}

						if(m_player->GetLife() < 0)
						{
							GameOver();
						}
					}
				}
			}

			// Update enemy
			enemy->Update(m_cBoundMap, dt, true, m_player->GetPosition());

			if(enemy->GetEnemyType() == EnemyIn2D::BOSS_GHOST)
			{
				if(enemy->Attack())
				{
					Projectile* projectile = FetchProjectile();

					projectile->Init(enemy->GetSkill(), enemy->GetPosition(), enemy->GetScale(), Vector2(-1, 0), m_bossbulletAnimation, m_currentLevel, 1);
				}
			}
		}
	}
}

void Scene2D::UpdateProjectile(double dt)
{
	for(vector<Projectile*>::iterator it = m_projectileList.begin(); it != m_projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)*it;

		// If projectile is on the same level
		if(projectile->GetActive() && projectile->GetLevel() != m_currentLevel)
		{
			projectile->SetActive(false);
		}

		if(projectile->GetActive())
		{
			// Update projectile
			projectile->Update(m_cBoundMap, dt);

			// Check if projectile is fired by player
			if(projectile->GetTag().GetEntity() == Tag::PLAYER)
			{
				for(vector<EnemyIn2D*>::iterator it2 = m_enemyList.begin(); it2 !=  m_enemyList.end(); ++it2)
				{
					EnemyIn2D *enemy = (EnemyIn2D *) *it2;

					// Check if projectile collide with enemy
					if(projectile->GetActive() && enemy->GetActive() && enemy->GetCurrentLevel() == m_currentLevel && enemy->CollideWith(projectile))
					{
						// Enemy takes damage
						if(enemy->TakeDamage(projectile->GetDamage()))
						{
							if(enemy->GetEnemyType() == EnemyIn2D::WHITE_GHOST_PATROL_LEFTRIGHT || enemy->GetEnemyType() == EnemyIn2D::WHITE_GHOST_PATROL_UPDOWN)
							{
								m_score += 10;
							}

							else if(enemy->GetEnemyType() == EnemyIn2D::RED_GHOST_PATROL_LEFTRIGHT || enemy->GetEnemyType() == EnemyIn2D::RED_GHOST_PATROL_UPDOWN)
							{
								m_score += 20;
							}

							else if(enemy->GetEnemyType() == EnemyIn2D::BOSS_GHOST)
							{
								m_score += 100;

								GameOver();
							}
						}

						// Projectile hit so turn projectile off
						projectile->SetActive(false);
					}
				}
			}

			else if(projectile->GetTag().GetEntity() == Tag::BOSS)
			{
				if(projectile->GetLevel() == m_currentLevel && projectile->CollideWith(m_player))
				{
					if(m_player->TakeDamage(projectile->GetDamage()))
					{
						if(m_currentEventSound != SND_DAMAGE)
						{
							if(m_currentEventSound != SND_BLANK)
							{
								m_eventSound->stop();
								m_eventSound->drop();
								m_eventSound = NULL;
							}

							m_eventSound = m_theSoundEngine->play2D(m_sounds[SND_DAMAGE], false, false, true);
							m_currentEventSound = SND_DAMAGE;
						}

						if(m_player->GetLife() < 0)
						{
							GameOver();
						}
					}
				}
			}
		}
	}
}

void Scene2D::UpdateItem()
{
	for(vector<Item*>::iterator it = m_itemList.begin(); it != m_itemList.end(); ++it)
	{
		Item* item = (Item*)*it;

		if(item->GetActive() && item->GetCurrentLevel() == m_currentLevel && item->CollideWith(m_player))
		{
			switch(item->GetType())
			{
			case Item::KEY_ITEM:
				{
					m_levelCompleted = true;

					if(m_currentBackgroundSound == SND_GHOST_QUEUE)
					{
						m_backgroundSound->stop();
						m_backgroundSound->drop();
						m_backgroundSound = m_theSoundEngine->play2D(m_sounds[SND_BACKGROUND], false, false, true);
						m_backgroundSound->setVolume(0.5f);
						m_currentBackgroundSound = SND_BACKGROUND;
					}
				}
				break;
			case Item::GUN_UPGRADE:
				{
					++m_gunCollectibleCount;
				}
				break;
			case Item::HEALTH_ITEM:
				{
					m_player->HealHealth(HEALAMOUNT);
				}
				break;
			case Item::FIRE_SPEED_POWER:
				{
					PowerUp* powerup = dynamic_cast<PowerUp*>(item);
					m_player->AddPowerUp(powerup);
				}
				break;
			case Item::MOVE_SPEED_POWER:
				{
					PowerUp* powerup = dynamic_cast<PowerUp*>(item);
					m_player->AddPowerUp(powerup);
				}
				break;
			}

			item->SetActive(false);
		}
	}
}

void Scene2D::UpdateLevel(int checkPosition_X, int checkPosition_Y)
{
	if(m_updateMap == false)
	{
		if(m_cDoorInteractionMap->theScreenMap[checkPosition_Y][checkPosition_X] > 1)		
		{
			if(m_currentLevel == (m_cDoorInteractionMap->theScreenMap[checkPosition_Y][checkPosition_X]))
			{
				m_currentLevel = (m_cDoorInteractionMap->theScreenMap[checkPosition_Y][checkPosition_X] - 1);
			}
			else
			{
				m_currentLevel = (m_cDoorInteractionMap->theScreenMap[checkPosition_Y][checkPosition_X]);
			}

			m_updateMap = true;
		}
		else if(m_cDoorInteractionMap->theScreenMap[checkPosition_Y][checkPosition_X+1] > 1)
		{
			if(m_currentLevel == (m_cDoorInteractionMap->theScreenMap[checkPosition_Y][checkPosition_X+1]))
			{
				m_currentLevel = (m_cDoorInteractionMap->theScreenMap[checkPosition_Y][checkPosition_X+1] - 1);
			}
			else
			{
				m_currentLevel = (m_cDoorInteractionMap->theScreenMap[checkPosition_Y][checkPosition_X+1]);
			}

			m_updateMap = true;
		}
		else if(m_cDoorInteractionMap->theScreenMap[checkPosition_Y+1][checkPosition_X] > 1)	
		{
			if(m_currentLevel == (m_cDoorInteractionMap->theScreenMap[checkPosition_Y+1][checkPosition_X]))
			{
				m_currentLevel = (m_cDoorInteractionMap->theScreenMap[checkPosition_Y+1][checkPosition_X] - 1);
			}
			else
			{
				m_currentLevel = (m_cDoorInteractionMap->theScreenMap[checkPosition_Y+1][checkPosition_X]);
			}
			m_updateMap = true;
		}
		else if(m_cDoorInteractionMap->theScreenMap[checkPosition_Y+1][checkPosition_X+1] > 1)
		{
			if(m_currentLevel == (m_cDoorInteractionMap->theScreenMap[checkPosition_Y+1][checkPosition_X+1]))
			{
				m_currentLevel = (m_cDoorInteractionMap->theScreenMap[checkPosition_Y+1][checkPosition_X+1] - 1);
			}
			else
			{
				m_currentLevel = (m_cDoorInteractionMap->theScreenMap[checkPosition_Y+1][checkPosition_X+1]);
			}
			m_updateMap = true;
		}
	}

	if(m_updateMap)
	{
		if(m_currentLevel == LEVEL1)
		{
			m_cMap->LoadMap( "Csv//level1_visual.csv" );
			m_cBoundMap->LoadMap( "Csv//level1_bound.csv" );
			m_cDoorInteractionMap->LoadMap( "Csv//level1_door.csv" );
		}
		else if(m_currentLevel == LEVEL2)
		{
			m_cMap->LoadMap( "Csv//level2_visual.csv" );
			m_cBoundMap->LoadMap( "Csv//level2_bound.csv" );
			m_cDoorInteractionMap->LoadMap( "Csv//level2_door.csv" );

			if(!m_ghostTriggered)
			{
				m_score += 100 * (m_currentLevel - 1);
			}
		}
		else if(m_currentLevel == LEVEL3)
		{
			m_cMap->LoadMap( "Csv//level3_visual.csv" );
			m_cBoundMap->LoadMap( "Csv//level3_bound.csv" );
			m_cDoorInteractionMap->LoadMap( "Csv//level3_door.csv" );

			if(!m_ghostTriggered)
			{
				m_score += 100 * (m_currentLevel - 1);
			}
		}
		else if(m_currentLevel == LEVEL4)
		{
			m_cMap->LoadMap( "Csv//level4_visual.csv" );
			m_cBoundMap->LoadMap( "Csv//level4_bound.csv" );
			m_cDoorInteractionMap->LoadMap( "Csv//level4_door.csv" );

			if(!m_ghostTriggered)
			{
				m_score += 100 * (m_currentLevel - 1);
			}
		}

		m_player->CalPosition((m_cMap->GetTileSize()), (m_cMap->GetScreenWidth() - (2 * m_cMap->GetTileSize())), (m_cMap->GetTileSize()), (m_cMap->GetScreenHeight() - (2 * m_cMap->GetTileSize())), (float)m_cMap->GetTileSize());
		m_updateMap = false;
		m_ghostQueueTimer = MAXGHOSTQUEUETIMER / m_currentLevel;
		m_ghostTriggered = false;
		m_levelCompleted = false;
		m_path->InitGrid(m_cBoundMap);
	}
}

bool Scene2D::CheckPlayerHiding()
{
	int checkPosition_X = (int) ((m_cMap->GetmapOffset().x + m_player->GetPosition().x) / m_cMap->GetTileSize());
	int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float)(m_player->GetPosition().y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());

	if(m_cBoundMap->theScreenMap[checkPosition_Y][checkPosition_X] == 2 || m_cBoundMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == 2 || m_cBoundMap->theScreenMap[checkPosition_Y+1][checkPosition_X] == 2 || m_cBoundMap->theScreenMap[checkPosition_Y+1][checkPosition_X+1] == 2)		
	{
		return true;
	}

	return false;
}

Projectile* Scene2D::FetchProjectile()
{
	for(vector<Projectile*>::iterator it = m_projectileList.begin(); it != m_projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)*it;
		if(!projectile->GetActive())
		{
			projectile->SetActive(true);
			return projectile;
		}
	}

	for(int i = 0; i < 10; ++i)
	{
		Projectile* projectile = new Projectile;
		m_projectileList.push_back(projectile);
	}

	Projectile* projectile = m_projectileList.back();
	projectile->SetActive(true);
	return projectile;
}

void Scene2D::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE0], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void Scene2D::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID[0] <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE0], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Scene2D::RenderMeshIn2D(Mesh *mesh, const bool enableLight, const float size, const float x, const float y, const bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80,80,-60,60,-10,10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();

	modelStack.LoadIdentity();
	modelStack.Translate(x,y,0);
	if(rotate)
	{
		modelStack.Rotate(0,0,0,1);
	}

	modelStack.Scale(size,size,size);
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP],1,GL_FALSE, &MVP.a[0]);


	for(int i = 0; i < Mesh::MAX_TEXTURE; ++i)
	{
		if(mesh->textureID[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0 + (i * 2)], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE0 + (i * 2)], i);
		}

		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0 + (i * 2)], 0);
		}
	}

	mesh->Render();

	for(int i = 0; i < Mesh::MAX_TEXTURE; ++i)
	{
		if(mesh->textureID[i] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void Scene2D::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_cMap->GetScreenWidth(), 0, m_cMap->GetScreenHeight(), -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	if (rotate)
		modelStack.Rotate(0, 0, 0, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
		glUniform1i(m_parameters[U_COLOR_TEXTURE0], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

void Scene2D::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);

		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	for(int i = 0; i < Mesh::MAX_TEXTURE; ++i)
	{
		if(mesh->textureID[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0 + (i * 2)], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE0 + (i * 2)], i);
		}

		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0 + (i * 2)], 0);
		}
	}
	mesh->Render();
	for(int i = 0; i < Mesh::MAX_TEXTURE; ++i)
	{
		if(mesh->textureID[i] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void Scene2D::RenderBackground()
{
	// Render the background
	//Render2DMesh(meshList[GEO_BACKGROUND], false, 1.0f);
}

void Scene2D::RenderUI()
{

	if(m_menuStatus == MAIN_MENU )
	{
		Render2DMesh(meshList[GEO_MAIN_MENU], false,1.0f);

		if(m_menuChoice == 1)
		{
			Render2DMesh(meshList[GEO_ARROW], false,100,5.f,490.f);
		}
		else if(m_menuChoice == 2)
		{
			Render2DMesh(meshList[GEO_ARROW], false,100,5.f,350.f);
		}
		else if(m_menuChoice == 3)
		{
			Render2DMesh(meshList[GEO_ARROW], false,100,5.f,200.f);
		}
		else if(m_menuChoice == 4)
		{
			Render2DMesh(meshList[GEO_ARROW], false,100,5.f,60.f);
		}
	}
	if(m_menuStatus == INSTRUCTIONS)
	{
		Render2DMesh(meshList[GEO_INSTRUCTIONS], false, 1.0f);
	}
	if(m_menuStatus == CREDITS)
	{
		Render2DMesh(meshList[GEO_CREDITS], false, 1.0f);
	}
	if(m_menuStatus == SCORE)
	{
		Render2DMesh(meshList[GEO_HIGHSCORE], false, 1.0f);
	}
	if(m_menuStatus == GAMEOVER)
	{
		Render2DMesh(meshList[GEO_GAMEOVER], false, 1.0f);
	}
	if(m_menuStatus == PAUSED)
	{
		Render2DMesh(meshList[GEO_PAUSED], false, 1.0f);
		if(m_menuChoice == 1)
		{
			Render2DMesh(meshList[GEO_PAUSED_ARROW], false,300,-30.f,500.f);
		}
		else if(m_menuChoice == 2)
		{
			Render2DMesh(meshList[GEO_PAUSED_ARROW], false,300,-70.f,360.f);
		}
	}

}
void Scene2D::RenderHUD()
{
	//On screen text
	std::ostringstream ss;
	ss.precision(3);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 1), 2, 70,0);	

	//On screen text
	std::ostringstream ss1;
	ss1.precision(3);
	ss1 << m_player->GetHealth();
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 10, 57);

	std::ostringstream ss2;
	ss2.precision(3);
	ss2 << m_score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 74, 57);


	Render2DMesh(meshList[GEO_HEALTH], false,35,0,765);
	Render2DMesh(meshList[GEO_SCORE], false,35,835,765);
	if(m_levelCompleted)
	{
		Render2DMesh(meshList[GEO_COMPLETED], false,35,435,765);
	}
	
	if(m_player->GetLife() == 3)
	{
		Render2DMesh(meshList[GEO_HEARTS], false,30,205,770);
		Render2DMesh(meshList[GEO_HEARTS], false,30,235,770);
		Render2DMesh(meshList[GEO_HEARTS], false,30,265,770);
	}
	else if(m_player->GetLife() == 2)
	{
		Render2DMesh(meshList[GEO_HEARTS], false,30,205,770);
		Render2DMesh(meshList[GEO_HEARTS], false,30,235,770);
	}
	else
		Render2DMesh(meshList[GEO_HEARTS], false,30,205,770);

	RenderTextOnScreen(meshList[GEO_TEXT], m_renderString, Color(1,1,1), 2.5, 0, 0);
}

void Scene2D::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	glDisable(GL_DEPTH_TEST);
	std::ostringstream ss;
	const int size = 5;
	if(m_menuStatus != GAME)
	{
		//Render UI
		RenderUI();
		if(m_menuStatus == SCORE)
		{
			for(int i = 0; i < size; ++i)
			{
				ss.str(std::string());
				ss << i+1 << ". " << m_highscore[i];
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 35, 35 - (i*5));
			}
		}
	}

	if(m_menuStatus == GAME)
	{
		// Render the tile map
		RenderTileMap();
		RenderHUD();
	}

	glEnable(GL_DEPTH_TEST);
}

void Scene2D::Exit()
{
	// Cleanup VBO
	for(vector<EnemyIn2D*>::iterator it = m_enemyList.begin(); it != m_enemyList.end(); ++it)
	{
		EnemyIn2D* enemy = (EnemyIn2D*)*it;

		if(enemy != NULL)
		{
			delete enemy;
			enemy = NULL;
		}
	}

	for(vector<Item*>::iterator it = m_itemList.begin(); it != m_itemList.end(); ++it)
	{
		Item* item = (Item*)*it;

		if(item != NULL)
		{
			delete item;
			item = NULL;
		}
	}

	if(TextArray != NULL)
	{
		delete TextArray;
		TextArray = NULL;
	}

	for(vector<Projectile*>::iterator it = m_projectileList.begin(); it != m_projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)*it;

		if(projectile != NULL)
		{
			delete projectile;
			projectile = NULL;
		}
	}

	if(m_player != NULL)
	{
		delete m_player;
		m_player = NULL;
	}

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}

	if(m_cMap != NULL)
	{
		delete m_cMap;
		m_cMap = NULL;
	}

	if(m_cBoundMap != NULL)
	{
		delete m_cBoundMap;
		m_cBoundMap = NULL;
	}

	if(m_cDoorInteractionMap != NULL)
	{
		delete m_cDoorInteractionMap;
		m_cDoorInteractionMap = NULL;
	}

	if(m_cEnemyAndItemMap != NULL)
	{
		delete m_cEnemyAndItemMap;
		m_cEnemyAndItemMap = NULL;
	}

	if(m_backgroundSound != NULL)
	{
		m_backgroundSound->stop();
		m_backgroundSound->drop();
		m_backgroundSound = NULL;
	}

	if(m_eventSound != NULL)
	{
		m_eventSound->stop();
		m_eventSound->drop();
		m_eventSound = NULL;
	}

	if(m_theSoundEngine != NULL)
	{
		m_theSoundEngine->drop();
		m_theSoundEngine = NULL;
	}

	if(m_path != NULL)
	{
		delete m_path;
		m_path = NULL;
	}

	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

/********************************************************************************
Render the tile map. This is a private function for use in this class only
********************************************************************************/
void Scene2D::RenderTileMap()
{
	int m = 0;
	m_cMap->SetmapFineOffset(Vector2((float)((int)(m_cMap->GetmapOffset().x) % m_cMap->GetTileSize())));

	for(int i = 0; i < m_cMap->GetNumOfTiles_Height(); i ++)
	{
		for(int k = 0; k < m_cMap->GetNumOfTiles_Width() + 1; k ++)
		{
			m = (int)m_cMap->GettileOffset().x + k;
			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if(m >= m_cMap->GetNumOfTiles_MapWidth())
			{
				break;
			}
			if (m_cMap->theScreenMap[i][m] == TILE_TOPLEFT)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_TOPLEFT - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_TOPRIGHT)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_TOPRIGHT - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_BOTTOMLEFT)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_BOTTOMLEFT - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_BOTTOMRIGHT)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_BOTTOMRIGHT - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_LEFT)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_LEFT - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_RIGHT)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_RIGHT - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_TOP)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_TOP - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_BOTTOM)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_BOTTOM - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_MIDDLE)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_MIDDLE - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_HIDE_MIDDLE)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_HIDE_MIDDLE - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_EDGE)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_EDGE - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_DOOR)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_DOOR - 1);
				Render2DMesh(tm, false, 32.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
		}
	}

	for(vector<EnemyIn2D*>::iterator it = m_enemyList.begin(); it != m_enemyList.end(); ++it)
	{
		EnemyIn2D* enemy = (EnemyIn2D*)*it;

		if(enemy->GetActive() && enemy->GetCurrentLevel() == m_currentLevel)
		{
			float theEnemy_x = enemy->GetPosition().x;
			float theEnemy_y = enemy->GetPosition().y;

			if(((theEnemy_x >= 0 + m_cMap->GetmapOffset().x) && (theEnemy_x < m_cMap->GetScreenWidth() + m_cMap->GetmapOffset().x)) && ((theEnemy_y >= 0) && (theEnemy_y < m_cMap->GetScreenHeight())))
			{
				Render2DMesh(enemy->GetMesh(), false, enemy->GetScale().x, (16.0f + theEnemy_x) - m_cMap->GetmapOffset().x, (16.0f + theEnemy_y) - m_cMap->GetmapOffset().y);
			}
		}
	}

	for(vector<Item*>::iterator it = m_itemList.begin(); it != m_itemList.end(); ++it)
	{
		Item* item = (Item*)*it;

		if(item->GetActive() && item->GetCurrentLevel() == m_currentLevel)
		{
			float item_x = item->GetPosition().x;
			float item_y = item->GetPosition().y;

			if(((item_x >= 0 + m_cMap->GetmapOffset().x) && (item_x < m_cMap->GetScreenWidth() + m_cMap->GetmapOffset().x)) && ((item_y >= 0) && (item_y < m_cMap->GetScreenHeight())))
			{
				Render2DMesh(item->GetMesh(), false, item->GetScale().x, (item_x) - m_cMap->GetmapOffset().x, (item_y) - m_cMap->GetmapOffset().y);
			}
		}
	}

	for(vector<Projectile*>::iterator it = m_projectileList.begin(); it != m_projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)*it;

		if(projectile->GetActive())
		{
			Render2DMesh(projectile->GetMesh(),false, projectile->GetScale().x, 16.0f + projectile->GetPosition().x, 16.0f + projectile->GetPosition().y);
		}
	}

	if(m_player->GetRenderPlayer())
	{
		Render2DMesh(m_player->GetMesh(), false, m_player->GetScale().x, 16.0f + m_player->GetPosition().x, 16.0f + m_player->GetPosition().y);
	}
}