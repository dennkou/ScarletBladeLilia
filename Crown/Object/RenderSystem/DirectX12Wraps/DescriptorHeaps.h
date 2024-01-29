#pragma once
#ifndef CROWN_RENDEROBJECT_DESCRIPTORHEAPS
#define CROWN_RENDEROBJECT_DESCRIPTORHEAPS
#include <d3d12.h>
#include <wrl.h>
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// �쐬�� : 2023/6/28
		// �쐬�� : �����D��
		// 
		// �f�B�X�N���v�^�q�[�v�̊Ǘ����s���N���X���恙
		// �ǉ��͉\�����Ǎ폜�͑z�肵�ĂȂ��恙
		// 
		//================================================
		class DescriptorHeaps
		{
		public:

			static void CreateDescriptorHeaps(ID3D12Device* device, unsigned int heapsSize = 65535);
			static DescriptorHeaps& GetInstance();
			static void DeleteDescriptorHeaps();

			/// <summary>
			/// �Ǘ����Ă���f�B�X�N���v�^�q�[�v�ɃR���X�^���g�o�b�t�@�r���[��ǉ�����恙
			/// </summary>
			/// <param name="constantBufferViewDesc">�쐬����R���X�^���g�o�b�t�@�r���[��</param>
			/// <returns>�f�B�X�N���v�^�̃I�t�Z�b�g�ʒu��</returns>
			unsigned int CreateConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& constantBufferViewDesc);

			/// <summary>
			/// �Ǘ����Ă���f�B�X�N���v�^�q�[�v�ɃV�F�[�_�[���\�[�X�r���[��ǉ�����恙
			/// </summary>
			/// <param name="resource">�V�F�[�_�[���\�[�X�r���[���Q�Ƃ��郊�\�[�X��</param>
			/// <param name="shaderResourceViewDesc">�쐬����V�F�[�_�[���\�[�X�r���[��</param>
			/// <returns>�f�B�X�N���v�^�̃I�t�Z�b�g�ʒu��</returns>
			unsigned int CreateShaderResourceView(ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC& shaderResourceViewDesc);

			/// <summary>
			/// �w��ʒu��GPU�n���h�����擾����恙
			/// </summary>
			/// <returns></returns>
			D3D12_GPU_DESCRIPTOR_HANDLE GetHandle(unsigned int offset) const;

			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			bool GetResetDescriptorHeapFlag() const noexcept { return m_changeSize; }

			void ResetDescriptorHeapFlag()noexcept { m_changeSize = false; }

			inline ID3D12DescriptorHeap* GetDescriptorHeap() const { return m_descriptorHeap[m_index].Get(); }

		private:
			DescriptorHeaps();
			~DescriptorHeaps();
			DescriptorHeaps(DescriptorHeaps& DescriptorHeaps) = delete;
			DescriptorHeaps& operator=(DescriptorHeaps& DescriptorHeaps) = delete;
			static DescriptorHeaps* me;

			/// <summary>
			/// �V�����f�B�X�N���v�^�p�̃q�[�v�̈���m�ۂ��āA�m�ۂ����ʒu��Ԃ��恙
			/// </summary>
			/// <returns></returns>
			D3D12_CPU_DESCRIPTOR_HANDLE CreateDescriptor();

			/// <summary>
			/// �f�B�X�N���v�^�q�[�v�𐶐�����恙
			/// </summary>
			/// <param name="flags"></param>
			/// <param name="NumDescriptors"></param>
			/// <param name="descriptorHeap"></param>
			void CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_FLAGS flags,UINT NumDescriptors,ID3D12DescriptorHeap** descriptorHeap);

			/// <summary>
			/// CPU�p�f�B�X�N���v�^�q�[�v��GPU�p�̃f�B�X�N���v�^�q�[�v�ɃA�b�v���[�h����恙
			/// </summary>
			void UploadDescriptorHeap();



			ID3D12Device* m_device;												//	�q�[�v�̍쐬�Ɏg�p����f�o�C�X���恙
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_cpuDescriptorHeap;	//	�V�F�[�_�[������F������Ȃ��f�B�X�N���v�^�q�[�v���恙
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap[2];	//	�V�F�[�_�[������F�������f�B�X�N���v�^�q�[�v���恙
			const UINT DESCRIPTOR_HANDLE_INCREMENT_SIZE;						//	�f�B�X�N���v�^�̃T�C�Y���恙
			unsigned int m_index;												//	���ݎg�p���Ă���f�B�X�N���v�^�q�[�v���恙
			unsigned int m_descriptorNum;										//	���݂̃f�B�X�N���v�^�����恙
			unsigned int m_maxDescriptorNum;									//	�f�B�X�N���v�^�̍ő吔���恙
			bool m_changeSize;													//	�ő�T�C�Y��ύX�����̃t���O���恙
		};
	}
}
#endif // !CROWN_RENDEROBJECT_DESCRIPTORHEAPS