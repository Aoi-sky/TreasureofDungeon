/*!
@file ShotEnemy.cpp
@brief �����΂��Ă���G
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	ShotEnemy::ShotEnemy(const shared_ptr<Stage>& stage):
		GameObject(stage)

	{
	}
	ShotEnemy::~ShotEnemy() {}

	void ShotEnemy::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");

		m_transform = GetComponent<Transform>();//�R���|�[�l���g�̎擾
		m_transform->SetPosition(Vec3(1.0f, 5.0f, 1.0f));//���W�ݒ�
		m_transform->SetScale(Vec3(1.0f, 2.0f, 1.0f));//�T�C�Y����


		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);


		AddTag(L"ShotEnemy");//�^�O�̐ݒ�

	}

	void ShotEnemy::OnUpdate() {

	}
}