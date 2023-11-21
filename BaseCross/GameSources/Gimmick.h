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
		Vec3 m_StartPos;
		bool m_fallenFlg;


		shared_ptr<EfkEffect> m_damageEffect;
		shared_ptr<EfkPlay> m_EfkPlay; // エフェクト実行オブジェクト
		wstring m_damageEffectStr;

		shared_ptr<Transform> m_transform; // トランスフォームコンポーネント

		// トランスフォームとモデルの差分行列
		Mat4x4 m_differenceMatrix;
	public:
		//コンストラクタ
		FallingRocks(const shared_ptr<Stage>& StagePtr);

		virtual ~FallingRocks();//デストラクタ



		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& other) override;
	};
}
//end basecross