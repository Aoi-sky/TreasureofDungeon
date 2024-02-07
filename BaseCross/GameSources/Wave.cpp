#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Wave::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");//�e�̌`
		drawComp->SetDrawActive(false);

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

		//�G�t�F�N�g�̏�����
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		auto ShEfkInterface = App::GetApp()->GetScene<Scene>()->GetEfkInterface();
		m_damageEffectStr = DataDir + L"Effects\\" + L"damage.efk";
		m_damageEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, m_damageEffectStr);


	}

	void Wave::OnUpdate() {
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();//�f���^�^�C�����擾

		//�ړ�����
		auto pos = m_transform->GetPosition();
		pos += m_forward * m_speed * delta;
		m_transform->SetPosition(pos.x, 1,pos.z);

	}
	void Wave::OnCollisionEnter(shared_ptr<GameObject>& Other) {

		const auto& stage = GetStage();

		if (Other->FindTag(L"FixedCylinder"))//��
		{
			stage->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Wall"))//��
		{
			stage->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Golem"))//�G
		{
			auto pos = m_transform->GetPosition();
			m_DamageEfkPlay = ObjectFactory::Create<EfkPlay>(m_damageEffect, pos, Vec3(0.1f));

			const auto& player = stage->GetSharedGameObject<Player>(L"Player");

			stage->RemoveGameObject<Wave>(GetThis<Wave>());
			stage->GetSharedGameObject<Golem>(L"Golem")->AddDamage(player->GetOffensiveAbility());
			return;
		}
		if (Other->FindTag(L"ShotEnemy"))//�G
		{
			auto pos = m_transform->GetPosition();
			m_DamageEfkPlay = ObjectFactory::Create<EfkPlay>(m_damageEffect, pos, Vec3(5.0f));
			stage->RemoveGameObject<Wave>(GetThis<Wave>());

			return;
		}
		if (Other->FindTag(L"FallingRocks"))//����
		{
			stage->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		else {
			stage->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		
	}

}