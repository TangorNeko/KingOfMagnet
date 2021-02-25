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
		std::vector<const LevelObjectData*> m_renderObjectDatas;
		prefab::CSkinModelRender* m_modelRender = nullptr;
		PhysicsStaticObject m_physicsStaticObject;

	};

}

