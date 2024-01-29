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
		// 作成日 : 2023/7/12
		// 作成者 : 服部颯澄
		// 
		// 不定形のコンスタントバッファだよ☆
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
			/// 不定形コンスタントバッファを作成するよ☆
			/// </summary>
			/// <param name="dataTypeDescriptor"> データの並びを指定する配列だよ☆ </param>
			/// <param name="descriptorHeaps"> ディスクリプタヒープの生成先だよ☆ </param>
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

			std::vector<DataType> m_dataTypeDescriptor;					//	データの種類と並びを指定する管理データだよ☆
			std::vector<byte> m_constantBufferData;						//	定数バッファに転送するデータだよ☆
			unsigned int m_descriptorOffset;							//	ディスクリプタのオフセット値だよ☆
			Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer;	//	転送先の定数バッファだよ☆

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

		//	テンプレートの実装部だよ☆

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