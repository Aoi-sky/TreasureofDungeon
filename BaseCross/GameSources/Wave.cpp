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
		m_transform->SetScale(Vec3(0.5f));
		//�R���W����������
		auto ptrColl = AddComponent<CollisionSphere>();
		//�Փ˔���
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);

	}

	void Wave::OnUpdate()
	{
		auto& app = App::GetApp();
		//�f���^�^�C�����擾
		float delta = app->GetElapsedTime();

		//�ړ�����
		auto pos = m_transform->GetPosition();//���݂̍��W������
		pos += m_forward * m_speed * delta;
		m_transform->SetPosition(pos);

	}
	void Wave::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedCylinder")|| Other->FindTag(L"FixedBox"))
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
	}

}