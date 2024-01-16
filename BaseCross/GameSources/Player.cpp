/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Speed(5.0f),
		m_Life(m_DefaultLife)
	{
		m_differenceMatrix.affineTransformation(
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.8f, 0.0f)
		);
	}
	
	void Player::OnCreate() {
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(0.5f, 0.5f, 0.5f);
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Vec3(0.0f, 0.85f, 0.0f));

		//�Փ˔����t����
		auto ptrColl = AddComponent<CollisionCapsule>();
		ptrColl->SetDrawActive(false);

		//�e�p�t�H�[�}���X�𓾂�
		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

		auto ptrGracity = AddComponent<Gravity>();

		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto shadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMultiMeshResource(L"M_PLAYER");

		//�`��R���|�[�l���g�̐ݒ�
		auto  ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		// �V�K�h���[�R���|�[�l���g�̐ݒ�
		ptrDraw->SetMultiMeshResource(L"M_PLAYER");
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);
		//�A�j���[�V�����ݒ�
		ptrDraw->AddAnimation(L"Wait", 10, 30, true);
		ptrDraw->AddAnimation(L"WalkStart", 50, 10, false);
		ptrDraw->AddAnimation(L"Walking1", 60, 10, false);
		ptrDraw->AddAnimation(L"Walking2", 70, 10, false);
		ptrDraw->AddAnimation(L"WalkEnd1", 80, 10, false);
		ptrDraw->AddAnimation(L"WalkEnd2", 100, 10, false);
		ptrDraw->AddAnimation(L"AttackStart", 120, 35, false);
		ptrDraw->AddAnimation(L"AttackEnd", 155, 25, false);
		ptrDraw->AddAnimation(L"Damage1", 190, 10, false);
		ptrDraw->AddAnimation(L"Damage2", 210, 90, false);
		ptrDraw->AddAnimation(L"Dead", 210, 30, false);
		ptrDraw->ChangeCurrentAnimation(L"Wait");

		//��������
		SetAlphaActive(true);
		SetDrawActive(true);

		// �^�O�̐ݒ�
		AddTag(L"Player");

		//�J�������擾
		auto ptrCamera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (ptrCamera) {
			//MyCamera�ł���
			//MyCamera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			ptrCamera->SetTargetObject(GetThis<GameObject>());
			ptrCamera->SetTargetToAt(Vec3(0, 0.25f, 0));
		}

		//�G�t�F�N�g�̏�����
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		auto ShEfkInterface = App::GetApp()->GetScene<Scene>()->GetEfkInterface();
		m_damageEffectStr = DataDir + L"Effects\\" + L"damage.efk";
		m_damageEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, m_damageEffectStr);

	}

	void Player::OnUpdate() {
		MovePlayer();
		PlayerDead();
		HitCheck();

		auto& app = App::GetApp();
		auto device = app->GetInputDevice(); // �C���v�b�g�f�o�C�X�I�u�W�F�N�g���擾����
		auto& pad = device.GetControlerVec()[0]; // �P�ڂ̃R���g���[���[�̏�Ԃ��擾����
		switch (m_currentMotion)
		{
		case Wait:
		case WalkStart:
		case Walking1:
		case Walking2:
		case WalkEnd1:
		case WalkEnd2:
			// X�{�^���������ꂽ��U�����[�V�����ɕύX
			if (pad.wPressedButtons & BUTTON_SHOT)
			{
				m_currentMotion = AttackStart;
			}
			break;
		default:
			break;
		}

		AnimationUpdate();
	}

	Vec2 Player::GetInputState() const{
		Vec2 ret;
		auto controllerVec = App::GetApp()->GetInputDevice().GetControlerVec();//�R���g���[���擾
		ret.x = 0.0f;
		ret.y = 0.0f;
		if (controllerVec[0].bConnected)
		{
			ret.x = controllerVec[0].fThumbLX;
			ret.y = controllerVec[0].fThumbLY;
		}

		return ret;
	}
	//player�̈ړ�
	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);
		auto inPut = GetInputState();//���͂̎擾
		float moveX = inPut.x;
		float moveZ = inPut.y;
		if (moveX != 0 || moveZ != 0) {
			float moveLength = 0;	//���������̃X�s�[�h
			auto ptrTransform = GetComponent<Transform>();
			auto ptrCamera = OnGetDrawCamera();
			auto front = ptrTransform->GetPosition() - ptrCamera->GetEye();//�i�s�����̌������v�Z
			front.y = 0;
			front.normalize();
			float frontAngle = atan2(front.z, front.x);//�i�s������������̊p�x���Z�o
			//�R���g���[���̌����v�Z
			Vec2 moveVec(moveX, moveZ);
			float moveSize = moveVec.length();
			float cntlAngle = atan2(-moveX, moveZ);//�R���g���[���̌�������p�x���v�Z
			float totalAngle = frontAngle + cntlAngle;//�g�[�^���̊p�x���Z�o
			angle = Vec3(cos(totalAngle), 0, sin(totalAngle));//�p�x����x�N�g�����쐬
			angle.normalize();//���K������
			angle *= moveSize;//�ړ��T�C�Y��ݒ�
			angle.y = 0;//Y���͕ω������Ȃ�
		}
		return angle;
	}

	void Player::MovePlayer() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		auto angle = GetMoveVector();
		if (angle.length() > 0.0f && m_currentMotion == Wait) {
			m_currentMotion = WalkStart;
		}
		switch (m_currentMotion)
		{
		case Wait:
		case WalkStart:
		case Walking1:
		case Walking2:
		case WalkEnd1:
		case WalkEnd2:
			if (angle.length() > 0.0f) {
				auto pos = GetComponent<Transform>()->GetPosition();
				pos += angle * elapsedTime * m_Speed;
				GetComponent<Transform>()->SetPosition(pos);
			}
			//��]�̌v�Z
			if (angle.length() > 0.0f) {
				auto utilPtr = GetBehavior<UtilBehavior>();
				utilPtr->RotToHead(angle, 1.0f);
			}
			break;
		case AttackStart:
			//��]�̌v�Z
			if (angle.length() > 0.0f) {
				auto utilPtr = GetBehavior<UtilBehavior>();
				utilPtr->RotToHead(angle, 1.0f);
			}
			break;
		default:
			break;
		}		
	}

	void Player::OnAttack() {
		// �v���C���[���������Ă���u�X�e�[�W�v���擾���A
		// ������Bullet�I�u�W�F�N�g��ǉ�����
		GetStage()->AddGameObject<Wave>(GetThis<Player>()); //�������g�̃I�u�W�F�N�g�̃|�C���^���擾����
	}

	void Player::HitCheck() {
		totalTime += totalTime < invincibleTime;
		if (HitFlg && totalTime >= invincibleTime) {
			AddPlayerDamage(10, Damage1);
			totalTime = 0;
		}
	}

	//�_���[�W�֐�
	void Player::AddPlayerDamage(int damage, eMotion Motion) {
		switch (m_currentMotion)
		{
		case Wait:
		case WalkStart:
		case Walking1:
		case Walking2:
		case WalkEnd1:
		case WalkEnd2:
			m_currentMotion = Motion;
			break;
		case AttackStart:
		case AttackEnd:
			if (Motion == Damage2) {
				m_currentMotion = Motion;
			}
			break;

		default:
			break;
		}

		m_Life -= damage;
		if (m_Life < 0) {
			m_Life = 0;
		}

		Vec3 pos = GetComponent<Transform>()->GetPosition();
		m_DamegeEfkPlay = ObjectFactory::Create<EfkPlay>(m_damageEffect, pos, Vec3(0.5f));
	}

	void Player::PlayerDead() {
		if (m_Life <= 0) {
			m_currentMotion = Dead;
		}
	}

	void Player::AnimationUpdate()
	{
		// �A�j���[�V�����̍Đ�
		auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();
		// �A�j���[�V�����̃^�C�v���ς���Ă�����
		if (m_currentMotion != m_pastMotion || ptrDraw->GetCurrentAnimation() != m_motionKey.at(m_currentMotion))
		{
			// �^�C�v�ɉ����ăA�j���[�V������ύX����
			ptrDraw->ChangeCurrentAnimation(m_motionKey.at(m_currentMotion));
			m_pastMotion = m_currentMotion;
		}

		// �O�t���[������̃f���^�^�C�����擾
		float deltaTime = App::GetApp()->GetElapsedTime();

		// ���[�V�����ɉ����čĐ�����
		switch (m_currentMotion)
		{
		case Wait:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		case WalkStart:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		case Walking1:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		case Walking2:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		case WalkEnd1:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		case WalkEnd2:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		case AttackStart:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		case AttackEnd:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		case Damage1:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		case Damage2:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		case Dead:
			ptrDraw->UpdateAnimation(deltaTime * 0.5f);
			break;
		default:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		}

		// �A�j���[�V�������I�����Ă��邩���`�F�b�N
		if (ptrDraw->IsTargetAnimeEnd()) {
			auto angle = GetMoveVector();

			switch (m_pastMotion)
			{
			case Wait:
				break;
			case WalkStart:
				if (angle.length() > 0.0f) {
					m_currentMotion = Walking1;
				}
				else {
					m_currentMotion = WalkEnd1;
				}
				break;
			case Walking1:
				if (angle.length() > 0.0f) {
					m_currentMotion = Walking2;
				}
				else {
					m_currentMotion = WalkEnd2;
				}
				break;
			case Walking2:
				if (angle.length() > 0.0f) {
					m_currentMotion = Walking1;
				}
				else {
					m_currentMotion = WalkEnd1;
				}
				break;
			case AttackStart:
				OnAttack();
				m_currentMotion = AttackEnd;
				break;
			case AttackEnd:
			case Damage1:
			case Damage2:
				if (angle.length() > 0.0f) {
					m_currentMotion = WalkStart;
				}
				else {
					m_currentMotion = Wait;
				}
				break;
			case Dead:
				SetDrawActive(false);
				PostEvent(1.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameOverStage");
				break;
			default:
				m_currentMotion = Wait;
				break;
			}
		}
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Golem"))
		{
			HitFlg = true;
		}
	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Golem"))
		{
			HitFlg = false;
		}
	}

}
//end basecross

