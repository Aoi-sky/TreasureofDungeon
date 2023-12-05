/*!
@file Gimmick.cpp
@brief ���΂Ȃǂ̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	FallingRocks::FallingRocks(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Scale(Vec3(3.0f, 3.0f, 3.0f)),
		m_Rotation(Vec3(0,0,0)),
		m_Position(Vec3(0, 0, 0))
	{
		m_differenceMatrix.affineTransformation(
			Vec3(0.8f, 0.8f, 0.8f),
			Vec3(0.0f),
			Vec3(0.0f, XM_PIDIV2, 0.0f),
			Vec3(0.0f, 0.2f, 0.0f)
		);

	}
	FallingRocks::~FallingRocks() {}

	void FallingRocks::OnCreate(){
		int i = rand() % 3;
		int xp,zm;
		if (i == 0){
			xp = 1;
			zm = 1;
		}else if (i == 1){
			xp = -1;
			zm = 1;
		}else if (i == 2){
			xp = 1;
			zm = -1;
		}else{
			xp = -1;
			zm = -1;
		}
		m_StartPos.x = float(rand() % 15) * xp;
		m_StartPos.z = float(rand() % 30) * zm;
		m_StartPos.y = 15.0f;

		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_StartPos);
		
		//�Փ˔����t����
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(true);
		ptrColl->SetFixed(false);

		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();

		//�e������
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"FALLINGROCKS");
		//�`�揈��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"FALLINGROCKS");
		ptrDraw->SetTextureResource(L"PLAYER");
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);

		AddTag(L"FallingRocks");
	}

	void FallingRocks::OnUpdate(){
		// �n�ʂ�艺�ɗ������Ȃ��ׂ̏���
		auto ptrTransform = GetComponent<Transform>();
		Vec3 pos = ptrTransform->GetPosition();
		auto ptrGra = AddComponent<Gravity>();

		// �n�ʂɗ��������𔻒�
		if (pos.y < 2.5f) {
			// �����Ă����ꍇ�̓t���O�𗧂Ă�
			m_fallenFlg = true;
		}

		// ��x�ł����ɗ����Ă����ꍇ�͒n�ʂɌŒ肷��
		if (m_fallenFlg) {
			pos.y = 1.5f;
			// �d�͂�0�ɐݒ�
			ptrGra->SetGravityVerocityZero();
		}
		pos.x = m_StartPos.x;
		pos.z = m_StartPos.z;
		// ���W��n�ʏ�ɐݒ�
		ptrTransform->SetPosition(pos);
	}

	void FallingRocks::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Wave"))
		{
			GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			GetStage()->AddGameObject<MoveFallingRocks>(GetThis<FallingRocks>());
			return;

		}
		if (Other->FindTag(L"Enemy"))//�G
		{
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			//GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			return;
		}
		if (Other->FindTag(L"FixedCylinder")|| Other->FindTag(L"FallingRocks"))//���E����
		{
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			GetStage()->AddGameObject<FallingRocks>();
			return;
		}
	}

	void FallingRocks::OnCollisionExit(shared_ptr<GameObject>& Other) {

	}

}
//end basecross
