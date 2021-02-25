#pragma once

namespace prefab
{
	struct LevelObjectData;
	class CMapChipRender : public IGameObject
	{
	public:
		CMapChipRender();
		~CMapChipRender();

		void Init();

		void QueryRenderObjDatas(std::function<void(const LevelObjectData& objData)> queryFunc)
		{
			for (const auto& renderObjectData : m_renderObjectDatas)
			{
				queryFunc(*renderObjectData);
			}
		}

		void AddRenderObject(const LevelObjectData& objData)
		{
			m_renderObjectDatas.push_back(&objData);
		}

	private:
		//オブジェクトのデータの可変長配列
		std::vector<const LevelObjectData*> m_renderObjectDatas;

		//オブジェクトのデータの個数分のSkinModelRender
		std::vector<prefab::CSkinModelRender*> m_modelRenders;

		//オブジェクトのデータの個数分のPhysicsStaticObject
		std::vector<PhysicsStaticObject*> m_physicsStaticObjects;

	};

}

