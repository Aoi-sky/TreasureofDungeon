/* !
@file ClearSprite.h
@brief �N���A�V�[���̔w�i�̎���
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	// �R���X�g���N�^
	ClearSprite::ClearSprite(const shared_ptr<Stage>& stage) :
		Sprites(stage),
		m_DefaultSize(Vec2((float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight()))
	{
	}
	// �f�X�g���N�^
	ClearSprite::~ClearSprite() {}

	// ������
	void ClearSprite::OnCreate()
	{
		Sprites::CreateSprite(Vec3(-m_DefaultSize.x / 2, m_DefaultSize.y / 2, 0.0f), m_DefaultSize, L"CLEAR");
	}

	// �X�V����
	void ClearSprite::OnUpdate()
	{
		Sprites::OnUpdate();
	}
}