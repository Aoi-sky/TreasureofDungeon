/*!
@file ClearStage.cpp
@brief �N���A���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void ClearStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	void ClearStage::CreateClearSprite() {

		AddGameObject<ClearSprite>();
		AddGameObject<ButtonSprite>();

	} // end CreateTitleSprite

	void ClearStage::PlayBGM() {
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		m_bgm = XAPtr->Start(L"CLEARSTAGE_BGM", XAUDIO2_LOOP_INFINITE, 0.1f);
	}

	void ClearStage::OnDestroy(){
		//BGM�̃X�g�b�v
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Stop(m_bgm);
	}

	void ClearStage::OnCreate() {
		CreateViewLight();
		CreateClearSprite();
		PlayBGM();
	}

	void ClearStage::OnUpdate() {
		auto& app = App::GetApp();
		auto device = app->GetInputDevice();
		auto& pad = device.GetControlerVec()[0];

		if (pad.wPressedButtons & XINPUT_GAMEPAD_B) {

			PostEvent(0.3f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
			auto XAPtr = App::GetApp()->GetXAudio2Manager();
			XAPtr->Start(L"SELECT_SE", 0, 0.5f);
		}


	}
}
//end basecross
