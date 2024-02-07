/*!
@file MyCamera.h
@brief �J����
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class MyCamera :public Camera {
		weak_ptr<GameObject> m_TargetObject;//�ڕW�ƂȂ�I�u�W�F�N�g
		float m_ToTargetLerp;//�ڕW��ǂ�������ۂ̕�Ԓl
		bsm::Vec3 m_TargetToAt;//�ڕW���王�_�𒲐�����ʒu�x�N�g��
		float m_RadY;
		float m_RadXZ;
		float m_CameraUpDownSpeed;//�J�����̏㉺�X�s�[�h
		float m_CameraUnderRot;//�J�����������鉺���p�x

		Vec3 m_newAt;
		Vec3 m_newEye;
		Vec3 m_ArmVec;

		//�r�̒����̐ݒ�
		float m_ArmLen;
		float m_MaxArm;
		float m_MinArm;
		float m_RotSpeed;//��]�X�s�[�h
		float m_ZoomSpeed;//�Y�[���X�s�[�h
		bool m_LRBaseMode;//���E�X�e�B�b�N�ύX�̃��[�h
		bool m_UDBaseMode;//�㉺�X�e�B�b�N�ύX�̃��[�h

		int m_count;
	public:
		MyCamera();//�R���X�g���N�^
		MyCamera(float ArmLen);//�R���X�g���N�^�i������arm�̒����j
		virtual ~MyCamera();//�f�X�g���N�^
		//�J�����̈ʒu
		virtual void SetEye(const bsm::Vec3& Eye)override;
		virtual void SetEye(float x, float y, float z)override;
		//�J�����̖ڕW�I�u�W�F�N�g���擾
		shared_ptr<GameObject> GetTargetObject() const;
		void SetTargetObject(const shared_ptr<GameObject>& Obj);
		float GetToTargetLerp() const;
		void SetToTargetLerp(float f);
		float GetArmLengh() const;
		void UpdateArmLengh();
		float GetMaxArm() const;
		void SetMaxArm(float f);
		float GetMinArm() const;
		void SetMinArm(float f);
		float GetRotSpeed() const;
		void SetRotSpeed(float f);
		bsm::Vec3 GetTargetToAt() const;
		void SetTargetToAt(const bsm::Vec3& v);
		bool GetLRBaseMode() const;
		bool IsLRBaseMode() const;
		void SetLRBaseMode(bool b);
		bool GetUDBaseMode() const;
		bool IsUDBaseMode() const;
		void SetUDBaseMode(bool b);
		void CameraCollisionEnter();
		void CameraShake();
		virtual void SetAt(const bsm::Vec3& At)override;
		virtual void SetAt(float x, float y, float z)override;
		virtual void OnUpdate()override;
		float GetRadRX() const
		{
			return m_RadXZ;
		}
	};

}
//end basecross
