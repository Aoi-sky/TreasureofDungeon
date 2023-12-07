/*!
@file TiteleStage.h
@brief �^�C�g�����
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage {

		bool stage = false;

		void CreateViewLight();//�r���[�̍쐬
		void CreateTitleSprite();//�X�v���C�g�̍쐬

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
