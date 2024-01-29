#pragma once
#ifndef CROWN_RENDER_OBJCET_GRAPHICSCOMMANDLIST
#define CROWN_RENDER_OBJCET_GRAPHICSCOMMANDLIST

#include <d3d12.h>
#include <vector>
#include "CommandAllocator.h"
#include <wrl.h>
#include <memory>
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// �쐬�� : 2023/6/21
		// �쐬�� : �����D��
		// 
		// �R�}���h���X�g�����b�v����N���X���恙
		// 
		//================================================
		class GraphicsCommandList
		{
		public:
			GraphicsCommandList();
			~GraphicsCommandList();

			/// <summary>
			/// �R�}���h���X�g��n���恙
			/// </summary>
			/// <returns></returns>
			inline ID3D12GraphicsCommandList* GetGraphicsCommandList() const noexcept { return m_graphicsCommandList.Get(); }

			/// <summary>
			/// ���\�[�X�̃R�s�[���̃R�}���h���X�g��n���恙
			/// </summary>
			/// <returns></returns>
			inline ID3D12GraphicsCommandList* GetCopyCommandList() const noexcept { return m_copyCommandList.Get(); }

			/// <summary>
			/// �R�}���h�L���[��n���恙
			/// </summary>
			/// <returns></returns>
			inline ID3D12CommandQueue* GetCommandQueue() const noexcept { return m_commandQueue.Get(); }

			/// <summary>
			/// ������������恙
			/// </summary>
			/// <param name="device">�R�}���h���X�g�̍쐬�Ɏg�p����ID3D12Device�̃|�C���^���恙</param>
			/// <param name="allocatorNum">�쐬����R�}���h�A���P�[�^�[�̐����恙�o���邾�����Ȃ��ق����������������ǂ����Ǐ��Ȃ�����Ə��������������Ȃ�恙</param>
			/// <param name="failedProcessingNum">����������������t���[�������恙</param>
			void Initialize(ID3D12Device* device, unsigned int allocatorNum = 2, unsigned int failedProcessingNum = 1);

			/// <summary>
			///	�`��Ɏg�p���̃��\�[�X������ĉ������Ȃ��悤�ɕێ����Ă����恙
			/// </summary>
			/// <param name="lockResource"></param>
			inline void LockResource(const Microsoft::WRL::ComPtr<ID3D12Resource>& lockResource) { m_commandAllocators[m_index]->LockResource(lockResource); }

			/// <summary>
			/// GPU���̑S�Ă̏������I������܂őҋ@����恙
			/// �R�}���h���X�g�̎��s�͂��Ȃ��恙
			/// </summary>
			void WaitForGpu() noexcept;

			/// <summary>
			/// �R�}���h���X�g�����s����恙
			/// </summary>
			void RunCommandList();

			/// <summary>
			/// �R�}���h���X�g�����s���A��������������܂őҋ@����恙
			/// </summary>
			void RunAndWait();

		private:

			/// <summary>
			/// �R�}���h���X�g�ɐς񂾏����𑖂点��恙
			/// </summary>
			void Run();

			/// <summary>
			/// ���̃R�}���h�A���P�[�^�[���擾����恙
			/// </summary>
			void NextCommandAllocator();

			unsigned int													m_addCommandAllocatorNum;						//	���ݒǉ���������Ă���R�}���h�A���P�[�^�[�̐����恙
			const unsigned int												MAX_FAILEDPROCESSING_NUM;						//	���������������ꂽ�t���[�������恙
			unsigned int													m_failedProcessingNum;							//	���݂̏������������恙
			unsigned int													m_index;										//	���ݎg�p���̃R�}���h�A���P�[�^�[�̃C���f�b�N�X���恙
			unsigned int													m_copyIndex;									//	�R�s�[���̃R�}���h�A���P�[�^�[�̃C���f�b�N�X���恙
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>				m_graphicsCommandList;							//	�R�}���h���X�g���恙
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>				m_copyCommandList;								//	�R�s�[�p�R�}���h���X�g���恙
			Microsoft::WRL::ComPtr<ID3D12CommandQueue>						m_commandQueue;									//	�R�}���h�L���[���恙
			std::vector<std::unique_ptr<CommandAllocator>>					m_commandAllocators;							//	�R�}���h�A���P�[�^�[�̔z�񂾂恙
			std::vector<std::unique_ptr<CommandAllocator>>					m_copyAllocators;								//	�R�s�[�p�R�}���h�A���P�[�^�[�̔z�񂾂恙

			HANDLE															m_waitEvent;									//	�`���܂őҋ@����ׂ̃C�x���g���恙

			ID3D12Device*													m_device;
		};
	}
}
#endif // !CROWN_RENDER_OBJCET_GRAPHICSCOMMANDLIST