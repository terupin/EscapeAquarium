#pragma once

#include "Model.h"

// �r���{�[�h�@�\����̃��f��
class BillboardModel :
	public Model
{

public:
	BillboardModel()
	{
		mLightActive = false;
	}

	//void Draw() override;
	void GetWVPRMatrix(ConstBufferData& out) override;

};

