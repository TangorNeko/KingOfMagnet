#pragma once

namespace prefab
{
	//レベルのオブジェクト情報
	struct LevelObjectData
	{
		Vector3 position = Vector3::Zero;			//オブジェクトの座標
		Quaternion rotation = Quaternion::Identity;	//オブジェクトの回転
		Vector3 scale = Vector3::One;				//オブジェクトの拡大率
		const char* name = nullptr;					//オブジェクトの名前
	};

	class CMapChipRender : public IGameObject
	{
	public:
		CMapChipRender();
		~CMapChipRender();

		/**
		 * @brief 初期化
		*/
		void Init();

		/**
		 * @brief モデルのオブジェクト情報を登録
		 * @param objData オブジェクト情報
		*/
		void AddRenderObject(LevelObjectData objData)
		{
			m_renderObjectData = objData;
		}

	private:
		LevelObjectData m_renderObjectData;					//オブジェクトのデータ
		prefab::CSkinModelRender* m_modelRender = nullptr;	//オブジェクトのSkinModelRender
		PhysicsStaticObject m_physicsStaticObject;			//オブジェクトのPhysicsStaticObject
	};

}

