#pragma once
#include "Level2D.h"

namespace prefab
{
	class CLevelRender2D : public IGameObject
	{
	private:
		Level2D m_level2D;
	public:

		/// <summary>
		/// ���x��2D�̏�����
		/// </summary>
		/// <param name="filePath">Casl�t�@�C���̃p�X</param>
		/// <param name="hookFunc">�t�b�N�֐�</param>
		void Init(const char* filePath, std::function<bool(Level2DObjectData& objData)> hookFunc)
		{
			m_level2D.Init(filePath, hookFunc);
		}

		/// <summary>
		/// ���t���[��m_level2D��Update�֐����ĂԁB
		/// </summary>
		void Update()override
		{
			m_level2D.Update();
		}

		/// <summary>
		/// �����_�[�X�e�b�v��2DLevel�p�̎��̂ݕ`�悷��B
		/// PostRender�Ȃ̂Ńu���[�����̃G�t�F�N�g�̉e�����󂯂Ȃ��B
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		/// <param name="camera">�J����</param>
		void PostRender(RenderContext& rc, Camera* camera)
		{
			if (rc.GetRenderStep() == RenderContext::eStep_RenderAllScreen)
			{
				m_level2D.Draw(rc);
			}
		}
	};
}

