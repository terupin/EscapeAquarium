

// C++����萔�o�b�t�@�œ]�����ꂽ�f�[�^���Z�b�g�����
cbuffer ConstantBufferMatrix : register(b1)
{
	matrix gWorld;
	matrix gView;
	matrix gProjection;
	matrix gWorldRotate;
	float4 gUvOffset;
	float4 gDiffuse;
	float3 gLightVector;
	float gLightPower;
	float4 gAmbient;
}

