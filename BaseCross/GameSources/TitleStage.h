/*!
@file TiteleStage.h
@brief タイトル画面
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage {

		bool stage = false;

		void CreateViewLight();//ビューの作成
		void CreateTitleSprite();//スプライトの作成

	public:
		TitleStage():
			Stage()
		{
		}
		virtual ~TitleStage() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};

}
//end basecross
