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
		//�쐬���������������B
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
			//�X�L�����f�������_���[���쐬
			m_modelRenders.push_back(NewGO<prefab::CSkinModelRender>(0));
			//�t�@�C���p�X���쐬�B
			char filepath[256];
			sprintf_s(filepath, "Assets/modelData/%s.tkm", m_renderObjectDatas[i]->name);
			m_modelRenders[i]->Init(filepath);

			//TODO:�|�W�V�������S�������ɂȂ��Ă���?�����ƃo���G�[�V�����̂���tkl�t�@�C�����쐬���Č��؂���K�v����B
			m_modelRenders[i]->SetPosition(m_renderObjectDatas[i]->position);
			m_modelRenders[i]->SetRotation(m_renderObjectDatas[i]->rotation);
			m_modelRenders[i]->SetScale(m_renderObjectDatas[i]->scale);

			//�����蔻����쐬�B
			PhysicsStaticObject* m_physicsStaticObject = new PhysicsStaticObject;
			m_physicsStaticObject->CreateFromModel(m_modelRenders[i]->GetModel(), m_modelRenders[i]->GetModel().GetWorldMatrix());
			m_physicsStaticObjects.push_back(m_physicsStaticObject);

			//�e(�\��)
		}

	}
}