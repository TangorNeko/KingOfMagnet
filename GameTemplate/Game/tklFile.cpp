#include "stdafx.h"
#include "tklFile.h"

bool tklFile::Load(const char* levelFilePath)
{
	//.tklファイルをオープン
	FILE* fp = fopen(levelFilePath, "rb");

	//開くのに失敗したらfalseを返す
	if (fp == nullptr)
	{
		return false;
	}

	//tklファイルのバージョンを取得
	fread(&m_tklVersion, sizeof(m_tklVersion), 1, fp);
	//骨の数を取得
	fread(&m_numBone, sizeof(m_numBone), 1, fp);

	//骨の数だけオブジェクトのVectorを拡張する
	m_objects.resize(m_numBone);

	for (int i = 0;i < m_numBone;i++)
	{
		//骨の名前の文字数を取得
		char numBoneName = 0;
		fread(&numBoneName, sizeof(numBoneName), 1, fp);
		auto& object = m_objects.at(i);
		//骨の名前を取得
		object.boneName = std::make_unique<char[]>(numBoneName + 1);
		fread(object.boneName.get(), numBoneName + 1, 1, fp);

		//親の骨の番号を取得
		fread(&object.parentBoneNo, sizeof(object.parentBoneNo), 1, fp);

		//骨の番号を取得
		object.boneNo = i;

		//バインドポーズの行列を取得
		fread(&object.bindPose, sizeof(object.bindPose), 1, fp);

		//バインドポーズの逆行列を取得
		fread(&object.invBindPose, sizeof(object.invBindPose), 1, fp);

		//シャドウキャスターフラグを取得
		fread(&object.ShadowCasterFlag, 1, 1, fp);

		//シャドウレシーバーフラグを取得
		fread(&object.ShadowReceiverFlag, 1, 1, fp);

		//intパラメーターの数を取得
		int numIntData = 0;
		fread(&numIntData, sizeof(numIntData), 1, fp);

		//intパラメーターの数だけintパラメーターを取得
		for (int j = 0;j < numIntData;j++)
		{
			int Data = 0;
			fread(&Data, sizeof(Data), 1, fp);

			object.intData.push_back(Data);
		}

		//floatパラメーターの数を取得
		int numFloatData = 0;
		fread(&numFloatData, sizeof(numFloatData), 1, fp);

		//floatパラメーターの数だけfloatパラメーターを取得
		for (int j = 0;j < numFloatData;j++)
		{
			float Data = 0.0f;
			fread(&Data, sizeof(Data), 1, fp);

			object.floatData.push_back(Data);
		}

		//stringパラメーターの数を取得
		int numStringData = 0;
		fread(&numStringData, sizeof(numStringData), 1, fp);

		//stringパラメーターの数だけstringパラメーターを取得
		std::vector<int> stringDataSize;
		for (int j = 0;j < numStringData;j++)
		{
			int DataSize = 0;
			fread(&DataSize, sizeof(DataSize), 1, fp);

			stringDataSize.push_back(DataSize);

			char* Data = nullptr;
			fread(&Data, sizeof(Data), 1, fp);

			object.stringData.push_back(Data);

		}

		//Vector3データの数を取得
		int numVector3Data = 0;
		fread(&numVector3Data, sizeof(numVector3Data), 1, fp);

		//Vector3データの数だけVector3データを取得
		for (int j = 0;j < numVector3Data;j++)
		{
			Vector3 Data = {0.0f,0.0f,0.0f};
			fread(&Data, sizeof(Data), 1, fp);

			object.vector3Data.push_back(Data);
		}

	}

	return true;
}
