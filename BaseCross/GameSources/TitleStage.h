/*!
@file TiteleStage.h
@brief �^�C�g�����
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage {

		//�r���[�̍쐬
		void CreateViewLight();
		//�X�v���C�g�̍쐬
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
