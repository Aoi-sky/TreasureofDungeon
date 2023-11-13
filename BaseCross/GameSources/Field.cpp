/*!
@file Field.cpp
@brief �t�B�[���h����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//�{�b�N�X
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox() {}

	//������
	void FixedBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//OBB�Փ˔����t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		AddTag(L"FixedBox");//�^�O������
		auto shadowPtr = AddComponent<Shadowmap>();//�e������i�V���h�E�}�b�v��`�悷��j
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"FIELD");//�e�N�X�`���𔽉f����
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);

	}


	//��
	Wall::Wall(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		FixedBox(StagePtr, Scale, Rotation, Position)
	{
	}
	Wall::~Wall() {}

	//������
	void Wall::OnCreate() {
		// �p������OnCreate�̌Ăяo��
		FixedBox::OnCreate();

		// �^�O�̐ݒ�
		AddTag(L"Wall");
	}


	//��
	Floor::Floor(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		FixedBox(StagePtr, Scale, Rotation, Position)
	{
	}
	Floor::~Floor() {}

	//������
	void Floor::OnCreate() {
		// �p������OnCreate�̌Ăяo��
		FixedBox::OnCreate();

		// �^�O�̐ݒ�
		AddTag(L"Floor");
	}


	//�~��
	FixedCylinder::FixedCylinder(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedCylinder::~FixedCylinder() {}

	//������
	void FixedCylinder::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//OBB�Փ˔����t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetFixed(true);
		AddTag(L"FixedCylinder");//�^�O������
		auto shadowPtr = AddComponent<Shadowmap>();//�e������i�V���h�E�}�b�v��`�悷��j
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMeshResource(L"DEFAULT_CYLINDER");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CYLINDER");
		ptrDraw->SetTextureResource(L"PILLAR");//�e�N�X�`���𔽉f����
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);
	}

	void FixedCylinder::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FallingRocks")) {
			GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			GetStage()->AddGameObject<FallingRocks>();
			return;
		}
	}

}
