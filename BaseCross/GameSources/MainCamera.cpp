#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void MainCamera::OnUpdate()
	{
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();//�f���^�^�C��

		auto device = app->GetInputDevice();
		auto pad = device.GetControlerVec()[0];

		Vec3 padRightStick(pad.fThumbRX, 0.0f, pad.fThumbRY);

		//�X�e�B�b�N�̌X���ɉ����ĉ�荞��
		m_angle += padRightStick.x * delta;
		//�����_�̍��W
		Vec3 at = m_targetTrans ? m_targetTrans->GetPosition() : GetAt();

		//�����_����̑��΍��W
		auto relativePos = Vec3(
			cos(m_angle) * m_distance,
			4.0f,
			sin(m_angle)* m_distance);

		Vec3 eye = at + relativePos;
		
		SetEye(eye);
		SetAt(at);

	}

}