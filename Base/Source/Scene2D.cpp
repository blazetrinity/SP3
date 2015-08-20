#include "Scene2D.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

static const float MAXGHOSTQUEUETIMER = 15.0f;

Scene2D::Scene2D()
	: m_cMap(NULL)
{
}

Scene2D::~Scene2D()
{
	if (m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}
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
	meshList[GEO_TEXT]->textureID[0] = LoadTGA("Image//calibri.tga");

	// Load the ground mesh and texture
	meshList[GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 1024, 800);
	meshList[GEO_BACKGROUND]->textureID[0] = LoadTGA("Image//2DBackGround.tga");

	meshList[GEO_ENEMY1] = MeshBuilder::GenerateSpriteAnimation("GEO_ENEMY1", 4, 3);
	meshList[GEO_ENEMY1]->textureID[0] = LoadTGA("Image//enemy1_sprite.tga");
	meshList[GEO_ENEMY2] = MeshBuilder::GenerateSpriteAnimation("GEO_ENEMY2", 4, 3);
	meshList[GEO_ENEMY2]->textureID[0] = LoadTGA("Image//enemy2_sprite.tga");

	meshList[GEO_CHARACTER_TEST] = MeshBuilder::GenerateSpriteAnimation("GEO_CHARACTER_TEST", 4, 3);
	meshList[GEO_CHARACTER_TEST]->textureID[0] = LoadTGA("Image//character_sprite.tga");
	meshList[GEO_TILEMAP] = MeshBuilder::GenerateTileMap("GEO_TILEMAP", 2, 3);
	meshList[GEO_TILEMAP]->textureID[0] = LoadTGA("Image//TileMap.tga");

	meshList[GEO_BULLET] = MeshBuilder::Generate2DMesh("GEO_BULLET", Color(1, 1, 1), 0, 0, 1, 1);
	meshList[GEO_BULLET]->textureID[0] = LoadTGA("Image//bullet.tga");

	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(800, 1024, 25, 32, 800 ,1024);
	m_cMap->LoadMap( "Image//level1_visual.csv" );

	m_cBoundMap = new CMap();
	m_cBoundMap->Init(800, 1024, 25, 32, 800 ,1024);
	m_cBoundMap->LoadMap( "Image//level1_bound.csv" );

	m_cDoorInteractionMap = new CMap();
	m_cDoorInteractionMap->Init(800, 1024, 25, 32, 800 ,1024);
	m_cDoorInteractionMap->LoadMap( "Image//level1_door.csv" );

	//Init Enemy Ai position and animations
	m_cEnemyAndItemMap = new CMap();
	m_cEnemyAndItemMap->Init(800, 1024, 25, 32, 800, 1024);

	for(int i = 1; i < NUM_LEVELS; ++i)
	{
		switch(i)
		{
		case 1:	m_cEnemyAndItemMap->LoadMap( "Image//Level1EnemyAndItem.csv" );
			break;
		case 2:	m_cEnemyAndItemMap->LoadMap( "Image//Level2EnemyAndItem.csv" );
			break;
		case 3:	m_cEnemyAndItemMap->LoadMap( "Image//Level3EnemyAndItem.csv" );
			break;
		}

		SpriteAnimation *whiteGhostSpriteAnimation = (dynamic_cast<SpriteAnimation*>(meshList[GEO_ENEMY2]));
		SpriteAnimation *redGhostSpriteAnimation = (dynamic_cast<SpriteAnimation*>(meshList[GEO_ENEMY1]));

		for(int j = 0; j < m_cEnemyAndItemMap->GetNumOfTiles_MapHeight(); j ++)
		{
			for(int k = 0; k < m_cEnemyAndItemMap->GetNumOfTiles_MapWidth(); k ++)
			{
				if(m_cEnemyAndItemMap->theScreenMap[j][k] == EnemyIn2D::WHITE_GHOST_PATROL_UPDOWN)
				{
					SpriteAnimation *newSpriteAnimation = whiteGhostSpriteAnimation;

					EnemyIn2D* theEnemy = new EnemyIn2D;
					theEnemy->Init(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32), 10, i, newSpriteAnimation, EnemyIn2D::WHITE_GHOST_PATROL_UPDOWN, 1, NULL);

					theEnemy->SetAnimation(EnemyIn2D::IDLE_RIGHT, 7, 7, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_LEFT, 4, 4, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_UP, 10, 10, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_DOWN, 1, 1, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::WALK_RIGHT, 6, 8, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_LEFT, 3, 5, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_UP, 9, 11, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_DOWN, 0, 2, 0, 0.5);
					theEnemy->ChangeAnimation(EnemyIn2D::IDLE_RIGHT);

					Strategy_Patrol* theStrategy = new Strategy_Patrol;
					theEnemy->ChangeStrategy(theStrategy);
					m_enemyList.push_back(theEnemy);
				}

				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == EnemyIn2D::WHITE_GHOST_PATROL_LEFTRIGHT)
				{
					SpriteAnimation* newSpriteAnimation = whiteGhostSpriteAnimation;

					EnemyIn2D* theEnemy = new EnemyIn2D;
					theEnemy->Init(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32), 10, i, newSpriteAnimation, EnemyIn2D::WHITE_GHOST_PATROL_LEFTRIGHT, 1, NULL);

					theEnemy->SetAnimation(EnemyIn2D::IDLE_RIGHT, 7, 7, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_LEFT, 4, 4, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_UP, 10, 10, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_DOWN, 1, 1, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::WALK_RIGHT, 6, 8, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_LEFT, 3, 5, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_UP, 9, 11, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_DOWN, 0, 2, 0, 0.5);
					theEnemy->ChangeAnimation(EnemyIn2D::IDLE_RIGHT);

					Strategy_Patrol* theStrategy = new Strategy_Patrol;
					theEnemy->ChangeStrategy(theStrategy);
					m_enemyList.push_back(theEnemy);
				}

				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == EnemyIn2D::RED_GHOST_PATROL_UPDOWN)
				{
					SpriteAnimation* newSpriteAnimation = redGhostSpriteAnimation;

					EnemyIn2D* theEnemy = new EnemyIn2D;
					theEnemy->Init(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32), 10, i, newSpriteAnimation, EnemyIn2D::RED_GHOST_PATROL_UPDOWN, 1, NULL);

					theEnemy->SetAnimation(EnemyIn2D::IDLE_RIGHT, 7, 7, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_LEFT, 4, 4, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_UP, 10, 10, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_DOWN, 1, 1, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::WALK_RIGHT, 6, 8, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_LEFT, 3, 5, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_UP, 9, 11, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_DOWN, 0, 2, 0, 0.5);
					theEnemy->ChangeAnimation(EnemyIn2D::IDLE_RIGHT);

					Strategy_Patrol* theStrategy = new Strategy_Patrol;
					theEnemy->ChangeStrategy(theStrategy);
					m_enemyList.push_back(theEnemy);
				}

				else if(m_cEnemyAndItemMap->theScreenMap[j][k] == EnemyIn2D::RED_GHOST_PATROL_LEFTRIGHT)
				{
					SpriteAnimation* newSpriteAnimation = redGhostSpriteAnimation;

					EnemyIn2D* theEnemy = new EnemyIn2D;
					theEnemy->Init(Vector2((float)k * m_cEnemyAndItemMap->GetTileSize(), (float)(m_cEnemyAndItemMap->GetScreenHeight() -  ((j * m_cEnemyAndItemMap->GetTileSize()) +  m_cEnemyAndItemMap->GetTileSize()))), Vector2(32, 32), 10, i, newSpriteAnimation, EnemyIn2D::RED_GHOST_PATROL_LEFTRIGHT, 1, NULL);

					theEnemy->SetAnimation(EnemyIn2D::IDLE_RIGHT, 7, 7, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_LEFT, 4, 4, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_UP, 10, 10, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::IDLE_DOWN, 1, 1, 0, 1);
					theEnemy->SetAnimation(EnemyIn2D::WALK_RIGHT, 6, 8, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_LEFT, 3, 5, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_UP, 9, 11, 0, 0.5);
					theEnemy->SetAnimation(EnemyIn2D::WALK_DOWN, 0, 2, 0, 0.5);
					theEnemy->ChangeAnimation(EnemyIn2D::IDLE_RIGHT);

					Strategy_Patrol* theStrategy = new Strategy_Patrol;
					theEnemy->ChangeStrategy(theStrategy);
					m_enemyList.push_back(theEnemy);
				}
			}
		}
	}

	// Init Player position and animation
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_CHARACTER_TEST]);
	Skill* skill = new Skill();
	skill->Init(5.0f, 20.f, 1.0f, true, Tag::PLAYER);

	m_player = new PlayerIn2D();
	m_player->Init(Vector2(20, 20), Vector2(32, 32), 10, 1, skill);
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
	m_sounds[SND_GHOST_QUEUE] = m_theSoundEngine->addSoundSourceFromFile("wav//ghost_queue.wav");
	m_sounds[SND_FIRE] = m_theSoundEngine->addSoundSourceFromFile("wav//fire.wav");
	m_sounds[SND_DAMAGE] = m_theSoundEngine->addSoundSourceFromFile("wav//damage.wav");
	m_sounds[SND_BOSS] = m_theSoundEngine->addSoundSourceFromFile("wav//boss.wav");
	//m_sounds[SND_BOSS_ATTACK] = m_theSoundEngine->addSoundSourceFromFile("wav//boss_attack.wav");

	m_backgroundSound = m_theSoundEngine->play2D(m_sounds[SND_BACKGROUND], true, false, true);
	m_currentSound = SND_BACKGROUND;

	// Init game element variables
	m_levelCompleted = false;
	m_currentLevel = 1;
	m_updateMap = false; 
	m_ghostQueueTimer = MAXGHOSTQUEUETIMER / m_currentLevel;
	m_spawnGhost = false;
	m_ghostTriggered = false;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	bLightEnabled = true;
}

void Scene2D::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Update the hero
	int checkPosition_X = (int) ((m_cMap->GetmapOffset().x + m_player->GetPosition().x) / m_cMap->GetTileSize());
	int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float)(m_player->GetPosition().y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());

	//Check if player is going into the next level
	UpdateLevel(checkPosition_X, checkPosition_Y);

	//Player update
	UpdatePlayer(dt);

	//Ai Update and collision with character
	UpdateEnemy(dt);

	//Projectile Update
	UpdateProjectile(dt);

	//Up ghost trigger
	if(!m_ghostTriggered && !m_levelCompleted)
	{
		m_ghostQueueTimer -= (float)dt;

		if(m_ghostQueueTimer <= 0)
		{
			if(m_currentSound != SND_GHOST_QUEUE)
			{
				m_backgroundSound->stop();
				m_backgroundSound->drop();
				m_backgroundSound = NULL;
				m_backgroundSound = m_theSoundEngine->play2D(m_sounds[SND_GHOST_QUEUE], false, false, true);
				m_currentSound = SND_GHOST_QUEUE;
			}

			if(m_backgroundSound->isFinished() == true)
			{
				// Check if player hiding
				//if(playerhiding)
				//{
				m_spawnGhost = true;
				//}

				//m_ghostQueueTimer = MAXGHOSTQUEUETIMER / m_currentLevel;
				m_backgroundSound->stop();
				m_backgroundSound->drop();
				m_backgroundSound = NULL;
				m_backgroundSound = m_theSoundEngine->play2D(m_sounds[SND_BACKGROUND], true, false, true);
				m_currentSound = SND_BACKGROUND;
			}
		}

		if(m_spawnGhost)
		{
			for(vector<EnemyIn2D*>::iterator it = m_enemyList.begin(); it != m_enemyList.end(); ++it)
			{
				EnemyIn2D* enemy = (EnemyIn2D*)*it;
				if(enemy->GetActive() == false && enemy->GetCurrentLevel() == m_currentLevel)
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
			projectile->Init(m_player->GetSkill(),m_player->GetPosition(), m_player->GetFacingNormal(), meshList[GEO_BULLET], m_currentLevel);
		}
	}

	//Update the player velocity and position
	m_player->Update(m_cBoundMap, dt, true);
}

void Scene2D::UpdateEnemy(double dt)
{
	for(vector<EnemyIn2D*>::iterator it = m_enemyList.begin(); it !=  m_enemyList.end(); ++it)
	{
		EnemyIn2D *enemy = (EnemyIn2D *) *it;

		if(enemy->GetActive() && enemy->GetCurrentLevel() == m_currentLevel)
		{
			if(((enemy->GetPosition().x - m_cMap->GetmapOffset().x) >= 0) && ((enemy->GetPosition().x - m_cMap->GetmapOffset().x) < m_cMap->GetScreenWidth()) && ((enemy->GetPosition().y >= 0) && (enemy->GetPosition().y < m_cMap->GetScreenHeight())))
			{
				/*float distance = (m_player->GetPos() - (m_enemyList[i]->GetPos() - m_cMap->GetmapOffset().x)).Length();
				Vector3 FacingNormal = ((m_enemyList[i]->GetPos() - m_cMap->GetmapOffset().x) - m_player->GetPos()).Normalize();

				if((distance) < m_cMap->GetTileSize())
				{
				if(FacingNormal.x < 0 && ((m_player->GetAnimation() == CCharacterIn2D::ATTACK_LEFT) || (m_player->GetAnimation() == CCharacterIn2D::JUMPATTACK_LEFT)))
				{
				m_enemyList[i]->SetActive(false);
				continue;
				}
				else if(FacingNormal.x > 0 && ((m_player->GetAnimation() == CCharacterIn2D::ATTACK_RIGHT) || (m_player->GetAnimation() == CCharacterIn2D::JUMPATTACK_RIGHT)))
				{
				m_enemyList[i]->SetActive(false);
				continue;
				}
				else if(!m_invunerable)
				{
				m_invunerable = true;
				m_invunerableTimer = 5;
				m_renderCharacter = false;
				}
				}*/
			}

			enemy->Update(m_cBoundMap, dt, true);
		}
	}
}

void Scene2D::UpdateProjectile(double dt)
{
	for(vector<Projectile*>::iterator it = m_projectileList.begin(); it != m_projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)*it;

		if(projectile->GetActive() && projectile->GetLevel() != m_currentLevel)
		{
			projectile->SetActive(false);
		}

		if(projectile->GetActive())
		{
			projectile->Update(m_cBoundMap, dt);
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
			m_cMap->LoadMap( "Image//level1_visual.csv" );
			m_cBoundMap->LoadMap( "Image//level1_bound.csv" );
			m_cDoorInteractionMap->LoadMap( "Image//level1_door.csv" );
		}
		else if(m_currentLevel == LEVEL2)
		{
			m_cMap->LoadMap( "Image//level2_visual.csv" );
			m_cBoundMap->LoadMap( "Image//level2_bound.csv" );
			m_cDoorInteractionMap->LoadMap( "Image//level2_door.csv" );
		}
		else if(m_currentLevel == LEVEL3)
		{
			m_cMap->LoadMap( "Image//level3_visual.csv" );
			m_cBoundMap->LoadMap( "Image//level3_bound.csv" );
			m_cDoorInteractionMap->LoadMap( "Image//level3_door.csv" );
		}
		else if(m_currentLevel == LEVEL4)
		{
			m_cMap->LoadMap( "Image//level4_visual.csv" );
			m_cBoundMap->LoadMap( "Image//level4_bound.csv" );
			m_cDoorInteractionMap->LoadMap( "Image//level4_door.csv" );
		}

		m_player->CalPosition((m_cMap->GetTileSize()), (m_cMap->GetScreenWidth() - (2 * m_cMap->GetTileSize())), (m_cMap->GetTileSize()), (m_cMap->GetScreenHeight() - (2 * m_cMap->GetTileSize())), (float)m_cMap->GetTileSize());
		m_updateMap = false;
	}
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

/********************************************************************************
Update Camera position
********************************************************************************/
void Scene2D::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
Update Weapon status
********************************************************************************/
void Scene2D::UpdateWeaponStatus(const unsigned char key)
{

}

void Scene2D::StartGame()
{

}

static const float SKYBOXSIZE = 1000.f;

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
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
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

	// Render the background image
	RenderBackground();

	// Render the tile map
	RenderTileMap();

	glEnable(GL_DEPTH_TEST);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	std::ostringstream ss1;
	ss1.precision(5);
	ss1 << "tileOffset_x: " << m_cMap->GettileOffset().x;
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 35, 6);

	std::ostringstream ss2;
	ss2.precision(5);
	ss2 << "mapOffset_x: " << m_cMap->GetmapOffset().x;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 35, 3);

	RenderTextOnScreen(meshList[GEO_TEXT], "Hello Screen", Color(0, 1, 0), 3, 0, 0);
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

	if(m_theSoundEngine != NULL)
	{
		m_theSoundEngine->drop();
		m_theSoundEngine = NULL;
	}

	if(m_backgroundSound != NULL)
	{
		m_backgroundSound->drop();
		m_backgroundSound = NULL;
	}

	if(m_eventSound != NULL)
	{
		m_eventSound->drop();
		m_eventSound = NULL;
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

			if (m_cMap->theScreenMap[i][m] == TILE_GROUND)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_GROUND - 1);
				Render2DMesh(tm, false, 30.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_ROOF)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_ROOF - 1);
				Render2DMesh(tm, false, 30.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_LEFTWALL)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_LEFTWALL - 1);
				Render2DMesh(tm, false, 30.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_RIGHTWALL)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_RIGHTWALL - 1);
				Render2DMesh(tm, false, 30.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_LADDER)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_LADDER - 1);
				Render2DMesh(tm, false, 30.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
			}
			else if(m_cMap->theScreenMap[i][m] == TILE_DOOR)
			{
				TileMap *tm = dynamic_cast<TileMap*>(meshList[GEO_TILEMAP]);
				tm->SetTileID(TILE_DOOR - 1);
				Render2DMesh(tm, false, 30.0f, (k + 0.5f) * m_cMap->GetTileSize() - m_cMap->GetmapFineOffset().x, (m_cMap->GetScreenHeight() - m_cMap->GetTileSize()) - (i - 0.5f) * m_cMap->GetTileSize());
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

	for(vector<Projectile*>::iterator it = m_projectileList.begin(); it != m_projectileList.end(); ++it)
	{
		Projectile* projectile = (Projectile*)*it;

		if(projectile->GetActive())
		{
			Render2DMesh(projectile->GetMesh(),false, projectile->GetScale().x, 0.0f + projectile->GetPosition().x, 0.0f + projectile->GetPosition().y);
		}
	}

	if(m_player->GetRenderPlayer())
	{
		Render2DMesh(m_player->GetMesh(), false, m_player->GetScale().x, 16.0f + m_player->GetPosition().x, 16.0f + m_player->GetPosition().y);
	}
}