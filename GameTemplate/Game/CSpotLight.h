#pragma once

namespace prefab
{
	struct SpotLigData //スポットライトのデータ
	{
		Vector3 ligPos;		//ライトの座標
		float pad;			//パディング
		Vector3 ligColor;	//ライトの色
		float ligRange;		//ライトの影響範囲
		Vector3 ligDir;		//ライトの向き
		float ligAngle;		//ライトの影響角度(ラジアン単位)
	};

	class CSpotLight : public IGameObject
	{
	private:
		SpotLigData spotLigData;	//スポットライトのデータ

		int m_spotLigTag;			//スポットライトのタグ(何番目に作られたライト?)
		//TODO:他のスポットライトを消した時に何番目かが更新されなければいけない
	public:
		~CSpotLight();
		bool Start();
		void Update();

		//スポットライトのデータを取得
		SpotLigData* GetLigData() { return &spotLigData; }

		//スポットライトのサイズを取得
		int GetLigDataSize() { return sizeof(SpotLigData); }

		//座標の設定と取得
		void SetPosition(Vector3 pos) { spotLigData.ligPos = pos; }
		Vector3 GetPosition() { return spotLigData.ligPos; }

		//色の設定と取得
		void SetColor(Vector3 color) { spotLigData.ligColor = color; }
		Vector3 GetColor() { return spotLigData.ligColor; }

		//影響範囲の設定と取得
		void SetRange(float range) { spotLigData.ligRange = range; }
		float GetRange() { return spotLigData.ligRange; }

		//向きの設定と取得
		void SetDirection(Vector3 dir) { spotLigData.ligDir = dir; }
		Vector3 GetDirection() { return spotLigData.ligDir; }

		//影響角度の設定と取得(ラジアン単位)
		void SetAngle(float angle) { spotLigData.ligAngle = angle; }
		float GetAngle() { return spotLigData.ligAngle; }

		//影響角度の設定と取得(デグリー単位)
		void SetAngleDeg(float angle) { spotLigData.ligAngle = Math::DegToRad(angle); }
		float GetAngleDeg() { return Math::RadToDeg(spotLigData.ligAngle); }
	};
}

