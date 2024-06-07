#pragma once
#ifndef CROWN_RENDEROBJECT_COMMANDALLOCATOR
#define CROWN_RENDEROBJECT_COMMANDALLOCATOR
#include <d3d12.h>
#include <wrl.h>
#include <vector>
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
		class CommandAllocator
		{
		public:
			CommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type);
			~CommandAllocator();

			/// <summary>
			/// GUP���̏������I��������m�F����恙
			/// </summary>
			/// <returns></returns>
			bool NotDrewEnd();

			/// <summary>
			/// GPU���̏������I��܂őҋ@����恙
			/// </summary>
			/// <param name="waitEvent"></param>
			void WaitForGpu() noexcept;

			/// <summary>
			///	�`��Ɏg�p���̃��\�[�X������ĉ������Ȃ��悤�ɕێ����Ă����恙
			/// </summary>
			/// <param name="lockResource"></param>
			inline void LockResource(const Microsoft::WRL::ComPtr<ID3D12Resource>& lockResource) { m_resourceHolder.push_back(lockResource); }

			void Reset();

			inline ID3D12CommandAllocator* GetCommandAllocator() const noexcept { return m_commandAllocator.Get(); }
			inline ID3D12Fence* GetFence() const noexcept { return m_fence.Get(); }
			inline UINT64 GetFenceValue() const noexcept { return m_fenceValue; }
			void CountUp() noexcept { ++m_fenceValue; }
		private:
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		m_commandAllocator;
			Microsoft::WRL::ComPtr<ID3D12Fence>					m_fence;
			UINT64												m_fenceValue;
			std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>	m_resourceHolder;		//	�`�撆�Ɍ���ĉ������Ȃ��悤�ɃV�F�A�|�C���^��ێ����Ă����恙
		};
	}
}
#endif // !CROWN_RENDEROBJECT_COMMANDALLOCATOR