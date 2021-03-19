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
		Skeleton m_skeleton; //スケルトン
		Model m_model[eModel_Num]; //モデル

		Vector3 m_position; //座標
		Quaternion m_qRot; //回転
		Vector3 m_scale; //拡大率

		AnimationClip* m_animationClips;
		int m_animationClipNum;
		Animation m_animation;

		void UpdateModel();
	public:
		CSkinModelRender() :m_position(Vector3::Zero), m_qRot(g_quatIdentity), m_scale(Vector3::One) {}

		void Render(RenderContext& rc,Camera* camera) override;

		void Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum);
		void Init(const char* modelPath,const char* skeletonPath);
		void Init(const char* modelPath);


		//TODO: アニメーションの追加
		void PlayAnimation(int animationNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animationNo, interpolateTime);
			m_animation.Progress(1.0f/60.0f);
		}
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		void SetPosition(Vector3 pos);
		void SetRotation(Quaternion qRot);
		void SetScale(Vector3 scale);
		void SetMatrix(Matrix world)
		{
			for (auto& model : m_model) {
				model.UpdateWorldMatrix(world);
				m_skeleton.Update(world);
			}
		}

		bool GetAnimFlag() { return m_animation.IsPlaying(); }
		Vector3 GetPosition() { return m_position; }
		Quaternion GetRotation() { return m_qRot; }
		Vector3 GetScale() { return m_scale; }
		Model& GetModel(EModel model = eModel_View1) { return m_model[model]; }
		Matrix GetWorldMatrixFromBoneName(const wchar_t* boneName);
	};
}

