#include "stdafx.h"
#include "Turret.h"
#include "Character_base.h"
Turret::~Turret()
{
	DeleteGO(m_skinModelRender);

}
bool Turret::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/turret.tkm");
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rot);
	m_skinModelRender->SetScale(m_scale);
	return true;
}
void Turret::Update()
{
	//‚ß‚ñ‚Ç‚­‚³‚¢‚©‚çCharacter_base‚Å‘‚­‚±‚Æ‚É‚µ‚½
}