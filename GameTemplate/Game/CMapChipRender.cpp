#include "stdafx.h"
#include "CMapChipRender.h"
#include "CLevel.h"

namespace prefab
{
	CMapChipRender::CMapChipRender()
	{

	}

	CMapChipRender::~CMapChipRender()
	{
		DeleteGO(m_modelRender);
	}

	void CMapChipRender::Init()
	{

		m_modelRender = NewGO<prefab::CSkinModelRender>(0);
		//ファイルパスを作成。
		char filepath[256];
		sprintf_s(filepath, "Assets/modelData/%s.tkm", m_renderObjectDatas[0]->name);
		m_modelRender->Init(filepath);
		m_modelRender->SetPosition(m_renderObjectDatas[0]->position);
		m_modelRender->SetRotation(m_renderObjectDatas[0]->rotation);
		m_modelRender->SetScale(m_renderObjectDatas[0]->scale);

		Vector3 a = m_modelRender->GetPosition();
		a.y += 25;
		a.z += 500;
		m_modelRender->SetPosition(a);

		//当たり判定。
		m_physicsStaticObject.CreateFromModel(m_modelRender->GetModel(), m_modelRender->GetModel().GetWorldMatrix());

		//影

	}
}