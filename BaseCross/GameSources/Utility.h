#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief ディグリー角をラジアン角に変換
	@param (deg) ディグリー角
	@return ラジアン角に変換されたディグリー角
	*/
	float DegToRad(const float& deg);
	Vec2 DegToRad(const Vec2& deg);
	Vec3 DegToRad(const Vec3& deg);
	Vec4 DegToRad(const Vec4& deg);


	/*!
	@brief ラジアン角をディグリー角に変換
	@param (rad) ラジアン角
	@return ディグリー角に変換されたラジアン角
	*/
	float RadToDeg(const float& rad);
	Vec2 RadToDeg(const Vec2& rad);
	Vec3 RadToDeg(const Vec3& rad);
	Vec4 RadToDeg(const Vec4& rad);
}