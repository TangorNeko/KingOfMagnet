#pragma once

class BackGround;
#include "MyCapsuleCollider.h"
class Psychokinesis:public IGameObject
{
	bool Start();
	void Update();
	~Psychokinesis();
public:
	Vector3 m_position[6] =
	{
		{ -20.0f,50.0f,float(rand() % 50)},
		{  20.0f,50.0f,float(rand() % 50)},
		{ -30.0f,30.0f,float(rand() % 50)},
		{  30.0f,30.0f,float(rand() % 50)},
		{ -15.0f,20.0f,float(rand() % 50)},
		{  15.0f,20.0f,float(rand() % 50)}
	};
	
	prefab::CSkinModelRender* m_skinModelRender[6] =
	{ 
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr 
	};
	
	

	Vector3 m_playerpos;
	int m_playerNum;
	int m_chargelevel;
	float m_charge;

	float m_UpDown;
	bool m_Up_On=false;

	int m_loopcount = 0;
	bool m_level2 = false;
	bool m_level3 = false;
	bool m_isOnce2 = true;
	bool m_isOnce3 = true;
	Vector3 toPos[6] =
	{ 
		{m_position[0]},
		{m_position[1]},
		{m_position[2]},
		{m_position[3]},
		{m_position[4]},
		{m_position[5]}
	};
	

	Quaternion qRotY;
	Quaternion m_rot;
	Quaternion m_rotY;
	Vector3 m_Dir;


	Vector3 m_moveDirection = { 0.0f,0.0f,0.0f };
	Vector3 m_moveSpeed = { 0.0f,0.0f,0.0f };

	Vector3 oldPos[6];
	MyCapsuleCollider m_collider[6];
	float m_velocity = 1.0f;
	//prefab::CSkinModelRender* m_skinModelRender = nullptr;
	//MyCapsuleCollider m_collider;

	prefab::CSkinModelRender* m_oldbullet = nullptr;
	prefab::CPointLight* m_pointLight = nullptr;
	int m_liveCount = 0;
	int m_parentNo = 0;
	bool m_isAffectedFromEnemyPower = false;
	bool m_Psycho_on = false;
	bool m_shot_on = false;

	BackGround* m_stageModel = nullptr;
};

