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

struct DirectionLigData
{
	float3 ligDir;
	float3 ligColor;
	float3 eyePos;
};

cbuffer DirectionLigCb : register(b1)
{
	DirectionLigData directionLigData[10];
	int directionLigNum;
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

	/*

	//ランバート拡散反射
	float t = dot(psIn.normal, -ligDir);

	if (t < 0)
		t = 0;

	float3 diffuseLig = ligColor * t;

	//フォン鏡面反射
	float3 toEye = eyePos - psIn.worldPos;
	toEye = normalize(toEye);

	float3 refVec = reflect(ligDir, psIn.normal);

	t = dot(toEye, refVec);
	if (t < 0)
		t = 0;

	t = pow(t, 5.0f);

	float3 specularLig = ligColor * t;

	float3 ambientLig = 0.3f;

	float3 finalLig = diffuseLig + ambientLig + specularLig;
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * finalLig;
	*/

	float4 finalColor = 0.0f;
	finalColor.a = 1.0f;

	//ランバート拡散反射
	for (int i = 0;i < 5;i++)
	{
		float t = dot(psIn.normal, -directionLigData[i].ligDir);

		if (t < 0)
			t = 0;

		float3 diffuseLig = directionLigData[i].ligColor * t;

		//フォン鏡面反射
		float3 toEye = directionLigData[i].eyePos - psIn.worldPos;
		toEye = normalize(toEye);

		float3 refVec = reflect(directionLigData[i].ligDir, psIn.normal);

		t = dot(toEye, refVec);
		if (t < 0)
			t = 0;

		t = pow(t, 5.0f);

		float3 specularLig = directionLigData[i].ligColor * t;

		//float3 ambientLig = 0.3f;

		float3 finalLig = diffuseLig + specularLig;// + ambientLig;

		finalColor.xyz +=  finalLig;
	}

	float3 ambientLig = 0.3f;
	finalColor.xyz += ambientLig;

	finalColor *= albedoColor;

	return finalColor;
}
