#pragma once

#include <d3d11.h>


// 3D���f����\���\����
struct ModelData
{
	ID3D11Buffer* mVertexBuffer;	// ���_�o�b�t�@�̃|�C���^
	ID3D11ShaderResourceView* mSRV;	// �e�N�X�`����SRV�@�iSRV��ID�݂����Ȃ��́j
	int mNumVertex;					// ���_��
};

// ���f�����\�����钸�_�P��\���\����
struct ModelVertex
{
	float x, y, z;		// �ʒu
	
//	float r, g, b, a;	// ���_�F
	float u, v, uv1;			// �e�N�X�`��UV���W
	float nx, ny, nz;	// �@�����
	
};

// �F��\���\����
struct RGBA
{
	float r, g, b, a;
};


struct VERTEX3D  // VERTEX = ���_
{
	float x, y, z;  // ���_�̍��W�i�ʒu�j
	RGBA color;  // ���_�̐F
	float u, v;  // �e�N�X�`����UV���W
};

