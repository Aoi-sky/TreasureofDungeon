/*!
@file ClearStage.h
@brief �N���A���
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class ClearStage :public Stage {

		void CreateViewLight();

	public:
		ClearStage():
			Stage()
		{
		}
		virtual ~ClearStage() {}


		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}
//end basecross
