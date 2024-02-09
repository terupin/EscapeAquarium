
// ���_�V�F�[�_�[�̃��C���֐�
// �����F�e���_�̑΂��čs�������ړ��E��]�E�k���Ȃǂ����s����
// �߂�l�F���̃V�F�[�_�[�̏����̌��ʂ��A���_�̃f�[�^�Ƃ��Ė߂�
// ���� inputPos�FVRAM�ɓ]�����ꂽ���_�f�[�^�i�̂����P�j���n�����
//      C����Őݒ肵�����_���W�Ɠ������W�n


// �萔�o�b�t�@
#include "cb.hlsl"

// return�p�̍\����
struct VS_OUTPUT
{
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD;
	float4 col : COLOR;
	float4 nor : NORMAL;
};

// ���_�V�F�[�_�[�@���C���֐��ientrypoint�j
// �߂�l�F���W�ϊ��ς݂̒��_�f�[�^
VS_OUTPUT vs_main(float3 inputPos : POSITION, // xy
	/*float4 col : COLOR,*/ float2 uv : TEXCOORD, float3 normal : NORMAL) // uv
{
	VS_OUTPUT output;

	output.pos.xyz = inputPos.xyz;  // �n���ꂽxy��uv��ϐ��Ɉڂ�
	output.pos.w = 1.0f;
	output.uv = uv + gUvOffset.xy;
	output.col = float4(1,1,1,1);
	output.nor.xyz = normal;
	output.nor.w = 1.0f;

	// ��ʔ䗦�ŉ����ɂȂ��Ă��܂��̂𒲐�����
	//output.pos.x *= 480.0f / 640.0f;

	// ���_���W�ɍs���������
	output.pos = mul(output.pos, gWorld); // ���[���h�ϊ��s��
	output.pos = mul(output.pos, gView);
	output.pos = mul(output.pos, gProjection);

	output.nor = normalize(mul(float4(normal.x, normal.y, normal.z, 1.0f), gWorldRotate));

	return output;
}