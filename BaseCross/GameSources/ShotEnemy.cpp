/*!
@file ShotEnemy.cpp
@brief �e���΂��Ă���G
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	ShotEnemy::ShotEnemy(const shared_ptr<Stage>& stage, const Vec3& Position):
		GameObject(stage),
		m_position(Position),
		m_rot(0),
		m_speed(1.0f),
		m_RecastCount(120),
		m_recastFlame(0)
	{
	}
	

	ShotEnemy::~ShotEnemy() {}

	void ShotEnemy::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		//drawComp->SetMeshResource(L"DEFAULT_SPHERE");
		//�`�揈��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();

		drawComp->SetMeshResource(L"ShootEnemy");

		m_transform = GetComponent<Transform>();//�R���|�[�l���g�̎擾
		m_transform->SetPosition(m_position);//���W�ݒ�
		m_transform->SetScale(Vec3(0.8f, 0.8f, 0.8f));//�T�C�Y����

		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);
		AddTag(L"ShotEnemy");//�^�O�̐ݒ�
	}

	void ShotEnemy::OnUpdate() {

		auto delta = App::GetApp()->GetElapsedTime();
		m_rot.y = m_rot.y + delta * m_speed;
		m_transform->SetRotation(Vec3(0.0f,m_rot.y, 0.0f ));

		m_recastFlame--;

		if (m_recastFlame <= 0)
		{
			// �S�[���������S���Ă���ꍇ�A�e����������
			if (GetStage()->GetSharedGameObject<Golem>(L"Golem")->GetGolemCurrentLife() > 0) {
				GetStage()->AddGameObject<EnemyBullet>(GetThis<ShotEnemy>());
				m_recastFlame = m_RecastCount;
			}
		}
	}

	void ShotEnemy::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Wave"))//��
		{
			GetStage()->RemoveGameObject<ShotEnemy>(GetThis<ShotEnemy>());
			return;
		}
	}
}