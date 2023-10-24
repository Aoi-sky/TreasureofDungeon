/*!
@file Gimmick.cpp
@brief ���΂Ȃǂ̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	FallingRocks::FallingRocks(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Scale(Vec3(1.0f,2.0f,1.0f)),
		m_Rotation(Vec3(0,0,0)),
		m_Position(Vec3(0, 0, 0))
	{
	}
	FallingRocks::~FallingRocks() {}

	void FallingRocks::OnCreate(){
		int i = rand() % 3;
		int xp,zm;
		if (i == 0){
			xp = 1;
			zm = 1;
		}
		else if (i == 1){
			xp = -1;
			zm = 1;
		}
		else if (i == 2){
			xp = 1;
			zm = -1;
		}
		else{
			xp = -1;
			zm = -1;
		}
		int x = rand() % 15 * xp;
		int z = rand() % 30 * zm;

		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(Vec3(x, 15.0f, z));
		
		auto ptrColl = AddComponent<CollisionObb>();//OBB�Փ�j�����t����
		ptrColl->SetFixed(false);
		auto ptrGra = AddComponent<Gravity>();//�d�͂�����
		//�e������
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		//�`�揈��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"PLAYER");
		ptrDraw->SetOwnShadowActive(true);
	}


	void FallingRocks::OnUpdate(){
		
	}

	void FallingRocks::FallingRocksCreate()
	{
		
	}


}
//end basecross
