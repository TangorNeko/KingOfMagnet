/*!
 * @brief	ディファードレンダリングのスプライトの描画
 */

cbuffer cb : register(b0){
	float4x4 mvp;		//���[���h�r���[�v���W�F�N�V�����s��B
	float4 mulColor;	//��Z�J���[�B
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

//ライトの定数バッファ
cbuffer LightDataCb : register(b1)
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

//カスケードシャドウのクロップされたビュープロジェクション行列の定数バッファ
cbuffer ShadowParamCb : register(b2)
{
    float4x4 mLVPC[3];
};

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

//G-Buffers
Texture2D<float4> g_albedo : register(t0);				//アルベドテクスチャ
Texture2D<float4> g_normal : register(t1);				//法線テクスチャ
Texture2D<float4> g_worldPos : register(t2);			//ワールド座標テクスチャ
Texture2D<float4> g_shadowMap_Near : register(t3);		//シャドウマップテクスチャ(近距離)
Texture2D<float4> g_shadowMap_Middle : register(t4);	//シャドウマップテクスチャ(中距離)
Texture2D<float4> g_shadowMap_Far : register(t5);		//シャドウマップテクスチャ(遠距離)

sampler g_sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
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

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(PSInput psIn) : SV_Target0
{
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	float4 normal = g_normal.Sample(g_sampler,psIn.uv);
	if(normal.x == 0.0f && normal.y == 0.0f && normal.z == 0.0f)
	{
		discard;
	}
	normal = (normal - 0.5f) * 2.0f;
	float4 worldPos = g_worldPos.Sample(g_sampler,psIn.uv);

	float4 finalColor = 0.0f;
	finalColor.a = 1.0f;


	//ディレクションライト
	for (int i = 0;i < directionLigNum;i++)
	{
		//ランバート拡散反射
		float3 diffuseLig = CalcLambertDiffuse(directionLigData[i].ligDir, directionLigData[i].ligColor,normal);

		//フォン鏡面反射
		float3 specularLig = CalcPhongSpecular(directionLigData[i].ligDir, directionLigData[i].ligColor, worldPos, normal);

		float3 finalLig = diffuseLig + specularLig;

		finalColor.xyz +=  finalLig;
	}

	//ポイントライト
	for (int i = 0; i < pointLigNum; i++)
	{
		float3 pointLigDir = worldPos - pointLigData[i].ligPos;
		pointLigDir = normalize(pointLigDir);

		//ランバート拡散反射
		float3 diffuseLig = CalcLambertDiffuse(pointLigDir, pointLigData[i].ligColor, normal);

		//フォン鏡面反射
		float3 specularLig = CalcPhongSpecular(pointLigDir,pointLigData[i].ligColor,worldPos,normal);

		float3 finalLig = diffuseLig +specularLig;

		//距離による減衰

		float3 distance = length(worldPos - pointLigData[i].ligPos);

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
		float3 spotLigDir = worldPos - spotLigData[i].ligPos;
		spotLigDir = normalize(spotLigDir);

		//ランバート拡散反射
		float3 diffuseLig = CalcLambertDiffuse(spotLigDir, spotLigData[i].ligColor, normal);

		//フォン鏡面反射
		float3 specularLig = CalcPhongSpecular(spotLigDir, spotLigData[i].ligColor, worldPos, normal);

		float3 finalLig = diffuseLig + specularLig;

		//距離による減衰

		float3 distance = length(worldPos - spotLigData[i].ligPos);

		float affect = 1.0f - 1.0f / spotLigData[i].ligRange * distance;

		if (affect < 0)
			affect = 0;

		affect = pow(affect, 3.0f);

		finalLig *= affect;

		//角度による減衰
		float3 toGround = worldPos - spotLigData[i].ligPos;
		toGround = normalize(toGround);

		float angle = dot(toGround, spotLigData[i].ligDir);
		
		//floatの誤差かacos(1)が0に、acos(-1)がπになるはずなのにNanになっていたので臨時変更(錦織)
		if (-1 < angle && angle < 1)
		{
			angle = acos(angle);
		}
		else if(angle > 0.9 )
		{
			angle = 0;
		}
		else
		{
			angle = acos(-1.0f);
		}

		affect = 1.0f - 1.0f / spotLigData[i].ligAngle * angle;
		if (affect <= 0.0f)
		{
			affect = 0.0f;
		}
		else
		{
			//0より大きい時だけ乗算
			affect = pow(affect, 0.5f);
		}

		finalLig *= affect;

		finalColor.xyz += finalLig;
	}

	//環境光
	float3 ambientLig = 0.3f;
	finalColor.xyz += ambientLig;

	finalColor *= albedoColor;

	//影
	Texture2D<float4> shadowMaps[3];
	shadowMaps[0] = g_shadowMap_Near;
	shadowMaps[1] = g_shadowMap_Middle;
	shadowMaps[2] = g_shadowMap_Far;

	for(int shadowMapNo = 0;shadowMapNo < 3;shadowMapNo++)
	{
		float4 posInLVP = mul(mLVPC[shadowMapNo],worldPos);

		float zInLVP = posInLVP.z / posInLVP.w;
		
		if(zInLVP >= 0.0f && zInLVP <= 1.0f)
		{
			float2 shadowMapUV = posInLVP.xy / posInLVP.w;

			shadowMapUV *= float2(0.5f,-0.5f);
    		shadowMapUV += 0.5f;

			 if(shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f && shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f)
    	        {
    	            float shadowValue = shadowMaps[shadowMapNo].Sample(g_sampler,shadowMapUV).x;

    	            if(zInLVP >= shadowValue.r + 0.001f)
    	            {
    	                finalColor.xyz *= 0.3f;
    	            }
					break;
    	        }
		}
	}

	return finalColor;
}
