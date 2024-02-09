#include "BillboardModel.h"
#include "Direct3D.h"

using namespace DirectX;

extern ID3D11Buffer* gpConstBuffer;
/*
void BillboardModel::Draw()
{
	DIRECT3D* d3d = Direct3D_Get();

	// ��]�s��
	XMMATRIX mxRotY = XMMatrixRotationY(
		XMConvertToRadians(mRotate.y));  // Y����]
	XMMATRIX mxRotX = XMMatrixRotationX(
		XMConvertToRadians(mRotate.x));
	XMMATRIX mxRotZ = XMMatrixRotationZ(
		XMConvertToRadians(mRotate.z));

	// ���s�ړ��s��
	XMMATRIX mxTrans = XMMatrixTranslation(
		mPos.x, mPos.y, mPos.z);  // ���̂̌��݈ʒu��\���ϐ����g���āA���s�ړ��̍s������

	// �g��k���s��
	XMMATRIX mxScale = XMMatrixScaling(
		mScale.x, mScale.y, mScale.z);

	// �r���{�[�h����
	// �r���[�ϊ��s����擾
	XMMATRIX mxView = Camera::mMainCamera->GetViewMatrix();
	// �t�s������
	XMMATRIX mxInverse =
		XMMatrixInverse(nullptr, mxView);
	// �t�s��̈ړ�������菜��
	mxInverse.r[3].m128_f32[0] = 0.0f;// �S�s�P��
	mxInverse.r[3].m128_f32[1] = 0.0f;// �S�s�Q��
	mxInverse.r[3].m128_f32[2] = 0.0f;// �S�s�R��

	// ���[���h�ϊ��s����쐬
	XMMATRIX mxWorld = mxScale * mxInverse * mxTrans;

	// �v���W�F�N�V�����ϊ��s����쐬
	// 1: ����p
	// 2: �A�X�y�N�g��@���@��{�A�E�C���h�E�T�C�Y�̏c����������΂悢
	// 3: near ������͈͂̈�Ԏ�O�̎��_����̋���
	// 4: far  ������͈͂̈�ԉ��̎��_����̋���
	XMMATRIX mxProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);

	// �`��Ŏg�p����e�N�X�`�����w�肷��֐��Ăяo��
	// �����Ŏw�肳�ꂽ�e�N�X�`�����A�s�N�Z���V�F�[�_�[�̃O���[�o���ϐ��ɃZ�b�g�����
	d3d->context->PSSetShaderResources(0, 1, &mModelData.mSRV);

	// �`��Ɏg�����_�o�b�t�@���w�肷��
	UINT strides = sizeof(VERTEX3D);
	UINT offsets = 0;
	// ��O�����F���_�o�b�t�@
	d3d->context->IASetVertexBuffers(0, 1,
		&mModelData.mVertexBuffer, &strides, &offsets);

	// �萔�o�b�t�@�X�V�@���@�b���ꂩ��V�F�[�_�[�Ƀf�[�^��]������
	D3D11_MAPPED_SUBRESOURCE pData;  // �萔�o�b�t�@�Ŋm�ۂ��ꂽ�������̈�̏�񂪊i�[�����\����
	// �萔�o�b�t�@�̃������̈�ɃA�N�Z�X���āA�f�[�^��]�����鏈��
	// �萔�o�b�t�@�̃������ɃA�N�Z�X���邽�߂ɁA���̃������̈�Ƀ��b�N��������

	// �J�����I�u�W�F�N�g����r���[�ϊ��s����擾
	XMMATRIX mxView = Camera::mMainCamera->GetViewMatrix();

	ConstBufferData cbdata;
	cbdata.world = XMMatrixTranspose(mxWorld);
	cbdata.view = XMMatrixTranspose(mxView);
	cbdata.projection = XMMatrixTranspose(mxProjection);

	HRESULT hr = d3d->context->Map(gpConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		

		// 1: �R�s�[��������̃A�h���X
		// 2: �R�s�[��̊m�ۂ���Ă���f�[�^�T�C�Y
		// 3: �R�s�[�������f�[�^�������Ă���擪�A�h���X
		// 4: �R�s�[�������f�[�^�̃T�C�Y
		memcpy_s(pData.pData, pData.RowPitch, &cbdata, sizeof(cbdata));

		d3d->context->Unmap(gpConstBuffer, 0);  // ���b�N��������������������
	}

	// �萔�o�b�t�@���V�F�[�_�[����A�N�Z�X�ł���悤�ɃZ�b�g����
	d3d->context->VSSetConstantBuffers(1, 1, &gpConstBuffer);

	// ����܂ł̐ݒ�Ŏ��ۂɕ`�悷��  �������F���ۂɕ`�悷�钸�_��
	d3d->context->Draw(mModelData.mNumVertex, 0);
}
*/
void BillboardModel::GetWVPRMatrix(ConstBufferData & out)
{
	// �r���{�[�h����
	// �r���[�ϊ��s����擾
	XMMATRIX mxView = Camera::mMainCamera->GetViewMatrix();
	// �t�s������
	XMMATRIX mxInverse =
		XMMatrixInverse(nullptr, mxView);
	// �t�s��̈ړ�������菜��
	mxInverse.r[3].m128_f32[0] = 0.0f;// �S�s�P��
	mxInverse.r[3].m128_f32[1] = 0.0f;// �S�s�Q��
	mxInverse.r[3].m128_f32[2] = 0.0f;// �S�s�R��

	// ���s�ړ��s��
	XMMATRIX mxTrans = XMMatrixTranslation(
		mPos.x, mPos.y, mPos.z);  // ���̂̌��݈ʒu��\���ϐ����g���āA���s�ړ��̍s������

	// �g��k���s��
	XMMATRIX mxScale = XMMatrixScaling(
		mScale.x, mScale.y, mScale.z);

	// ���[���h�ϊ��s����쐬
	XMMATRIX mxRotate = mxInverse;
	XMMATRIX mxWorld = mxScale * mxRotate * mxTrans;

	// �v���W�F�N�V�����ϊ��s����쐬
	// 1: ����p
	// 2: �A�X�y�N�g��@���@��{�A�E�C���h�E�T�C�Y�̏c����������΂悢
	// 3: near ������͈͂̈�Ԏ�O�̎��_����̋���
	// 4: far  ������͈͂̈�ԉ��̎��_����̋���
	XMMATRIX mxProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);

	// �萔�o�b�t�@�̓]���p�\���̂ɃR�s�[�@�����̍ہA�s��͓]�u�s��ɂ���
	out.world = XMMatrixTranspose(mxWorld);
	out.view = XMMatrixTranspose(mxView);
	out.projection = XMMatrixTranspose(mxProjection);
	out.worldRotate = XMMatrixTranspose(mxRotate);
}
