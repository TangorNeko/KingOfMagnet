#pragma once

/// <summary>
/// トランジションを実行
/// ・シングルトン
/// </summary>
class TransitionGenerator : public IGameObject
{
	static TransitionGenerator* m_instance;
public:
	//トランジションを呼び出すときのお名前
	enum TransitionName {
		UnderSlide,	//下からフェード
		NanameBox,	//ナナメボックス
		Mesh,		//メッシュ
		Circle,		//円
		TransitionName_Nom	//データ数
	};

	TransitionGenerator();
	~TransitionGenerator();
	void Update()override;

	/// <summary>
	/// インスタンスを取得シリーズ
	/// </summary>
	/// <returns>インスタンスです</returns>
	static TransitionGenerator* GetInstance() {
		return m_instance;
	}

	//トランジションの実行
	void TransitionInit(
		const TransitionName& name,
		const int& MoveTime,
		const bool& mode
	);

private:
	//Sprite m_transitionSprite;
	prefab::CSpriteRender* m_transitionSpriteRender = nullptr;

	struct Transition_Data {		//スプライトパターンデータ構造体
		const char* SpriteName;		//スプライトファイル名
	};

	//トランジションの素材スプライト一覧
	Transition_Data Transition_DataList[TransitionName_Nom]{
		{ "Assets/Image/Transition/under.dds" },
		{ "Assets/Image/Transition/naname_box.dds" },
		{ "Assets/Image/Transition/mesh.dds" },
		{ "Assets/Image/Transition/circle_b.dds" },
	};

	//トランジション用
	int m_transitionLimit = 0;			//トランジション実行時間
	float m_borderLine = 0.0f;			//描画されるボーダーライン
	bool m_transitionMode = false;		//falseなら表示、trueなら消去
	bool m_nowTransitionFlag = false;	//トランジション中
	const float MoveSpeed = 10.0f;		//計算用
	RenderContext* m_rc = nullptr;		//描画先
	bool m_initFlag = false;			//初回設定
};

