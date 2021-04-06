/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[�B
 */

cbuffer cb : register(b0){
	float4x4 mvp;		//���[���h�r���[�v���W�F�N�V�����s��B
	float4 mulColor;	//��Z�J���[�B
};
struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> sceneTexture : register(t0);	//�J���[�e�N�X�`���B
sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMain(PSInput In) : SV_Target0
{
    float4 color = 0.0f;// = -sceneTexture.Sample(Sampler,In.uv);
    color.a = 1.0f;

    // step-1 基準テクセル＋近傍8テクセルの平均を計算する
    //1.5テクセル分ずらすためのUV値を求める。
    float offsetU = 1.5f / 1280.0f;
    float offsetV = 1.5f / 720.0f;

    //基準テクセルから右のテクセルのカラーをサンプリングする。
    color += 2 * sceneTexture.Sample(Sampler, In.uv + float2(offsetU, 0.0f));
    //基準テクセルから左
    color -= 2 * sceneTexture.Sample(Sampler, In.uv + float2(-offsetU, 0.0f));
    //テクセルから下
    color += sceneTexture.Sample(Sampler, In.uv + float2(0.0f, offsetV));
    //上
    color += sceneTexture.Sample(Sampler, In.uv + float2(0.0f, -offsetV));
    //右下
    color += sceneTexture.Sample(Sampler, In.uv + float2(offsetU, offsetV));
    //右上
    color += sceneTexture.Sample(Sampler, In.uv + float2(offsetU, -offsetV));
    //左下
    color -= sceneTexture.Sample(Sampler, In.uv + float2(-offsetU, offsetV));
    //左上
    color -= sceneTexture.Sample(Sampler, In.uv + float2(-offsetU, -offsetV));

    //基準テクセルと近傍8テクセルの平均なので9で除算する。
    //color /= 9.0f;
    return color;
}

float4 PSSamplingLuminance(PSInput In) : SV_Target0
{
    // step-13 輝度を抽出するピクセルシェーダーを実装
    //メインレンダリングターゲットからカラーをサンプリング
    float4 color = sceneTexture.Sample(Sampler,In.uv);
    //サンプリングしたカラーの明るさを計算
    float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    //clip関数は引数の値がマイナスになると、以降の処理をスキップする。
    //なので、マイナスになるとピクセルカラーは出力されない。
    //今回の実装はカラーの明るさが1以下ならピクセルキルする。
    clip(t - 1.0f);
    return color;
}