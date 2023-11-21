/*!
@file Field.h
@brief ゲームフィールド
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//キューブの生成
	class FixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;

		shared_ptr<MeshResource> m_mesh; // OnCreateで生成したメッシュを保存するために宣言

	public:
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//初期化
		virtual void OnCreate() override;
	};

	//壁の生成
	class Wall : public FixedBox {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		Wall(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~Wall();
		//初期化
		virtual void OnCreate() override;
	};


	//壁の生成
	class Floor : public FixedBox {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		Floor(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~Floor();
		//初期化
		virtual void OnCreate() override;
	};


	//円柱の生成
	class FixedCylinder : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		FixedCylinder(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedCylinder();
		//初期化
		virtual void OnCreate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};


}
//end basecross