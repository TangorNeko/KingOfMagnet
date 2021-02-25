#pragma once
class tklFile
{
public:
	struct SObject
	{
		std::unique_ptr<char[]> boneName;	//ボーンの名前。
		int parentBoneNo;					//親のボーン番号
		int boneNo;							//ボーン番号
		Vector3 bindPose[4];				//バインドポーズ行列
		Vector3 invBindPose[4];				//バインドポーズ逆行列
		bool ShadowCasterFlag;				//シャドウキャスターフラグ(未使用)
		bool ShadowReceiverFlag;			//シャドウレシーバーフラグ(未使用)
		std::vector<int> intData;			//intの追加データ
		std::vector<float> floatData;		//floatの追加データ
		std::vector<char*> stringData;		//文字列の追加データ
		std::vector<Vector3> vector3Data;	//Vector3の追加データ
	};

	//SObjectのユニークポインタ
	typedef std::unique_ptr<SObject> CObject;

	//SObjectに対して共通の処理を行う
	void QueryObject(std::function<void(SObject& obj)>query)
	{
		for (auto& obj : m_objects)
		{
			query(obj);
		}
	}

	//.tklファイルを読み込んで内容をSObjectの可変長配列m_objectsに書き込む
	bool Load(const char* levelFilePath);
//private:
	//tklファイルのバージョン
	int m_tklVersion = 0;
	//ボーンの数
	int m_numBone = 0;
	//SObjectの可変長配列
	std::vector<SObject> m_objects;
public:
	//ボーンの数を取得
	int GetNumBone() { return m_numBone; }
};

