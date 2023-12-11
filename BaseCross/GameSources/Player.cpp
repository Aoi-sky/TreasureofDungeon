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
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);
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
	}

	//�_���[�W�֐�
	void Player::AddPlayerDamage(int damage) {
		m_Life -= damage;
	}

	void Player::PlayerDead() {
		if (m_Life <= 0) {
			PostEvent(1.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameOverStage");
		}
	}
	
	void Player::OnCreate() {
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(0.2f, 0.2f, 0.2f);
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Vec3(0.0f, 0.85f, 0.0f));

		//�Փ˔����t����
		auto ptrColl = AddComponent<CollisionCapsule>();

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
		ptrDraw->AddAnimation(L"Wait", 0, 1, true, 10);
		ptrDraw->AddAnimation(L"Walk", 5,40 ,false, 10);
		ptrDraw->AddAnimation(L"Attack", 50, 60, false,10);
		ptrDraw->ChangeCurrentAnimation(L"Walk");



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
	}

	void Player::OnUpdate() {
		MovePlayer();
		PlayerDead();
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime(); // �O�t���[������̌o�ߎ��ԁi60FPS�j

		float elapsedTime = App::GetApp()->GetElapsedTime();
		//�A�j���[�V�������X�V����
		auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();
		ptrDraw->UpdateAnimation(elapsedTime);		


		auto device = app->GetInputDevice(); // �C���v�b�g�f�o�C�X�I�u�W�F�N�g���擾����
		auto& pad = device.GetControlerVec()[0]; // �P�ڂ̃R���g���[���[�̏�Ԃ��擾����
		// X�{�^���������ꂽ��e�𔭎˂���
		if (pad.wPressedButtons & BUTTON_SHOT)
		{
			ptrDraw->ChangeCurrentAnimation(L"Attack");

			// �v���C���[���������Ă���u�X�e�[�W�v���擾���A
			// ������Bullet�I�u�W�F�N�g��ǉ�����
			GetStage()->AddGameObject<Wave>(GetThis<Player>()); //�������g�̃I�u�W�F�N�g�̃|�C���^���擾����
		}
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Golem"))
		{
			AddPlayerDamage(5);
		}
	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		
	}

}
//end basecross

