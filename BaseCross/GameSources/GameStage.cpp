/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	//�J�����̐���
	void GameStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto ptrMyCamera = ObjectFactory::Create<MyCamera>();
		ptrView->SetCamera(ptrMyCamera);
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();

		AddGameObject<CameraObject>(ptrMyCamera);
	}

	
	// �Z���}�b�v�̐���
	void GameStage::CreateStageCellMap()
	{
		//�Z���}�b�v�𐶐�
		auto Ptr = AddGameObject<StageCellMap>(Vec3(0.0f, 0.0f, 0.0f), 1.0f, 50, 80);

		//�Z���}�b�v�̋���\��
		Ptr->SetDrawActive(false);

		//�V�F�A�I�u�W�F�N�g�ɓo�^
		SetSharedGameObject(L"StageCellMap", Ptr);
	}

	// �Z���}�b�v���̃Z���ɃR�X�g��ݒ�
	void GameStage::SetCellMapCost() {
		////�Z���}�b�v����FixedBox�̏����Z�b�g
		//auto PtrCellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		//auto BoxGroup = GetSharedObjectGroup(L"CellMap");
		////�Z���}�b�v����Z���̔z����擾
		//auto& CellVec = PtrCellmap->GetCellVec();
		////�{�b�N�X�O���[�v����{�b�N�X�̔z����擾
		//auto& BoxVec = BoxGroup->GetGroupVector();
		//vector<AABB> ObjectsAABBVec;
		//for (auto& v : BoxVec) {
		//	auto FixedBoxPtr = dynamic_pointer_cast<FixedBox>(v.lock());
		//	if (FixedBoxPtr) {
		//		auto ColPtr = FixedBoxPtr->GetComponent<CollisionObb>();
		//		//�{�b�N�X�̏Փ˔��肩���b�s���O����AABB���擾���ĕۑ�
		//		ObjectsAABBVec.push_back(ColPtr->GetObb().GetWrappedAABB());
		//	}
		//}
		////�Z���z�񂩂�Z�����X�L����
		//for (auto& v : CellVec) {
		//	for (auto& v2 : v) {
		//		for (auto& vObj : ObjectsAABBVec) {
		//			if (HitTest::AABB_AABB_NOT_EQUAL(v2.m_PieceRange, vObj)) {
		//				//�{�b�N�X��ABB��NOT_EQUAL�ŏՓ˔���
		//				v2.m_Cost = -1;
		//				break;
		//			}
		//		}
		//	}
		//}
	}

	// �ǂ̐���
	void GameStage::CreateWall() {
		vector<vector<Vec3>> vec = {
			{//�k��
				Vec3(50.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,10.0f,40.0f)//Position
			},
			{//���
				Vec3(50.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,10.0f,-40.0f)//Position
			},
			{//����
				Vec3(5.0f, 20.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(25.0f,10.0f,0.0f)//Position
			},
			{//����
				Vec3(5.0f, 20.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-25.0f,10.0f,0.0f)//Position
			}
		};
		for (auto v : vec) {
			AddGameObject<Wall>(v[0], v[1], v[2]);
		}

		vector<vector<Vec3>> vec2 = {
			{//�V��
				Vec3(50.0f, 1.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,20.0f,0.0f)//Position
			}
		};
		for (auto v : vec2) {
			AddGameObject<Wall>(v[0], v[1], v[2]);
		}
	}

	// ���̐���
	void GameStage::CreateFloor() {
		// �n�ʂ̐���
		vector<vector<Vec3>> vec = {
			{
				Vec3(50.0f, 1.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,0.0f,0.0f)//Position
			},
		};
		for (auto v : vec) {
			AddGameObject<Floor>(v[0], v[1], v[2]);
		}
	}

	// ���̐���
	void GameStage::CreatePiller() {
		vector<vector<Vec3>> vec = {
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(15.0f,10.0f,-25.0f)//Position
			},
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(15.0f,10.0f,25.0f)//Position
			},
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-15.0f,10.0f,-25.0f)//Position
			},
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-15.0f,10.0f,25.0f)//Position
			},
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(15.0f,10.0f,0.0f)//Position
			},
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-15.0f,10.0f,0.0f)//Position
			},
		};
		for (auto v : vec) {
			AddGameObject<FixedCylinder>(v[0], v[1], v[2]);
		}
	}

	
	//�v���C���[�̐���
	void GameStage::CreatePlayer() {
		auto ptrPlayer = AddGameObject <Player>();
		SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");

		AddGameObject<PlayerHpBar>();
	}

	//�G�̍쐬
	void GameStage::CreateEnemy() {
		//auto Ptrcellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		//AddGameObject<Enemy>(Ptrcellmap, Vec3(0, 1.0f, 15.0f));
		AddGameObject<ShotEnemy>(Vec3(10.0f,2.0f,20.0f));
		AddGameObject<ShotEnemy>(Vec3(10.0f,2.0f,-20.0f));
		AddGameObject<ShotEnemy>(Vec3(-10.0f,2.0f,20.0f));
		AddGameObject<ShotEnemy>(Vec3(-10.0f,2.0f,-20.0f));
	}

	void GameStage::CreateGolem() {
		auto ptrGolem = AddGameObject<Golem>(Vec3(0, 2.5f, 20.0f));
		SetSharedGameObject(L"Golem", ptrGolem);
		AddGameObject<GolemHpBar>();
	}

	//���΂̍쐬
	void GameStage::CreateFallingRocks() {
		AddGameObject<FallingRocks>();
	}
	

	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̐���
			CreateViewLight();
			//�ǂ̐���
			CreateWall();
			//���̐���
			CreateFloor();
			//���̐���
			CreatePiller();
			//�Z���}�b�v�̐���
			CreateStageCellMap();
			//�v���C���[�̐���
			CreatePlayer();
			//�G�̐���
			CreateEnemy();
			// �S�[�����̐���
			CreateGolem();
			//���΂̍쐬
			CreateFallingRocks();
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnDraw()
	{
		// �f�o�b�O������������I�ɋ�ɂ���
		App::GetApp()->GetScene<Scene>()->SetDebugString(L"");

		// �p�����̕`�掞�̊֐������s����
		Stage::OnDraw();

		// �f�o�b�N�p������̕\����\���؂�ւ�
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawActive(true);
	}
}
//end basecross
