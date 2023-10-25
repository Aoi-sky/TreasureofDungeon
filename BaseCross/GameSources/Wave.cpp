#include "stdafx.h"
#include "Project.h"



namespace basecross {
	void Wave::OnCreate()
	{
		//�e�̌`
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");

		//�������x�N�g���ŏ���
		auto ownerTrans = m_owner->GetComponent<Transform>();
		m_forward = ownerTrans->GetForward();

		//�����̃g�����X�R���|�[�l���g���������č��W��傫����ݒ�
		m_transform = GetComponent<Transform>();
		m_transform->SetPosition(ownerTrans->GetPosition());
		m_transform->SetScale(Vec3(1.0f,0.5f,1.0f));
		//�R���W����������
		auto ptrColl = AddComponent<CollisionSphere>();
		//�Փ˔���
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);

		AddTag(L"Wave");
	}

	void Wave::OnUpdate()
	{
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();//�f���^�^�C�����擾

		//�ړ�����
		auto pos = m_transform->GetPosition();//���݂̍��W������
		pos += m_forward * m_speed * delta;
		m_transform->SetPosition(pos);

	}
	void Wave::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedCylinder"))//��
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			GetStage()->AddGameObject<FallingRocks>();
			return;
		}
		if (Other->FindTag(L"FixedBox"))//��
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Golrm"))//�S�[����
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