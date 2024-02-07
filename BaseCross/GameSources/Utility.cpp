#include "stdafx.h"
#include "Utility.h"

namespace basecross
{
	/*!
	@brief ディグリー角をラジアン角に変換
	@param (deg) ディグリー角
	@return ラジアン角に変換されたディグリー角
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
	@brief ラジアン角をディグリー角に変換
	@param (rad) ラジアン角
	@return ディグリー角に変換されたラジアン角
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