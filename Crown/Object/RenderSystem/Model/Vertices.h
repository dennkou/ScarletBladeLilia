#pragma once
#ifndef CROWN_RENDEROBJECT_VERTICES
#define CROWN_RENDEROBJECT_VERTICES
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include "./../DirectX12Wraps/ResourceUploader.h"
#include "d3dx12.h"
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// 作成日 : 2023/6/28
		// 作成者 : 服部颯澄
		// 
		// 頂点データを扱うクラスだよ☆
		// 
		//================================================
		class Vertices
		{
		public:
			Vertices();
			~Vertices();

			template<class VerticesData>
			void CreateBuffer(ID3D12Device* device, std::vector<VerticesData>& vertices, std::vector<unsigned int>& indexBuffer)
			{
				//	頂点バッファーの作成☆
				D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
				D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ResourceUploader::GetInstance()->Get255AlignmentSize<VerticesData>(static_cast<unsigned int>(vertices.size())));
				device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&m_vertexBuffer));
				ResourceUploader::GetInstance()->UploadData<VerticesData>(m_vertexBuffer, ResourceUploader::GetInstance()->Get255AlignmentSize<VerticesData>(static_cast<unsigned int>(vertices.size())), [&](VerticesData* map)
					{
						std::copy(vertices.begin(), vertices.end(), map);
					});
				m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
				m_vertexBufferView.SizeInBytes = static_cast<UINT>(ResourceUploader::GetInstance()->Get255AlignmentSize<VerticesData>(static_cast<unsigned int>(vertices.size())));
				m_vertexBufferView.StrideInBytes = sizeof(VerticesData);

				//	インデックスバッファーの作成☆
				heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
				resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ResourceUploader::GetInstance()->Get255AlignmentSize<unsigned int>(static_cast<unsigned int>(indexBuffer.size())));
				device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&m_indexBuffer));
				ResourceUploader::GetInstance()->UploadData<unsigned int>(m_indexBuffer, ResourceUploader::GetInstance()->Get255AlignmentSize<unsigned int>(static_cast<unsigned int>(indexBuffer.size())), [&](unsigned int* map)
					{
						std::copy(indexBuffer.begin(), indexBuffer.end(), map);
					});
				m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
				m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
				m_indexBufferView.SizeInBytes = static_cast<UINT>(ResourceUploader::GetInstance()->Get255AlignmentSize<unsigned int>(static_cast<unsigned int>(indexBuffer.size())));

				m_vertexBuffer->SetName(L"vertexBuffer");
				m_indexBuffer->SetName(L"indexBuffer");
			}

			inline D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView()						{ return &m_vertexBufferView; }
			inline const Microsoft::WRL::ComPtr<ID3D12Resource> GetConstVertexBuffer()	{ return m_vertexBuffer; }
			inline D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView()						{ return &m_indexBufferView; }
			inline const Microsoft::WRL::ComPtr<ID3D12Resource> GetConstIndexBuffer()	{ return m_indexBuffer; }

		private:
			Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

			Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
			D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
		};
	}
}
#endif // !CROWN_RENDEROBJECT_VERTICES