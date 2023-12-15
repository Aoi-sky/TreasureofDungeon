/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		shared_ptr<SoundItem> m_bgm; // �T�E���h�A�C�e��

		//�r���[�̍쐬
		void CreateViewLight();
		//�ǂ̍쐬
		void CreateWall();
		//���̍쐬
		void CreateFloor();
		//���̍쐬
		void CreatePiller();
		//�v���C���[�̍쐬
		void CreatePlayer();
		//�G�̍쐬
		void CreateEnemy();
		//�Z���}�b�v�̐���
		void CreateStageCellMap();
		// �Z���}�b�v�̃R�X�g��ݒ�
		void SetCellMapCost();
		//���΃M�~�b�N
		void CreateFallingRocks();
		// �S�[�����̐���
		void CreateGolem();

		//BGM�̐���
		void PlayBGM();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		
		virtual void OnCreate()override;
		virtual void OnDestroy()override;
		virtual void OnDraw() override;
	};


}
//end basecross

