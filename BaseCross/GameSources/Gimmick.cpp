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
		auto playerPos = GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();

		m_StartPos.x = playerPos.x + float(rand() % 10) * xp + 2.0f;
		m_StartPos.z = playerPos.z + float(rand() % 10) * zm + 2.0f;
		m_StartPos.y = 15.0f;

		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_StartPos);
		
		//�Փ˔����t����
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(false);
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

		//�G�t�F�N�g�̏�����
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		auto ShEfkInterface = App::GetApp()->GetScene<Scene>()->GetEfkInterface();
		m_damageEffectStr = DataDir + L"Effects\\" + L"spark.efk";
		m_damageEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, m_damageEffectStr);

	}

	void FallingRocks::OnUpdate(){
		// �n�ʂ�艺�ɗ������Ȃ��ׂ̏���
		auto ptrTransform = GetComponent<Transform>();
		Vec3 pos = ptrTransform->GetPosition();
		auto ptrGra = AddComponent<Gravity>();

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

		if (pos.y < -5.0f) {
			GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			GetStage()->AddGameObject<FallingRocks>();
		}

	}

	void FallingRocks::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Wave"))
		{
			GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			GetStage()->AddGameObject<MoveFallingRocks>(GetThis<FallingRocks>());

			auto pos = GetComponent<Transform>()->GetPosition();
			m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_damageEffect, pos, Vec3(1.0f));

			return;

		}
		if (Other->FindTag(L"Enemy") || Other->FindTag(L"Golem"))//�G
		{
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			return;
		}
		if (Other->FindTag(L"FixedCylinder")|| Other->FindTag(L"FallingRocks"))//���E����
		{
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			GetStage()->AddGameObject<FallingRocks>();
			return;
		}
		if (Other->FindTag(L"Floor"))//�G
		{
			m_fallenFlg = true;
			auto XAPtr = App::GetApp()->GetXAudio2Manager();
			XAPtr->Start(L"STONE_SE", 0, 2.0f);
			return;
		}

	}

	void FallingRocks::OnCollisionExit(shared_ptr<GameObject>& Other) {

	}

}
//end basecross
