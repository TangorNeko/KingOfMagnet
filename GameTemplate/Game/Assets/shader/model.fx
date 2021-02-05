/*!
 * @brief	シンプルなモデルシェーダー。
 */


 ////////////////////////////////////////////////
 // 定数バッファ。
 ////////////////////////////////////////////////
 //モデル用の定数バッファ
cbuffer ModelCb : register(b0) {
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//ライト用の構造体
struct DirectionLigData
{
	float3 ligDir;
	float3 ligColor;
};

struct PointLigData
{
	float3 ligPos;
	float3 ligColor;
	float ligRange;
};

struct SpotLigData
{
	float3 ligPos;
	float3 ligColor;
	float ligRange;
	float3 ligDir;
	float ligAngle;
};

cbuffer DirectionLigCb : register(b1)
{
	DirectionLigData directionLigData[5];
	PointLigData pointLigData[10];
	SpotLigData spotLigData[10];
	float3 eyePos;
	int directionLigNum;
	int pointLigNum;
	int spotLigNum;
};

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn {
	int4  Indices  	: BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn {
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//法線
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn {
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos		: TEXCOORD1;
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;
	float w = 0.0f;
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
		w += skinVert.Weights[i];
	}

	skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);

	return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if (hasSkin) {
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	else {
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
	
	psIn.worldPos = psIn.pos;

	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.normal = mul(m, vsIn.normal);

	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

	float4 finalColor = 0.0f;
	finalColor.a = 1.0f;


	//ディレクションライト
	for (int i = 0;i < directionLigNum;i++)
	{
		//ランバート拡散反射
		float t = dot(psIn.normal, -directionLigData[i].ligDir);

		if (t < 0)
			t = 0;

		float3 diffuseLig = directionLigData[i].ligColor * t;

		//フォン鏡面反射
		float3 toEye = eyePos - psIn.worldPos;
		toEye = normalize(toEye);

		float3 refVec = reflect(directionLigData[i].ligDir, psIn.normal);

		t = dot(toEye, refVec);
		if (t < 0)
			t = 0;

		t = pow(t, 5.0f);

		float3 specularLig = directionLigData[i].ligColor * t;

		float3 finalLig = diffuseLig + specularLig;

		finalColor.xyz +=  finalLig;
	}

	//ポイントライト
	for (int i = 0; i < pointLigNum; i++)
	{
		float3 pointLigDir = psIn.worldPos - pointLigData[i].ligPos;
		pointLigDir = normalize(pointLigDir);

		//ランバート拡散反射
		float t = dot(psIn.normal, -pointLigDir);
		if (t < 0)
			t = 0;

		float3 diffuseLig = pointLigData[i].ligColor * t;

		//フォン鏡面反射
		float3 toEye = eyePos - psIn.worldPos;
		toEye = normalize(toEye);

		float3 refVec = reflect(pointLigDir, psIn.normal);

		t = dot(toEye, refVec);
		if (t < 0)
			t = 0;

		t = pow(t, 5.0f);

		float3 specularLig = pointLigData[i].ligColor * t;

		float3 finalLig = diffuseLig + specularLig;

		//距離による減衰

		float3 distance = length(psIn.worldPos - pointLigData[i].ligPos);

		float affect = 1.0f - 1.0f / pointLigData[i].ligRange * distance;

		if (affect < 0)
			affect = 0;

		affect = pow(affect, 3.0f);

		finalLig *= affect;

		finalColor.xyz += finalLig;
	}

	//スポットライト
	for (int i = 0; i < spotLigNum; i++)
	{
		float3 spotLigDir = psIn.worldPos - spotLigData[i].ligPos;
		spotLigDir = normalize(spotLigDir);

		//ランバート拡散反射
		float t = dot(psIn.normal, -spotLigDir);
		if (t < 0)
			t = 0;

		float3 diffuseLig = spotLigData[i].ligColor * t;

		//フォン鏡面反射
		float3 toEye = eyePos - psIn.worldPos;
		toEye = normalize(toEye);

		float3 refVec = reflect(spotLigDir, psIn.normal);

		t = dot(toEye, refVec);
		if (t < 0)
			t = 0;

		t = pow(t, 5.0f);

		float3 specularLig = spotLigData[i].ligColor * t;

		float3 finalLig = diffuseLig + specularLig;

		//距離による減衰

		float3 distance = length(psIn.worldPos - spotLigData[i].ligPos);

		float affect = 1.0f - 1.0f / spotLigData[i].ligRange * distance;

		if (affect < 0)
			affect = 0;

		affect = pow(affect, 3.0f);

		finalLig *= affect;

		//角度による減衰
		float3 toGround = psIn.worldPos - spotLigData[i].ligPos;
		toGround = normalize(toGround);

		float angle = dot(toGround, spotLigData[i].ligDir);
		
		angle = acos(angle);

		affect = 1.0f - 1.0f / spotLigData[i].ligAngle * angle;
		if (affect < 0.0f)
			affect = 0.0f;

		affect = pow(affect, 0.5f);

		finalLig *= affect;

		finalColor.xyz += finalLig;
	}

	float3 ambientLig = 0.3f;
	finalColor.xyz += ambientLig;

	finalColor *= albedoColor;

	return finalColor;
}
