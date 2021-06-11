#pragma once
#include "Level2D.h"

namespace prefab
{
	class CLevelRender2D : public IGameObject
	{
	public:
		
		/**
		 * @brief レベル2Dの初期化
		 * @param filePath Caslファイルのパス
		 * @param hookFunc フック関数
		*/
		void Init(const char* filePath, std::function<bool(Level2DObjectData& objData)> hookFunc)
		{
			m_level2D.Init(filePath, hookFunc);
		}
		
		/**
		 * @brief 毎フレームm_level2DのUpdate関数を呼ぶ
		*/
		void Update()override
		{
			m_level2D.Update();
		}
		
		/**
		 * @brief レンダーステップが2DLevelの時のみ描画する
		 * PostRenderなのでブルーム等のエフェクトの影響を受けない
		 * @param rc レンダーコンテキスト
		 * @param camera 描画に使用するカメラ
		*/
		void PostRender(RenderContext& rc, Camera* camera)
		{
			if (rc.GetRenderStep() == RenderContext::eStep_RenderAllScreen)
			{
				m_level2D.Draw(rc);
			}
		}

	private:
		Level2D m_level2D;	//2Dレベル
	};
}

