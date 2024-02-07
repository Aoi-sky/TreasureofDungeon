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
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");//�e�̌`�i���b�V���j��ݒ�
		
		m_mesh = MeshResource::CreateCube(1.0f, true); // �{�b�N�X�𐶐�����
		const Vec3 scale(m_Scale / 10);

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(m_mesh);
		ptrDraw->SetTextureResource(L"FLOOR");//�e�N�X�`���𔽉f����
		ptrDraw->SetSamplerState(SamplerState::AnisotropicWrap); 

		auto vertices = m_mesh->GetBackupVerteces<VertexPositionNormalTexture>();
		for (auto& vertex : vertices) { 
			if (vertex.normal.z < 0.0f || vertex.normal.z > 0.0f){ // �@����Z�������̌����Ă�����XY���ʂ̃|���S���i�O��j
				vertex.textureCoordinate.x *= scale.x;
				vertex.textureCoordinate.y *= scale.y;
			}
			if (vertex.normal.x < 0.0f || vertex.normal.x > 0.0f){ // �@����X�����̌����Ă�����ZY���ʂ̃|���S���i���E�j
				vertex.textureCoordinate.x *= scale.z;
				vertex.textureCoordinate.y *= scale.y;
			}
			if (vertex.normal.y < 0.0f || vertex.normal.y > 0.0f){ // �@����Y�����������Ă�����XZ���ʂ̃|���S���i�㉺�j
				vertex.textureCoordinate.x *= scale.x;
				vertex.textureCoordinate.y *= scale.z;
			}
		}
		m_mesh->UpdateVirtexBuffer<VertexPositionNormalTexture>(vertices);

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
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetTextureResource(L"WALL");//�e�N�X�`���𔽉f����

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
	}

	void FixedCylinder::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FallingRocks")) {
			GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			GetStage()->AddGameObject<FallingRocks>();
			return;
		}
	}

}
