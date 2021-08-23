/*!
 * @brief	リングゲージ用のシェーダー
 */

cbuffer cb : register(b0){
	float4x4 mvp;		//���[���h�r���[�v���W�F�N�V�����s��B
	float4 mulColor;	//��Z�J���[�B
};

cbuffer GaugeDataCB : register(b1)
{
    float3 spriteStartDir;
    float gaugeProgress;
    float2 rotateOrigin;
    int isClockwise;
    int colorFlag;
};

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> redTexture : register(t0);	//赤色のゲージテクスチャ
Texture2D<float4> blueTexture : register(t1);   //青色のゲージテクスチャ
sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}


float4 PSMain( PSInput In ) : SV_Target0
{
	float4 color;
    
	//フラグによって持ってくるテクスチャを分岐
    if(colorFlag == -1)
    {
        color = redTexture.Sample(Sampler, In.uv);
    }
    else
    {
        color = blueTexture.Sample(Sampler, In.uv);
    }


	//回転の中心から調べるピクセルへのベクトル
	float2 toPixelfromOrigin = In.pos.xy - rotateOrigin;
	//正規化する
    toPixelfromOrigin = normalize(toPixelfromOrigin);

	//Z軸
    float3 axisz = {0.0f,0.0f,1.0f};

	//Z軸との外積を計算することによってスタート位置の向きから垂直なベクトルが得られる
    float3 Cross = cross(spriteStartDir,axisz);

	//垂直なベクトルと回転の中心からピクセルへのベクトルの角度を調べる
    float crossdot = dot(Cross,toPixelfromOrigin);

    float2 dir = spriteStartDir.xy;

    float2 revdir = -spriteStartDir.xy;

	//開始方向の角度と回転の中心から調べるピクセルへのベクトルの角度を調べる
    float deg = dot(toPixelfromOrigin,dir);

    deg = degrees(acos(deg));

	//開始方向の反対側の角度と回転の中心から調べるピクセルへのベクトルの角度を調べる
    float deg2 = dot(toPixelfromOrigin,revdir);
    deg2 = degrees(acos(deg2));
    
//反時計周り
if(isClockwise == false)
{
	//最初の180度
    if(gaugeProgress < 180)
    {
        if(crossdot < 0)
        {
            return color;
        }
        else if (deg < gaugeProgress)
        {
            discard;
        }
    }
    else//後半の180度
    {
        if(crossdot > 0)
        {
            discard;
        }
        else if(deg2 < (gaugeProgress - 180))
        {
            discard;
        }
    }
}
else//時計回り
{
	//最初の180度
    if(gaugeProgress < 180)
    {
        if(crossdot > 0)
        {
            return color;
        }
        else if (deg < gaugeProgress)
        {
            discard;
        }
    }
    else//後半の180度
    {
        if(crossdot < 0)
        {
            discard;
        }
        else if(deg2 < (gaugeProgress - 180))
        {
            discard;
        }
    }
}
    
    return color;
}