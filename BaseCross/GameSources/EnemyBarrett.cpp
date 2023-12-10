#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void EnemyBullet::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");//�e�̌`

		//�������x�N�g���ŏ���
		auto ownerTrans = m_owner->GetComponent<Transform>();
		m_transform = GetComponent<Transform>();//�R���|�[�l���g�̎擾
		m_transform->SetPosition(ownerTrans->GetPosition());//���W�ݒ�
		m_transform->SetScale(Vec3(0.5f, 0.5f, 0.5f));//�T�C�Y����
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W����������
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);
		AddTag(L"EnemyBullet");//�^�O�̐ݒ�

		MoveEnemyBullet();
	}

	void EnemyBullet::OnUpdate() {
		float delta = App::GetApp()->GetElapsedTime();//�f���^�^�C�����擾

		//�ړ�����
		auto pos = m_transform->GetPosition();
		pos += m_forward * m_speed * delta;
		m_transform->SetPosition(pos.x,1.0f,pos.z);
	}
	void EnemyBullet::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedCylinder"))//��
		{
			GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
			return;
		}
		if (Other->FindTag(L"Wall")|| Other->FindTag(L"Floor"))//�ǁE��
		{
			GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
			return;
		}
		if (Other->FindTag(L"Enemy"))//�G
		{
			GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
			return;
		}
		if (Other->FindTag(L"FallingRocks"))//����
		{
			GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
			return;
		}
		if (Other->FindTag(L"Player"))//�v���C���[
		{
			GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
			GetStage()->GetSharedGameObject<Player>(L"Player")->AddPlayerDamage(m_damege);
			return;
		}
	}

	void EnemyBullet::MoveEnemyBullet()
	{
		// �f���^�^�C���̎擾
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();

		// �x�N�g���̎擾
		auto playerTrans = GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>();
		auto playerPos = playerTrans->GetPosition(); // �v���C���[�̈ʒu���擾
		auto pos = m_transform->GetPosition();
		
		m_forward = playerPos - pos; // �v���C���[�Ƃ̋������v�Z

		// �x�N�g���̐��K������
		float normalizeMagnification = 1 / sqrt(
			m_forward.x * m_forward.x +
			m_forward.y * m_forward.y +
			m_forward.z * m_forward.z);
		m_forward *= normalizeMagnification;

		float rotationY = atan2f(-m_forward.z, m_forward.x); // ��]�̌v�Z
		m_transform->SetRotation(Vec3(0, rotationY, 0)); // ��]����
	}
}