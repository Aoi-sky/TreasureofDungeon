/*!
@file MyCamera.h
@brief カメラ
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class MyCamera :public Camera {
		weak_ptr<GameObject> m_TargetObject;//目標となるオブジェクト
		float m_ToTargetLerp;//目標を追いかける際の補間値
		bsm::Vec3 m_TargetToAt;//目標から視点を調整する位置ベクトル
		float m_RadY;
		float m_RadXZ;
		float m_CameraUpDownSpeed;//カメラの上下スピード
		float m_CameraUnderRot;//カメラを下げる下限角度

		Vec3 m_newAt;
		Vec3 m_newEye;
		Vec3 m_ArmVec;

		//腕の長さの設定
		float m_ArmLen;
		float m_MaxArm;
		float m_MinArm;
		float m_RotSpeed;//回転スピード
		float m_ZoomSpeed;//ズームスピード
		bool m_LRBaseMode;//左右スティック変更のモード
		bool m_UDBaseMode;//上下スティック変更のモード

		int m_count;
	public:
		MyCamera();//コンストラクタ
		MyCamera(float ArmLen);//コンストラクタ（初期のarmの長さ）
		virtual ~MyCamera();//デストラクタ
		//カメラの位置
		virtual void SetEye(const bsm::Vec3& Eye)override;
		virtual void SetEye(float x, float y, float z)override;
		//カメラの目標オブジェクトを取得
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
