#include "stdafx.h"
#include "BackGround.h"

BackGround::~BackGround()
{
	DeleteGO(m_skinModelRender);
}

bool BackGround::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/bg/bg.tkm");

	return true;
}

void BackGround::Update()
{

}