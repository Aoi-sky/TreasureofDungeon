/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{

	public:

		void CreateResourses();

		void RoadTexture(const wstring& UseTextureName, const wstring& TextureDataName);//�e�N�X�`��
		//void RoadSound(const wstring& soundName, const wstring& soundDataName);//�T�E���h
		void RoadStaticModelMesh(const wstring& staticModelMeshName, const wstring& staticModelbmfName);//�ÓI���f��
		void RoadBoneModel(const wstring& boneModelbmfName, const wstring& boneModelMeshName, const wstring& boneModelMeshTangentName);//���I���f��


		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g�擾
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
	};

}

//end basecross
