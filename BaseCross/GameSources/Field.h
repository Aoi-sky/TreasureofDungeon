/*!
@file Field.h
@brief �Q�[���t�B�[���h
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//�L���[�u�̐���
	class FixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;

		shared_ptr<MeshResource> m_mesh; // OnCreate�Ő����������b�V����ۑ����邽�߂ɐ錾

	public:
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override;
	};

	//�ǂ̐���
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
		//������
		virtual void OnCreate() override;
	};


	//�ǂ̐���
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
		//������
		virtual void OnCreate() override;
	};


	//�~���̐���
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
		//������
		virtual void OnCreate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};


}
//end basecross