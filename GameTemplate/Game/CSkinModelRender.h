#pragma once

namespace prefab
{
	class CSkinModelRender :public IGameObject
	{
	private:
		Skeleton m_skeleton; //スケルトン
		Model m_model; //モデル

		Vector3 m_position; //座標
		Quaternion m_qRot; //回転
		Vector3 m_scale; //拡大率

		void UpdateModel();
	public:
		CSkinModelRender() :m_position(Vector3::Zero), m_qRot(g_quatIdentity), m_scale(Vector3::One) {}

		void Render(RenderContext& rc) override;

		void Init(const char* modelPath,const char* skeletonPath);


		//TODO: アニメーションの追加
		//void PlayAnimation();
		
		void SetPosition(Vector3 pos);
		void SetRotation(Quaternion qRot);
		void SetScale(Vector3 scale);
	};
}

