/*!
@file Gimmick.cpp
@brief óéêŒÇ»Ç«ÇÃé¿ëÃ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	FallingRocks::FallingRocks(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Scale(Vec3(5.0f, 5.0f, 5.0f)),
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
		int x = rand() % 15 * xp;
		int z = rand() % 30 * zm;

		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(Vec3(float(x), 15.0f, float(z)));
		
		auto ptrColl = AddComponent<CollisionSphere>();//OBBè’ìÀjîªíËÇïtÇØÇÈ
		ptrColl->SetFixed(false);
		ptrColl->SetDrawActive(true);

		auto ptrGra = AddComponent<Gravity>();//èdóÕÇÇ¬ÇØÇÈ
		//âeÇÇ¬ÇØÇÈ
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"FALLINGROCKS");
		//ï`âÊèàóù
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"FALLINGROCKS");
		ptrDraw->SetTextureResource(L"PLAYER");
		ptrDraw->SetOwnShadowActive(true);

		AddTag(L"FallingRocks");
	}

	void FallingRocks::OnUpdate(){
		
	}

	void FallingRocks::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Wave"))//è’åÇîg
		{
			GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			GetStage()->AddGameObject<MoveFallingRocks>(GetThis<FallingRocks>());
			return;
		}
		if (Other->FindTag(L"Enemy"))//ìG
		{
			GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			return;
		}
		if (Other->FindTag(L"FixedCylinder")|| Other->FindTag(L"FallingRocks"))//íåÅEóéêŒ
		{
			GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			GetStage()->AddGameObject<FallingRocks>();
			return;
		}
		if (Other->FindTag(L"FixedFloor"))//è∞
		{

		}
	}
}
//end basecross
