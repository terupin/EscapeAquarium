#include "ObjModelLoader.h"
#include "Direct3D.h"
#include <stdio.h>

ModelData ObjModelLoader::Load(const char* pObjFileName, const wchar_t* pTexFileName)
{
	errno_t err = fopen_s(&mFp, pObjFileName, "rt"); // �e�L�X�g�ǂݍ��݃��[�h

	if (err == 0) {
		for (;;) {
			char c = fgetc(mFp); // �t�@�C������P�����ǂݍ���

			switch (c)
			{
			case 'v':
				c = fgetc(mFp);
				switch (c) {
				case ' ': // �X�y�[�X����
					FinVertex();
					break;

				case 't':
					FinUv();
					break;

				case 'n':
					FinNormal();
					break;
				}
				break;

			case 'f':
				FinFace();
				break;

			case '\n':
				break;

			case EOF: // �t�@�C���̍Ō�ɓ��B
				goto END_OF_FILE;

			default:
				FinSkip();
				break;
			}
		}

END_OF_FILE:
		fclose(mFp);
	}
	else {
		throw E_FAIL;
	}

	ModelData modelData;

	modelData.mNumVertex = (int)mVertexList.size();

	// ���_�o�b�t�@�]���p�̒��_�z��𓮓I�m��
	ModelVertex* pVx = new ModelVertex[modelData.mNumVertex];

	// Obj�t�@�C������ǂݍ��񂾃f�[�^��z��ɃR�s�[
	for (int i = 0; i < modelData.mNumVertex; i++) {
		pVx[i] = mVertexList[i];
	}

	// ���f���p�̒��_�o�b�t�@�쐬
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(ModelVertex) * modelData.mNumVertex;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// �o�b�t�@�̏����l�w��
	D3D11_SUBRESOURCE_DATA initialVertexData;
	// ���_�o�b�t�@�̏����l
	initialVertexData.pSysMem = pVx;
	// ���_�o�b�t�@�ł͎g�p���Ȃ�
	initialVertexData.SysMemPitch = 0;
	initialVertexData.SysMemSlicePitch = 0;

	HRESULT hr = Direct3D_Get()->device->CreateBuffer(&bufferDesc, &initialVertexData, &modelData.mVertexBuffer);

	delete pVx;

	if (FAILED(hr))
	{
		throw hr;
	}

	// �e�N�X�`���ǂݍ���
	Direct3D_LoadTexture(pTexFileName, &modelData.mSRV);

	return modelData;
}

void ObjModelLoader::FinSkip()
{
	char buf[256];
	fgets(buf, sizeof(buf) - 1, mFp);
}

void ObjModelLoader::FinVertex()
{
	F3 pos;
	//fscanf_s(mFp, "%f", &pos.x);
	//fscanf_s(mFp, "%f", &pos.y);
	//fscanf_s(mFp, "%f", &pos.z);
	fscanf_s(mFp, "%f %f %f", &pos.x, &pos.y, &pos.z);
	mPositionList.emplace_back(pos);
}

void ObjModelLoader::FinNormal()
{
	F3 normal;
	//fscanf_s(mFp, "%f", &normal.x);
	//fscanf_s(mFp, "%f", &normal.y);
	//fscanf_s(mFp, "%f", &normal.z);
	fscanf_s(mFp, "%f %f %f", &normal.x, &normal.y, &normal.z);
	mNormalList.emplace_back(normal);
}

void ObjModelLoader::FinUv()
{
	F2 uv;
	//fscanf_s(mFp, "%f", &uv.x);
	//fscanf_s(mFp, "%f", &uv.y);
	fscanf_s(mFp, "%f %f", &uv.x, &uv.y);
	mUvList.emplace_back(uv);
}

void ObjModelLoader::FinFace()
{
	ModelVertex vx[3];

	for (int i = 0; i < 3; i++) {
		int p, n, t;
		fscanf_s(mFp, "%d/%d/%d", &p, &t, &n);
		// obj�t�@�C����face���1�n�܂�̂���-1����0�n�܂�ɂ���
		p--;
		n--;
		t--;
		vx[i].x = mPositionList[p].x;
		vx[i].y = mPositionList[p].y;
		vx[i].z = -mPositionList[p].z; // �e�N�X�`�����]�̂���

		vx[i].nx = mNormalList[n].x;
		vx[i].ny = mNormalList[n].y;
		vx[i].nz = mNormalList[n].z;
		
//		vx[i].r = vx[i].g = vx[i].b = vx[i].a = 1.0f;

		vx[i].u = mUvList[t].x;
		vx[i].v = 1.0f-mUvList[t].y; // �e�N�X�`�����]�̂���
	}

	mVertexList.emplace_back(vx[0]);
	mVertexList.emplace_back(vx[2]);
	mVertexList.emplace_back(vx[1]);
}
