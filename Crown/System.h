#pragma once
#ifndef CROWN_SYSTEM
#define CROWN_SYSTEM

#include "Object/Window.h"
#include "Object/RenderSystem/RenderSystem.h"

namespace Crown
{
	//================================================================================
	// �쐬�� : 2023/6/27
	// �쐬�� : �����D��
	// 
	// ���색�C�u�����B�S�Ă̊Ǘ��ƃA�v���P�[�V�����ւ̋��n�����s���恙
	// 
	//================================================================================
	class System final
	{
	public:
		~System();

		/// <summary>
		/// system�N���X�ւ̗B��̃|�C���^��Ԃ��恙
		/// </summary>
		/// <returns></returns>
		static System& GetInstance();

		/// <summary>
		/// �G���W���S�̂̏��������s���恙
		/// </summary>
		void Initialize();

		/// <summary>
		/// �G���W���S�̂̍X�V���s���恙
		/// ����I�ɌĂяo���K�v������恙
		/// </summary>
		void Update();

		/// <summary>
		/// �G���W���S�̂̏I�������𑖂点��恙
		/// </summary>
		void Finalize();

		/// <summary>
		/// Crown�S�̂��I�����悤�Ƃ��Ă���̂��������t���O����������恙
		/// </summary>
		/// <returns>true�ŏI�����悤�Ƃ��Ă���</returns>
		inline bool GetEndFlag() const { return m_endFlag; }

		/// <summary>
		/// �`��G���W����n���恙
		/// </summary>
		/// <returns></returns>
		inline RenderObject::RenderSystem& GetRenderSystem() { return m_renderSystem; }

		/// <summary>
		/// �E�B���h�E�n���h�����擾����恙
		/// </summary>
		inline HWND GetWindow() { return m_window.GetWindowHandle(); }

		/// <summary>
		/// �E�B���h�E�̉������擾����恙
		/// </summary>
		/// <returns></returns>
		inline int GetWindowSizeX() { return m_window.GetWindowWidth(); }

		/// <summary>
		/// �E�B���h�E�̏c�����擾����恙
		/// </summary>
		/// <returns></returns>
		inline int GetWindowSizeY() { return m_window.GetWindowHeight(); }
	private:
		System();
		System(System& system) = delete;
		System& operator=(System& crown) = delete;

		static std::unique_ptr<System> m_system;

		Window m_window;
		RenderObject::RenderSystem m_renderSystem;
		bool m_endFlag;
	};
}
#endif	//	CROWN