#include "stdafx.h"
#include "ShowModel.h"

ShowModel::~ShowModel()
{
	DeleteGO(m_skinModelRender);
}

bool ShowModel::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);


	m_lig = NewGO<prefab::CDirectionLight>(0);
	m_lig->SetDirection({ 1.0f,0.0f,0.0f });
	m_lig->SetColor({ 1.0f,1.0f,1.0f });

	m_skinModelRender->Init("Assets/modelData/unityChan.tkm", "Assets/modelData/unityChan.tks",m_lig->GetLigData());

	return true;
}

void ShowModel::Update()
{
	m_position.z += 0.1f;

	m_skinModelRender->SetPosition(m_position);

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_lig->SetDirection({ 0.0f,0.0f,-1.0f });
	}
}