#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Wave::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");//�e�̌`
		drawComp->SetDrawActive(true);

		//�������x�N�g���ŏ���
		auto ownerTrans = m_owner->GetComponent<Transform>();
		m_forward = ownerTrans->GetForward();
		m_transform = GetComponent<Transform>();//�R���|�[�l���g�̎擾
		m_transform->SetPosition(ownerTrans->GetPosition() + m_forward * m_speed);//���W�ݒ�
		m_transform->SetScale(Vec3(1.5f,5.0f,1.5f));//�T�C�Y����
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W����������
		//�Փ˔���
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);
		AddTag(L"Wave");//�^�O�̐ݒ�
	}

	void Wave::OnUpdate() {
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();//�f���^�^�C�����擾

		//�ړ�����
		auto pos = m_transform->GetPosition();
		pos += m_forward * m_speed * delta;
		m_transform->SetPosition(pos.x, 1,pos.z);

		GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());

		//m_time += delta * 0.1f;
		//if (m_time >= 1.0f)
		//{
		//	GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
		//	m_time = 0;
		//}

	}
	void Wave::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedCylinder"))//��
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Wall"))//��
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Enemy"))//�G
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Golem"))//�G
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			GetStage()->GetSharedGameObject<Golem>(L"Golem")->AddDamage(70);
			return;
		}
		if (Other->FindTag(L"ShotEnemy"))//�G
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"FallingRocks"))//����
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		
	}

}