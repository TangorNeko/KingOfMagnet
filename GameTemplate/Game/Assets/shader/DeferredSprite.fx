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

Texture2D<float4> albedo : register(t0);	//�J���[�e�N�X�`���
Texture2D<float4> normal : register(t1);	//�J���[�e�N�X�`���
Texture2D<float4> worldPos : register(t2);	//�J���[�e�N�X�`���
Texture2D<float4> shdowColor : register(t3);	//�J���[�e�N�X�`���
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
	return albedo.Sample(Sampler, In.uv) * mulColor;
}