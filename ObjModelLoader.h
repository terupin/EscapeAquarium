#pragma once

#include "ModelData.h"
#include <vector>

/*
	3D���f���f�[�^�uOBJ�t�@�C���v�ǂݍ��݃T���v��

	�����F
	�@OBJ�t�@�C�����̃O���[�v���P�ɂ܂Ƃ܂��Ă��邱��
	�AOBJ�t�@�C�����̖ʏ��iface�j���O�p�`�݂̂ō\������Ă��邱��
	�B�}�e���A�����i.mtl�t�@�C���j�͓ǂݍ��܂Ȃ�
*/

class ObjModelLoader
{

public:
	// OBJ�t�@�C���ƃe�N�X�`����ǂݍ����ModelData�Ƃ��ĕԂ�
	ModelData Load(const char* pObjFileName, const wchar_t* pTexFileName);

private:
	void FinSkip();
	void FinVertex();
	void FinNormal();
	void FinUv();
	void FinFace();

	struct F3 {
		float x, y, z;
	};

	struct F2 {
		float x, y;
	};

	std::vector<F3> mPositionList;
	std::vector<F3> mNormalList;
	std::vector<F2> mUvList;
	std::vector<ModelVertex> mVertexList;

	FILE* mFp;

};

