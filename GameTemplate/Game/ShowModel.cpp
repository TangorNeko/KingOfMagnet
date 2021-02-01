#include "stdafx.h"
#include "ShowModel.h"

ShowModel::~ShowModel()
{
	DeleteGO(m_skinModelRender);
}

bool ShowModel::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->Init("Assets/modelData/unityChan.tkm", "Assets/modelData/unityChan.tks");

	return true;
}

void ShowModel::Update()
{
	m_position.z += 0.1f;

	m_skinModelRender->SetPosition(m_position);
}