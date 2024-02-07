#include "stdafx.h"
#include "Utility.h"

namespace basecross
{
	/*!
	@brief �f�B�O���[�p�����W�A���p�ɕϊ�
	@param (deg) �f�B�O���[�p
	@return ���W�A���p�ɕϊ����ꂽ�f�B�O���[�p
	*/
	float DegToRad(const float& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	Vec2 DegToRad(const Vec2& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	Vec3 DegToRad(const Vec3& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	Vec4 DegToRad(const Vec4& deg)
	{
		return deg * (XM_PI / 180.0f);
	}

	/*!
	@brief ���W�A���p���f�B�O���[�p�ɕϊ�
	@param (rad) ���W�A���p
	@return �f�B�O���[�p�ɕϊ����ꂽ���W�A���p
	*/
	float RadToDeg(const float& rad)
	{
		return rad * (180.0f / XM_PI);
	}

	Vec2 RadToDeg(const Vec2& rad)
	{
		return rad * (180.0f / XM_PI);
	}

	Vec3 RadToDeg(const Vec3& rad)
	{
		return rad * (180.0f / XM_PI);
	}

	Vec4 RadToDeg(const Vec4& rad)
	{
		return rad * (180.0f / XM_PI);
	}
}