

// C++から定数バッファで転送されたデータがセットされる
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

