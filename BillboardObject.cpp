#include "BillboardObject.h"
#include "BillboardModel.h"

BillboardObject::BillboardObject()
{
	// �r���{�[�h�@�\����̃��f��
	mModel = new BillboardModel();
	mHit = new HitSphere(mModel->mPos, 1.0f);
}
