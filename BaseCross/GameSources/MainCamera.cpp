#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void MainCamera::OnUpdate()
	{
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();//デルタタイム

		auto device = app->GetInputDevice();
		auto pad = device.GetControlerVec()[0];

		Vec3 padRightStick(pad.fThumbRX, 0.0f, pad.fThumbRY);

		//スティックの傾きに応じて回り込む
		m_angle += padRightStick.x * delta;
		//注視点の座標
		Vec3 at = m_targetTrans ? m_targetTrans->GetPosition() : GetAt();

		//注視点からの相対座標
		auto relativePos = Vec3(
			cos(m_angle) * m_distance,
			4.0f,
			sin(m_angle)* m_distance);

		Vec3 eye = at + relativePos;
		
		SetEye(eye);
		SetAt(at);

	}

}