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

//ライト用の構造体たち
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
	//各配列数はCLightManager.hのMaxLightNumと同じにすること
	DirectionLigData directionLigData[5];
	PointLigData pointLigData[20];
	SpotLigData spotLigData[20];
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
	float3 normalInView : TEXCOORD2;
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1); //法線マップ
Texture2D<float4> g_specularMap : register(t2); //スペキュラマップ　rgbにスペキュラカラー、aに金属度
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

	psIn.normalInView = mul(mView, psIn.normal);

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

//ランバート拡散反射を計算する。
float3 CalcLambertDiffuse(float3 ligDir, float3 ligColor,float3 normal )
{
	float t = dot(normal, -ligDir);
	
	if (t < 0)
	{
		t = 0;
	}

	return ligColor * t;
}

//フレネル反射を考慮した拡散反射を計算する。
float3 CalcDiffuseFromFresnel(float3 normal, float3 ligDir, float3 worldPos)
{
	//光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
	
	//光源に向かうベクトルは光源のベクトルの逆
	float3 toLight = -ligDir;
	float3 toEye = eyePos - worldPos;
	toEye = normalize(toEye);

	float3 halfVec = normalize(toLight + toEye);

	//表面の粗さ
	float roughness = 0.5f;
	float energyBias = lerp(0.0f, 0.5f, roughness);

	//光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める
	float dotLH = saturate(dot(toLight, halfVec));

	float Fd90 = energyBias + 2.0f * dotLH * dotLH * roughness;

	float dotNL = saturate(dot(normal, toLight));

	float FL = Fd90 + (dotNL - Fd90);
	FL = dotNL;

	float dotNV = saturate(dot(normal, toEye));

	float FV = Fd90 + (dotNV - Fd90);
	FV = dotNV;

	return (FL * FV) / 3.141592f;

}

//フォン鏡面反射を計算する。
float3 CalcPhongSpecular(float3 ligDir, float3 ligColor, float3 worldPos, float3 normal)
{
	float3 toEye = eyePos - worldPos;
	toEye = normalize(toEye);

	float3 refVec = reflect(ligDir, normal);

	float t = dot(toEye, refVec);
	
	if (t < 0)
	{
		t = 0;
	}

	t = pow(t, 5.0f);

	return ligColor * t;
}

// ベックマン分布を計算する
float Beckmann(float m, float t)
{
	float t2 = t * t;
	float t4 = t * t * t * t;
	float m2 = m * m;
	float D = 1.0f / (4.0f * m2 * t4);
	D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
	return D;
}

// フレネルを計算。Schlick近似を使用
float SpcFresnel(float f0, float u)
{
	// from Schlick
	return f0 + (1 - f0) * pow(1 - u, 5);
}

//クックトランス鏡面反射を計算する。
float3 CalcCookTrranceSpecular(float3 ligDir, float3 worldPos, float3 normal,float metaric)
{
	float3 toLig = -ligDir;
	float3 toEye = eyePos - worldPos;
	toEye = normalize(toEye);

	float microfacet = 0.76f;

	//金属度を垂直入射の時の反射率として扱う
	//金属度が高くなるほどフレネル反射は大きくなる
	float f0 = metaric;

	//ライトへのベクトルとカメラへのベクトルのハーフベクトルを求める
	float3 halfVec = normalize(toLig+toEye);

	//各種ベクトルがどれぐらい似ているかを内積を利用して求める。
	//法線とハーフベクトル
	float NdotH = saturate(dot(normal, halfVec));
	//カメラへのベクトルとハーフベクトル
	float VdotH = saturate(dot(toEye, halfVec));
	//法線とライトへのベクトル
	float NdotL = saturate(dot(normal, toLig));
	//法線とカメラへのベクトル
	float NdotV = saturate(dot(normal, toEye));

	//D項をベックマン分布を用いて計算する
	float D = Beckmann(microfacet, NdotH);

	//F項をSchlick近似を用いて計算する
	float F = SpcFresnel(f0, VdotH);

	//G項を求める
	float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

	//m項を求める
	float m = 3.141592f * NdotV * NdotH;

	//ここまで求めた値を利用して、クックトランスモデルの鏡面反射を求める
	return max(F * D * G / m, 0.0f);
}

float3 CalcLimLight(float3 ligDir, float3 ligColor, float3 normalInView,float3 normal)
{
	float power1 = 1.0f - max(0.0f, dot(ligDir, normal));

	float power2 = 1.0f - max(0.0f, normalInView.z * -1.0f);

	float limPower = power1 * power2;

	limPower = pow(limPower, 5.0f);

	return ligColor * limPower;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	//アルベドカラー
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	//スペキュラカラー
	float3 specColor = g_specularMap.SampleLevel(g_sampler, psIn.uv, 0).rgb;
	//金属度
	float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;

	float4 finalColor = 0.0f;
	finalColor.a = 1.0f;

	//ディレクションライト
	for (int i = 0;i < directionLigNum;i++)
	{
		//正規化ランバート拡散反射
		float3 diffuseNormalizeLambert = CalcLambertDiffuse(directionLigData[i].ligDir, directionLigData[i].ligColor,psIn.normal) / 3.141592f;

		//フレネル反射を考慮した拡散反射
		float3 diffuseFromFresnel = CalcDiffuseFromFresnel(psIn.normal, directionLigData[i].ligDir, psIn.worldPos);

		//拡散反射全体
		float3 diffuseLig = albedoColor * diffuseFromFresnel * diffuseNormalizeLambert;

		//クックトランス鏡面反射
		float3 specularLig = CalcCookTrranceSpecular(directionLigData[i].ligDir, psIn.worldPos, psIn.normal, metaric);

		//金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
		//スペキュラカラーの強さを鏡面反射率として扱う
		float specTerm = length(specColor.xyz);
		specularLig *= lerp(float3(specTerm, specTerm, specTerm), specColor, metaric);

		//リムライト
		//float3 limLig = CalcLimLight(directionLigData[i].ligDir, directionLigData[i].ligColor, psIn.normalInView, psIn.normal);

		float3 finalLig = diffuseLig * (1.0f - specTerm) + specularLig;

		finalColor.xyz +=  finalLig;
	}

	//ポイントライト
	for (int i = 0; i < pointLigNum; i++)
	{
		float3 pointLigDir = psIn.worldPos - pointLigData[i].ligPos;
		pointLigDir = normalize(pointLigDir);

		//正規化ランバート拡散反射
		float3 diffuseNormalizeLambert = CalcLambertDiffuse(pointLigDir, pointLigData[i].ligColor, psIn.normal) / 3.141592f;

		//フレネル反射を考慮した拡散反射
		float3 diffuseFromFresnel = CalcDiffuseFromFresnel(psIn.normal, pointLigDir, psIn.worldPos);

		//拡散反射全体
		float3 diffuseLig = albedoColor * diffuseFromFresnel * diffuseNormalizeLambert;

		//クックトランス鏡面反射
		float3 specularLig = CalcCookTrranceSpecular(pointLigDir, psIn.worldPos, psIn.normal, metaric);

		//金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
		//スペキュラカラーの強さを鏡面反射率として扱う
		float specTerm = length(specColor.xyz);
		specularLig *= lerp(float3(specTerm, specTerm, specTerm), specColor, metaric);

		//リムライト
		//float3 limLig = CalcLimLight(directionLigData[i].ligDir, directionLigData[i].ligColor, psIn.normalInView, psIn.normal);

		float3 finalLig = diffuseLig * (1.0f - specTerm) + specularLig;

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

		//正規化ランバート拡散反射
		float3 diffuseNormalizeLambert = CalcLambertDiffuse(spotLigDir, spotLigData[i].ligColor, psIn.normal) / 3.141592f;

		//フレネル反射を考慮した拡散反射
		float3 diffuseFromFresnel = CalcDiffuseFromFresnel(psIn.normal, spotLigDir, psIn.worldPos);

		//拡散反射全体
		float3 diffuseLig = albedoColor * diffuseFromFresnel * diffuseNormalizeLambert;

		//クックトランス鏡面反射
		float3 specularLig = CalcCookTrranceSpecular(spotLigDir, psIn.worldPos, psIn.normal, metaric);

		//金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
		//スペキュラカラーの強さを鏡面反射率として扱う
		float specTerm = length(specColor.xyz);
		specularLig *= lerp(float3(specTerm, specTerm, specTerm), specColor, metaric);

		//リムライト
		//float3 limLig = CalcLimLight(directionLigData[i].ligDir, directionLigData[i].ligColor, psIn.normalInView, psIn.normal);

		float3 finalLig = diffuseLig * (1.0f - specTerm) + specularLig;

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

	//環境光
	float3 ambientLig = 0.3f;
	finalColor.xyz += ambientLig * albedoColor;

	//finalColor *= albedoColor;

	return finalColor;
}
