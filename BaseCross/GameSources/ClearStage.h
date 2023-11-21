/*!
@file ClearStage.h
@brief ƒNƒŠƒA‰æ–Ê
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
