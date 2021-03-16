#pragma once

namespace prefab
{
	class CSkinModelRender :public IGameObject
	{
	public:
		enum EModel {
			eModel_View1,
			eModel_View2,
			eModel_Num,
		};
	private:
		Skeleton m_skeleton; //�X�P���g��
		Model m_model[eModel_Num]; //���f��

		Vector3 m_position; //���W
		Quaternion m_qRot; //��]
		Vector3 m_scale; //�g�嗦

		AnimationClip* m_animationClips;
		Animation m_animation;

		void UpdateModel();
	public:
		CSkinModelRender() :m_position(Vector3::Zero), m_qRot(g_quatIdentity), m_scale(Vector3::One) {}

		void Render(RenderContext& rc,Camera* camera) override;

		void Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum);
		void Init(const char* modelPath,const char* skeletonPath);
		void Init(const char* modelPath);


		//TODO: �A�j���[�V�����̒ǉ�
		//void PlayAnimation();
		
		void SetPosition(Vector3 pos);
		void SetRotation(Quaternion qRot);
		void SetScale(Vector3 scale);

		Vector3 GetPosition() { return m_position; }
		Quaternion GetRotation() { return m_qRot; }
		Vector3 GetScale() { return m_scale; }
		Model& GetModel(EModel model = eModel_View1) { return m_model[model]; }
	};
}

