/*!
@file Gimmick.cpp
@brief ���΂Ȃǂ̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	FallingRocks::FallingRocks(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Scale(Vec3(5.0f, 5.0f, 5.0f)),
		m_Rotation(Vec3(0,0,0)),
		m_Position(Vec3(0, 0, 0)),
		randPos(0),
		isFloor(false)
	{
		m_differenceMatrix.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
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
		int randx = rand() % 15 * xp;
		int randz = rand() % 30 * zm;

		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(Vec3(float(randx), 15.0f, float(randz)));
		randPos = Vec3(float(randx), 15.0f, float(randz));
		
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

		if (isFloor) {
			pos.y = 2.0f;
			// ���W��n�ʏ�ɐݒ�
			ptrTransform->SetPosition(randPos.x, pos.y, randPos.z);
			// �d�͂�0�ɐݒ�
			ptrGra->SetGravityVerocityZero();
		}
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
			return;
		}
		if (Other->FindTag(L"FixedCylinder")|| Other->FindTag(L"FallingRocks"))//���E����
		{
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			//GetStage()->AddGameObject<FallingRocks>();
			return;
		}
		if (Other->FindTag(L"Floor"))//��
		{
			isFloor = true;
		}
	}

	void FallingRocks::OnCollisionExit(shared_ptr<GameObject>& Other) {

	}

}
//end basecross
