#pragma once
class tklFile
{
public:

	//tklファイルに含まれるボーン情報
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

	/**
	 * @brief SObjectに対して共通の処理を行う
	 * @param query 共通処理の関数
	*/
	void QueryObject(std::function<void(SObject& obj)>query)
	{
		for (auto& obj : m_objects)
		{
			query(obj);
		}
	}

	/**
	 * @brief .tklファイルを読み込んで内容をSObjectの可変長配列m_objectsに書き込む
	 * @param levelFilePath tklファイルのパス
	 * @return 成功した?
	*/
	bool Load(const char* levelFilePath);

	/**
	 * @brief ボーンの数を取得
	 * @return ボーンの数
	*/
	int GetNumBone() { return m_numBone; }

	/**
	 * @brief SObjectの可変長配列のデータを取得
	 * @param index 何番目のデータか
	 * @return SObjectデータ
	*/
	SObject& GetObjectAt(int index)
	{
		return m_objects[index];
	}
private:
	int m_tklVersion = 0;						//tklファイルのバージョン
	int m_numBone = 0;							//ボーンの数
	std::vector<SObject> m_objects;				//SObjectの可変長配列
	typedef std::unique_ptr<SObject> CObject;	//SObjectのユニークポインタ
};

