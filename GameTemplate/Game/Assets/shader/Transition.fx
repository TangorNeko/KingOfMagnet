
//�g�����W�V�����p�̃V�F�[�_�[

cbuffer cb:register(b0) {
	float4x4 mvp;	//���[���h�r���[�v���W�F�N�V�����s��
	float Alpha;	//�A���t�@�l
};

struct VSInput {
	float4 pos : SV_Position;
	float2 uv : TEXCOORD0;
};
struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);	//�J���[�e�N�X�`��
sampler Sampler : register(s0);

PSInput VSMain(VSInput In) {
	PSInput psIn;
	psIn.pos = mul(mvp, In.pos);
	psIn.uv = In.uv;

	return psIn;
}

float4 PSMain(PSInput In) :SV_Target0{
	//�g�����W�V�����p
	float4 color = colorTexture.Sample(Sampler, In.uv);
	float4 finalColor = { 0.0f,0.0f,0.0f,1.0f };
	color.r += 0.2f;

	//����
	finalColor.a = color.r * Alpha;

	if (finalColor.a > 1.0f) {
		finalColor.a = 1.0f;
	}

	return finalColor;
}

