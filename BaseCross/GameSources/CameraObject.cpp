#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void CameraObject::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");//�e�̌`
		drawComp->SetDrawActive(false);

		//�������x�N�g���ŏ���
		auto ownerTrans = m_owner->GetEye();
		m_transform = GetComponent<Transform>();//�R���|�[�l���g�̎擾
		m_transform->SetPosition(ownerTrans);//���W�ݒ�
		m_transform->SetScale(Vec3(0.1f));//�T�C�Y����
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W����������
		ptrColl->SetAfterCollision(AfterCollision::None);//�Փ˔���
		ptrColl->SetSleepActive(true);
		ptrColl->SetDrawActive(false);

		AddTag(L"CameraObject");//�^�O�̐ݒ�
	}

	void CameraObject::OnUpdate() {
		auto ownerTrans = m_owner->GetEye();
		m_transform->SetPosition(ownerTrans);

		auto pos = m_transform->GetPosition();
		if (pos.x < -25.0f - 2.0f || pos.x > 25.0f - 2.0f || pos.z < -40.0f - 2.0f || pos.z > 40.0f - 2.0f) {
			m_owner->CameraCollisionEnter();
		}		
	}


	void CameraObject::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Wall"))//��
		{
			m_owner->CameraCollisionEnter();
			return;
		}
		if (Other->FindTag(L"FixedCylinder"))//��
		{
			m_owner->CameraCollisionEnter();
			return;
		}
	}

}