#pragma once
#include "BillboardObject.h"

// �����G�t�F�N�g
class Explosion :
	public BillboardObject
{

public:
	void Update() override;

	bool IsFinished()
	{
		return mFinished;
	}

private:
	int mTime;

	bool mFinished = false;

};

