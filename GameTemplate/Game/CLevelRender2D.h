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
		/// レベル2Dの初期化
		/// </summary>
		/// <param name="filePath">Caslファイルのパス</param>
		/// <param name="hookFunc">フック関数</param>
		void Init(const char* filePath, std::function<bool(Level2DObjectData& objData)> hookFunc)
		{
			m_level2D.Init(filePath, hookFunc);
		}

		/// <summary>
		/// 毎フレームm_level2DのUpdate関数を呼ぶ。
		/// </summary>
		void Update()override
		{
			m_level2D.Update();
		}

		/// <summary>
		/// レンダーステップが2DLevel用の時のみ描画する。
		/// PostRenderなのでブルーム等のエフェクトの影響を受けない。
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="camera">カメラ</param>
		void PostRender(RenderContext& rc, Camera* camera)
		{
			if (rc.GetRenderStep() == RenderContext::eStep_RenderAllScreen)
			{
				m_level2D.Draw(rc);
			}
		}
	};
}

