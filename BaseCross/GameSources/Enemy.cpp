#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Enemy::Enemy(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<StageCellMap>& CellMap,
		const Vec3& Position) :
		GameObject(StagePtr),
		m_CelMap(CellMap),
		m_StartPosition(Position),
		m_Force(0),
		m_Velocity(0),
		m_Speed(0.05f)
	{}
	Enemy::~Enemy(){}
	//初期化
	void Enemy::OnCreate() {
		auto ptr = GetComponent<Transform>();
		ptr->SetScale(0.5f, 0.5f, 0.5f);
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(m_StartPosition);
		AddComponent<CollisionSphere>();
		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			AddComponent<PathSearch>(MapPtr);
		}
		//描画コンポーネントの設定
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		//描画するメッシュを設定
		ptrDraw->SetMeshResource(L"GOLRM");
		ptrDraw->SetDiffuse(Col4(1.0f, 0, 0, 1.0f));

		AddTag(L"Golrm");
	}
	void Enemy::OnUpdate() {
		m_Force.setAll(0.0f);
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		PlayerPos.y = GetStartPosition().y;
		if (SeekBehavior(PlayerPos) == CellSearchFlg::Failed) {
			if (SeekBehavior(GetStartPosition()) == CellSearchFlg::Arrived) {
				ArriveBehavior(GetStartPosition());
			}
		}
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_Velocity += m_Force * ElapsedTime * m_Speed;
		auto EnemyPos = GetComponent<Transform>()->GetPosition();
		if (length(EnemyPos - PlayerPos) <= 1.8f) {
			m_Velocity *= 0.95f;
		}
		auto Pos = GetComponent<Transform>()->GetPosition();
		Pos += m_Velocity * ElapsedTime;
		GetComponent<Transform>()->SetPosition(Pos);

	}
	bool Enemy::Search(const Vec3& TargetPos) {
		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			auto PathPtr = GetComponent<PathSearch>();
			m_CellPath.clear();
			if (PathPtr->SearchCell(TargetPos, m_CellPath)) {//パス検索をかける
				//検索が成功した
				m_CellIndex = 0;
				m_TargetCellIndex = (int)(m_CellPath.size() - 1);
				if (m_CellIndex == m_TargetCellIndex) {
					m_NextCellIndex = m_CellIndex;//すでに同じセルにいる
				}
				else {
					m_NextCellIndex = m_CellIndex + 1;//離れている
				}
				return true;
			}
			else {
				//失敗した
				m_CellIndex = -1;
				m_NextCellIndex = -1;
				m_TargetCellIndex = -1;
			}
		}
		return false;
	}
	CellSearchFlg Enemy::SeekBehavior(const Vec3& TargetPos) {
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto MapPtr = m_CelMap.lock();
		if (!MapPtr) {
			return CellSearchFlg::Failed;
		}
		if (Search(TargetPos)) {
			auto PtrSeek = GetBehavior<SeekSteering>();
			auto PtrArrive = GetBehavior<ArriveSteering>();
			if (m_NextCellIndex == 0) {
				m_Force = PtrArrive->Execute(m_Force, m_Velocity, TargetPos);
				//到着した
				return CellSearchFlg::Arrived;
			}
			else {
				AABB ret;
				MapPtr->FindAABB(m_CellPath[m_NextCellIndex], ret);
				auto Pos = ret.GetCenter();
				Pos.y = TargetPos.y;
				m_Force = PtrSeek->Execute(m_Force, m_Velocity, Pos);
				return CellSearchFlg::Seek;
			}
		}
		else {
			auto PtrSeek = GetBehavior<SeekSteering>();
			CellIndex PlayerCell;
			if (MapPtr->FindCell(TargetPos, PlayerCell)) {
				AABB ret;
				MapPtr->FindAABB(PlayerCell, ret);
				auto Pos = ret.GetCenter();
				Pos.y = TargetPos.y;
				m_Force = PtrSeek->Execute(m_Force, m_Velocity, Pos);
				return CellSearchFlg::Seek;
			}
		}
		return CellSearchFlg::Failed;
	}

	void Enemy::ArriveBehavior(const Vec3& TargetPos) {
		auto PtrArrive = GetBehavior<ArriveSteering>();
		m_Force = PtrArrive->Execute(m_Force, m_Velocity, TargetPos);
	}
}


