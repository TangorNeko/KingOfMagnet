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
		DeleteGO(m_modelRender);

		/*
		for (auto mphysicsStaticObject : m_physicsStaticObjects)
		{
			delete mphysicsStaticObject;
		}
		*/
	}

	void CMapChipRender::Init()
	{

			//�X�L�����f�������_���[���쐬
			m_modelRender = NewGO<prefab::CSkinModelRender>(0);
			//�t�@�C���p�X���쐬�B
			char filepath[256];
			sprintf_s(filepath, "Assets/modelData/%s.tkm", m_renderObjectData.name);
			m_modelRender->Init(filepath);

			//TODO:�|�W�V�������S�������ɂȂ��Ă���?�����ƃo���G�[�V�����̂���tkl�t�@�C�����쐬���Č��؂���K�v����B
			m_modelRender->SetPosition(m_renderObjectData.position);
			m_modelRender->SetRotation(m_renderObjectData.rotation);
			m_modelRender->SetScale(m_renderObjectData.scale);

			//�����蔻����쐬�B
			m_physicsStaticObject.CreateFromModel(m_modelRender->GetModel(), m_modelRender->GetModel().GetWorldMatrix());

			//�e(�\��)

	}
}