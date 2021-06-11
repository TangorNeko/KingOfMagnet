#pragma once
#include "Level2D.h"

namespace prefab
{
	class CLevelRender2D : public IGameObject
	{
	public:
		
		/**
		 * @brief ���x��2D�̏�����
		 * @param filePath Casl�t�@�C���̃p�X
		 * @param hookFunc �t�b�N�֐�
		*/
		void Init(const char* filePath, std::function<bool(Level2DObjectData& objData)> hookFunc)
		{
			m_level2D.Init(filePath, hookFunc);
		}
		
		/**
		 * @brief ���t���[��m_level2D��Update�֐����Ă�
		*/
		void Update()override
		{
			m_level2D.Update();
		}
		
		/**
		 * @brief �����_�[�X�e�b�v��2DLevel�̎��̂ݕ`�悷��
		 * PostRender�Ȃ̂Ńu���[�����̃G�t�F�N�g�̉e�����󂯂Ȃ�
		 * @param rc �����_�[�R���e�L�X�g
		 * @param camera �`��Ɏg�p����J����
		*/
		void PostRender(RenderContext& rc, Camera* camera)
		{
			if (rc.GetRenderStep() == RenderContext::eStep_RenderAllScreen)
			{
				m_level2D.Draw(rc);
			}
		}

	private:
		Level2D m_level2D;	//2D���x��
	};
}

