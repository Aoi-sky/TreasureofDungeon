/*!
@file GimmickMove.cpp
@brief ���΂̈ړ�
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void MoveFallingRocks::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"FALLINGROCKS");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"FALLINGROCKS");
		ptrDraw->SetTextureResource(L"PLAYER");
		ptrDraw->SetOwnShadowActive(true);

		//�������x�N�g���ŏ���
		m_Positon = m_owner->GetComponent<Transform>()->GetPosition();
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		m_forward = ptrPlayer->GetComponent<Transform>()->GetForward();

		m_transform = GetComponent<Transform>();//�R���|�[�l���g�̎擾
		m_transform->SetPosition(m_Positon.x, 3.0f, m_Positon.z);//���W�ݒ�
		m_transform->SetScale(Vec3(5.0f, 5.0f, 5.0f));//�T�C�Y����
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W����������
		//�Փ˔���
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);

		AddTag(L"MoveFallingRocks");//�^�O�̐ݒ�
	}

	void MoveFallingRocks::OnUpdate() {
		float delta = App::GetApp()->GetElapsedTime();//�f���^�^�C�����擾

		//�ړ�����
		auto pos = m_transform->GetPosition();
		pos += m_forward * m_speed * delta;
		m_transform->SetPosition(pos.x,5,pos.z);

	}
	void MoveFallingRocks::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedCylinder") || Other->FindTag(L"FallingRocks"))//��
		{
			GetStage()->RemoveGameObject<MoveFallingRocks>(GetThis<MoveFallingRocks>());
			return;
		}
		if (Other->FindTag(L"Wall"))//��
		{
			GetStage()->RemoveGameObject<MoveFallingRocks>(GetThis<MoveFallingRocks>());
			return;
		}
		if (Other->FindTag(L"Enemy"))
		{
			GetStage()->RemoveGameObject<MoveFallingRocks>(GetThis<MoveFallingRocks>());
			return;
		}
	}
}
//end basecross
