/*!
@file TiteleStage.h
@brief タイトル画面
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage {

		//ビューの作成
		void CreateViewLight();
		//スプライトの作成
		void CreateTitleSprite();


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
