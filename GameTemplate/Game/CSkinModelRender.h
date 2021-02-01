#pragma once

namespace prefab
{
	class CSkinModelRender :public IGameObject
	{
	private:
		Skeleton m_skeleton; //�X�P���g��
		Model m_model; //���f��

		Vector3 m_position; //���W
		Quaternion m_qRot; //��]
		Vector3 m_scale; //�g�嗦

		void UpdateModel();
	public:
		CSkinModelRender() :m_position(Vector3::Zero), m_qRot(g_quatIdentity), m_scale(Vector3::One) {}

		void Render(RenderContext& rc) override;

		void Init(const char* modelPath,const char* skeletonPath);


		//TODO: �A�j���[�V�����̒ǉ�
		//void PlayAnimation();
		
		void SetPosition(Vector3 pos);
		void SetRotation(Quaternion qRot);
		void SetScale(Vector3 scale);
	};
}

