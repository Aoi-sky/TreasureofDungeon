/*!
@file Gimmick.h
@brief 落石などのギミック
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class FallingRocks : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;

	public:
		//コンストラクタ
		FallingRocks(const shared_ptr<Stage>& StagePtr);
		virtual ~FallingRocks();//デストラクタ

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
}
//end basecross