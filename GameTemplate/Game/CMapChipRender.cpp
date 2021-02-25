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
		//作成した分だけ消す。
		for (auto modelRender : m_modelRenders)
		{
			DeleteGO(modelRender);
		}

		for (auto mphysicsStaticObject : m_physicsStaticObjects)
		{
			delete mphysicsStaticObject;
		}
	}

	void CMapChipRender::Init()
	{

		for (int i = 0; i < m_renderObjectDatas.size(); i++)
		{
			//スキンモデルレンダラーを作成
			m_modelRenders.push_back(NewGO<prefab::CSkinModelRender>(0));
			//ファイルパスを作成。
			char filepath[256];
			sprintf_s(filepath, "Assets/modelData/%s.tkm", m_renderObjectDatas[i]->name);
			m_modelRenders[i]->Init(filepath);

			//TODO:ポジションが全部同じになっている?もっとバリエーションのあるtklファイルを作成して検証する必要あり。
			m_modelRenders[i]->SetPosition(m_renderObjectDatas[i]->position);
			m_modelRenders[i]->SetRotation(m_renderObjectDatas[i]->rotation);
			m_modelRenders[i]->SetScale(m_renderObjectDatas[i]->scale);

			//当たり判定を作成。
			PhysicsStaticObject* m_physicsStaticObject = new PhysicsStaticObject;
			m_physicsStaticObject->CreateFromModel(m_modelRenders[i]->GetModel(), m_modelRenders[i]->GetModel().GetWorldMatrix());
			m_physicsStaticObjects.push_back(m_physicsStaticObject);

			//影(予定)
		}

	}
}