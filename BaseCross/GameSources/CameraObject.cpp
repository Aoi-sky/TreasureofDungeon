#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void CameraObject::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");//�e�̌`
		drawComp->SetDrawActive(true);

		//�������x�N�g���ŏ���
		auto ownerTrans = m_owner->GetEye();
		m_transform = GetComponent<Transform>();//�R���|�[�l���g�̎擾
		m_transform->SetPosition(ownerTrans);//���W�ݒ�
		m_transform->SetScale(Vec3(5.0f));//�T�C�Y����
		auto ptrColl = AddComponent<CollisionSphere>();//�R���W����������
		ptrColl->SetAfterCollision(AfterCollision::None);//�Փ˔���
		ptrColl->SetSleepActive(true);
		ptrColl->SetDrawActive(true);

		AddTag(L"CameraObject");//�^�O�̐ݒ�
	}

	void CameraObject::OnUpdate() {
		auto ownerTrans = m_owner->GetEye();
		m_transform->SetPosition(ownerTrans);
	}


	void CameraObject::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Wall"))//��
		{
			m_owner->OnUpdate2();
			//GetStage()->AddGameObject<MyCamera>()->CameraCollisionEnter();
			return;
		}
		if (Other->FindTag(L"FixedCylinder"))//��
		{
			return;
		}
	}

}