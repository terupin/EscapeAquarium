#include "HitSphere.h"

bool HitSphere::IsHit(HitSphere * pOther)
{
	// active��false�Ȃ瓖���蔻���f�ʂ肳����
	if (active == false || pOther->active == false)
	{
		return false;
	}

	float x = pOther->mCenter.x - mCenter.x;
	float y = pOther->mCenter.y - mCenter.y;
	float z = pOther->mCenter.z - mCenter.z;
	float ctc = x * x + y * y + z * z; // center to center
	float h1_h2 = pOther->mHankei + mHankei;
	return ctc < h1_h2 * h1_h2;
}

HitSphere::HitSphere(DirectX::XMFLOAT3 & center, float hankei)
	: mCenter(center)
{
	mHankei = hankei;
}
