// �s�N�Z���V�F�[�_�[�̃��C���֐��@���@�|���S����`�悷��̂ɕK�v�ȃs�N�Z���������Ăяo�����
// �e�s�N�Z���̐F���w�肷��̂����
// �߂�l�F�@���̃s�N�Z�������F�ɂ��������ARGBA�Ŏw�肷��
// ����inputPos�F�@�s�N�Z���̍��W�B�E�C���h�E�̍�������_�Ƃ���s�N�Z�����W�B

// �e�N�X�`�����󂯎��O���[�o���ϐ�
Texture2D gTexture : register(t0);

// �T���v���[�̃O���[�o���ϐ�
SamplerState gSampler : register(s0);

// �萔�o�b�t�@
#include "cb.hlsl"

float4 ps_main(float4 inputPos : SV_POSITION,
	float2 uv : TEXCOORD, float4 col : COLOR, float3 normal : NORMAL) : SV_Target
{
	// �e�N�X�`���̃e�N�Z���F���o�̓s�N�Z���F�ɂ���
	float4 pixelColor = gTexture.Sample(gSampler, uv);

	// �f�B�t���[�Y�}�e���A���̐F��������
	pixelColor *= gDiffuse;

	// ���C�g�̋������O�����C�g�����s�v
	if (gLightPower <= 0.0f)
	{
		// ���S���߃s�N�Z���Ȃ�o�͂��Ȃ���Z�o�b�t�@���X�V����Ȃ�
		if (pixelColor.a == 0.0f)
			discard;

		return pixelColor;
	}

	float4 finalColor = pixelColor;

	return finalColor;
}
