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
		//�r���[�̍쐬
		void CreateViewLight();
		//�t�B�[���h�̍쐬
		void CreateField();
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
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
	};


}
//end basecross

