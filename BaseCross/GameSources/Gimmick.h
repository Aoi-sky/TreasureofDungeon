/*!
@file Gimmick.h
@brief ���΂Ȃǂ̃M�~�b�N
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class FallingRocks : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;

	public:
		//�R���X�g���N�^
		FallingRocks(const shared_ptr<Stage>& StagePtr);
		virtual ~FallingRocks();//�f�X�g���N�^

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
}
//end basecross