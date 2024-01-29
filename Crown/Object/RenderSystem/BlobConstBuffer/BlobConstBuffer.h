#pragma once
#ifndef CROWN_RENDEROBJECT_BLOBCONSTBUFFER
#define CROWN_RENDEROBJECT_BLOBCONSTBUFFER
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include <memory>
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// �쐬�� : 2023/7/12
		// �쐬�� : �����D��
		// 
		// �s��`�̃R���X�^���g�o�b�t�@���恙
		// 
		//================================================
		class BlobConstBuffer
		{
		public:
			enum class DataType : unsigned char
			{
				Int = 0,
				Float = 1,
				Float2 = 2,
				Float3 = 3,
				Float4 = 4,
				Matrix = 5,

				Num
			};

			/// <summary>
			/// �s��`�R���X�^���g�o�b�t�@���쐬����恙
			/// </summary>
			/// <param name="dataTypeDescriptor"> �f�[�^�̕��т��w�肷��z�񂾂恙 </param>
			/// <param name="descriptorHeaps"> �f�B�X�N���v�^�q�[�v�̐����悾�恙 </param>
			/// <param name="device"></param>
			BlobConstBuffer(std::vector<DataType>& dataTypeDescriptor, ID3D12Device* device);
			~BlobConstBuffer();

			template<class Data>
			Data* GetParameter(unsigned int parameterIndex);

			template<class Data>
			void SetParameter(unsigned int parameterIndex, const Data& data);

			inline unsigned int GetDescriptorOffset() { return m_descriptorOffset; }
			inline const Microsoft::WRL::ComPtr<ID3D12Resource> GetBuffer() { return m_constantBuffer; }
		private:
			byte* GetDataPointer(unsigned int parameterIndex);
			void UploadConstantBuffer();

			std::vector<DataType> m_dataTypeDescriptor;					//	�f�[�^�̎�ނƕ��т��w�肷��Ǘ��f�[�^���恙
			std::vector<byte> m_constantBufferData;						//	�萔�o�b�t�@�ɓ]������f�[�^���恙
			unsigned int m_descriptorOffset;							//	�f�B�X�N���v�^�̃I�t�Z�b�g�l���恙
			Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer;	//	�]����̒萔�o�b�t�@���恙

			static constexpr size_t DATA_SIZE[static_cast<unsigned char>(DataType::Num)] = 
			{
				sizeof(int),							//	Int,
				sizeof(float),							//	Float,
				sizeof(float) << 1,						//	Float2,
				(sizeof(float) << 1) + sizeof(float),	//	Float3,
				sizeof(float) << 2,						//	Float4,
				sizeof(float) << 4						//	Matrix,
			};
			
		};

		//	�e���v���[�g�̎��������恙

		template<class Data>
		inline Data* BlobConstBuffer::GetParameter(unsigned int parameterIndex)
		{
			if (DATA_SIZE[static_cast<unsigned int>(m_dataTypeDescriptor[parameterIndex])] == sizeof(Data))
			{
				return reinterpret_cast<Data*>(GetDataPointer(parameterIndex));
			}
			else
			{
				return nullptr;
			}
		}

		template<class Data>
		inline void BlobConstBuffer::SetParameter(unsigned int parameterIndex, const Data& data)
		{
			if (DATA_SIZE[static_cast<unsigned int>(m_dataTypeDescriptor[parameterIndex])] == sizeof(Data))
			{
				(*reinterpret_cast<Data*>(GetDataPointer(parameterIndex))) = data;
			}
			UploadConstantBuffer();
		}
	}
}
#endif // !CROWN_RENDEROBJECT_BLOBCONSTBUFFER