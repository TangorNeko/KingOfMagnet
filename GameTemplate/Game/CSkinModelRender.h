#pragma once

namespace prefab
{
	class CSkinModelRender :public IGameObject
	{
	public:
		enum EModel {
			eModel_View1,
			eModel_View2,
			eModel_Shadow,
			eModel_Num,
		};
		//アニメーション速度
		float m_animation_speed = 1.0;
	private:
		Skeleton m_skeleton; //スケルトン
		Model m_model[eModel_Num]; //モデル

		Vector3 m_position; //座標
		Quaternion m_qRot; //回転
		Vector3 m_scale; //拡大率

		AnimationClip* m_animationClips;
		int m_animationClipNum;
		Animation m_animation;

		bool m_isShadowCaster = false;

		/// <summary>
		/// モデルの情報を更新する。
		/// </summary>
		void UpdateModel();
	public:
		CSkinModelRender() :m_position(Vector3::Zero), m_qRot(g_quatIdentity), m_scale(Vector3::One) {}

		void Render(RenderContext& rc,Camera* camera) override;

		/// <summary>
		/// モデルの初期化関数。アニメーションつき。
		/// </summary>
		/// <param name="modelPath">モデルファイルのパス(.tkm)</param>
		/// <param name="skeletonPath">スケルトンファイルのパス(.tks)</param>
		/// <param name="animationClips">アニメーションクリップの配列</param>
		/// <param name="animationClipNum">アニメーションクリップの数</param>
		void Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum);

		/// <summary>
		/// モデルの初期化関数。
		/// </summary>
		/// <param name="modelPath">モデルファイルのパス(.tkm)</param>
		/// <param name="skeletonPath">スケルトンファイルのパス(.tks)</param>
		void Init(const char* modelPath,const char* skeletonPath);

		/// <summary>
		/// モデルの初期化関数。モデルパスだけ版。
		/// </summary>
		/// <param name="modelPath">モデルファイルのパス(.tkm)</param>
		void Init(const char* modelPath);

		/// <summary>
		/// アニメーションの再生
		/// </summary>
		/// <param name="animationNo">アニメーション番号</param>
		/// <param name="interpolateTime">アニメーションの補完時間</param>
		void PlayAnimation(int animationNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animationNo, interpolateTime);
			m_animation.Progress(m_animation_speed /60.0f);
			UpdateModel();
		}

		/// <summary>
		/// アニメーションを再生しているか?
		/// </summary>
		/// <param name="pos"></param>
		/// <returns>再生している:true 再生していない:false</returns>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// モデルの座標を設定する。
		/// </summary>
		/// <param name="pos">座標(Vector3)</param>
		void SetPosition(Vector3 pos);

		/// <summary>
		/// モデルの回転を設定する。
		/// </summary>
		/// <param name="qRot">回転(Quaternion)</param>
		void SetRotation(Quaternion qRot);

		/// <summary>
		/// モデルの拡大率を設定する。
		/// </summary>
		/// <param name="scale">拡大率(Vector3)</param>
		void SetScale(Vector3 scale);

		/// <summary>
		/// モデルのワールド行列を直接セットする。
		/// </summary>
		/// <param name="world">ワールド行列(Matrix)</param>
		void SetMatrix(Matrix world)
		{
			for (auto& model : m_model) {
				model.UpdateWorldMatrix(world);
				m_skeleton.Update(world);
			}
		}

		void SetShadowCasterFlag(bool isShadowCaster)
		{
			m_isShadowCaster = isShadowCaster;
		}

		/// <summary>
		/// アニメーションの再生フラグを取得
		/// </summary>
		/// <returns>再生している:true 再生していない:false</returns>
		bool GetAnimFlag()
		{ 
			return m_animation.IsPlaying(); 
		}

		/// <summary>
		/// モデルと線分が交差している?
		/// </summary>
		/// <param name="start">線分の始点</param>
		/// <param name="end">線分の終点</param>
		/// <param name="crossPoint">線分の交差点(戻り値、参照受け取り)</param>
		/// <returns>交差しているか(している:true していない:false)</returns>
		bool isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint);

		/// <summary>
		/// モデルの座標を取得
		/// </summary>
		/// <returns>座標(Vector3)</returns>
		Vector3 GetPosition()
		{
			return m_position;
		}

		/// <summary>
		/// モデルの回転を取得
		/// </summary>
		/// <returns>回転量(Quaternion)</returns>
		Quaternion GetRotation()
		{
			return m_qRot;
		}

		/// <summary>
		/// モデルの拡大率を取得
		/// </summary>
		/// <returns>拡大率(Vector3)</returns>
		Vector3 GetScale()
		{
			return m_scale;
		}

		/// <summary>
		/// モデルを取得
		/// </summary>
		/// <param name="model">ビューポート番号(カメラ番号)</param>
		/// <returns>番号から取得したモデルへの参照</returns>
		Model& GetModel(EModel model = eModel_View1)
		{
			return m_model[model];
		}

		/// <summary>
		/// ボーンの名前からそのボーンのワールド行列を取得する。
		/// </summary>
		/// <param name="boneName">ボーンの名前</param>
		/// <returns></returns>
		Matrix GetWorldMatrixFromBoneName(const wchar_t* boneName);

		//事前にモデルをロードしておく。
		void PreLoadModel(const char* tkmFilePath);
	};
}

