#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief �f�B�O���[�p�����W�A���p�ɕϊ�
	@param (deg) �f�B�O���[�p
	@return ���W�A���p�ɕϊ����ꂽ�f�B�O���[�p
	*/
	float DegToRad(const float& deg);
	Vec2 DegToRad(const Vec2& deg);
	Vec3 DegToRad(const Vec3& deg);
	Vec4 DegToRad(const Vec4& deg);


	/*!
	@brief ���W�A���p���f�B�O���[�p�ɕϊ�
	@param (rad) ���W�A���p
	@return �f�B�O���[�p�ɕϊ����ꂽ���W�A���p
	*/
	float RadToDeg(const float& rad);
	Vec2 RadToDeg(const Vec2& rad);
	Vec3 RadToDeg(const Vec3& rad);
	Vec4 RadToDeg(const Vec4& rad);
}