#include "TextureBuffer.h"
#include <DirectXTex.h>
#include <d3dx12.h>



#pragma comment(lib,"DirectXTex.lib")



Crown::RenderObject::TextureBuffer::TextureBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* copyCommandList, DescriptorHeaps* descriptorHeap)
	:
	m_device(),
	m_descriptorHeap(),
	m_copyCommandList(),
	m_textureDescriptorHeapOffsets()
{
	m_device = device;
	m_descriptorHeap = descriptorHeap;
	m_copyCommandList = copyCommandList;
	RGBA white[2 * 2] = {};
	for (int i = 0; i < 2 * 2; ++i)
	{
		white[i].r = 255;
		white[i].g = 255;
		white[i].b = 255;
		white[i].a = 255;
	}
	CreateTextureData(L"白テクスチャ", 2, 2, white);
	RGBA black[2 * 2] = {};
	for (int i = 0; i < 2 * 2; ++i)
	{
		black[i].r = 0;
		black[i].g = 0;
		black[i].b = 0;
		black[i].a = 255;
	}
	CreateTextureData(L"黒テクスチャ", 2, 2, black);
	RGBA grayGradation[2 * 256] = {};
	for (unsigned int i = 0; i < 2 * 256; ++i)
	{
		grayGradation[i].r = static_cast<unsigned char>(i >> 1);
		grayGradation[i].g = static_cast<unsigned char>(i >> 1);
		grayGradation[i].b = static_cast<unsigned char>(i >> 1);
		grayGradation[i].a = 255;
	}
	CreateTextureData(L"グレイグラデーションテクスチャ", 2, 256, grayGradation);
}



Crown::RenderObject::TextureBuffer::~TextureBuffer()
{
}



unsigned int Crown::RenderObject::TextureBuffer::TextureAcquisition(std::wstring filePath)
{
	//	データがなかったら読み込みを試みるよ☆
	if (m_textureDescriptorHeapOffsets.find(filePath) == m_textureDescriptorHeapOffsets.end())
	{
		DirectX::ScratchImage scratchImage = {};
		if (DirectX::LoadFromWICFile(filePath.data(), DirectX::WIC_FLAGS_NONE, nullptr, scratchImage) == S_OK)
		{
			const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);
			CreateData(filePath, static_cast<UINT>(image->width), static_cast<UINT>(image->height), static_cast<UINT>(image->rowPitch), image->format, image->pixels);
		}
		else
		{
			//	読み込めなかったからエラーを返すよ☆
			assert(0);
			return 0;
		}
	}

    return m_textureDescriptorHeapOffsets[filePath];
}



const Microsoft::WRL::ComPtr<ID3D12Resource> Crown::RenderObject::TextureBuffer::GetTextureBuffer(unsigned int m_offset)
{
	assert(m_resources.find(m_offset) != m_resources.end());
	return m_resources[m_offset];
}



void Crown::RenderObject::TextureBuffer::CreateTextureData(std::wstring dataName, UINT width, UINT height, RGBA* data)
{
	CreateData(dataName,width,height,sizeof(RGBA) * width, DXGI_FORMAT_R8G8B8A8_UNORM, reinterpret_cast<uint8_t*>(data));
}



void Crown::RenderObject::TextureBuffer::CreateData(std::wstring& dataName, UINT width, UINT height, UINT rowPitch, DXGI_FORMAT format, uint8_t* pixels)
{
	//	作成対象のバッファーだよ☆
	Microsoft::WRL::ComPtr<ID3D12Resource> textureBuffer = nullptr;
	unsigned int m_offset = 0;

	//	255アライメントしたrowPitchだよ☆
	UINT dataPitchAlignment = rowPitch + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - rowPitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;

	//	アップロードバッファーの作成☆
	D3D12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC uploadResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(static_cast<UINT64>(dataPitchAlignment) * height);
	Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer = nullptr;
	m_device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &uploadResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuffer));
	m_uploadBuffer.emplace_back(uploadBuffer);

	//	アップロードバッファーにテクスチャデータを転送☆
	uint8_t* mapImage = nullptr;
	uploadBuffer->Map(0, nullptr, (void**)&mapImage);
	uint8_t* srcAddress = pixels;
	size_t pitch = dataPitchAlignment;
	for (unsigned int i = 0; i < height; ++i)
	{
		std::copy_n(srcAddress, rowPitch, mapImage);
		srcAddress += rowPitch;
		mapImage += pitch;
	}
	uploadBuffer->Unmap(0, nullptr);

	//	コピー用の設定だよ☆
	D3D12_TEXTURE_COPY_LOCATION src = {};
	src.pResource = uploadBuffer.Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint.Offset = 0;
	src.PlacedFootprint.Footprint.Width = width;
	src.PlacedFootprint.Footprint.Height = height;
	src.PlacedFootprint.Footprint.Depth = 1;
	src.PlacedFootprint.Footprint.RowPitch = dataPitchAlignment;
	src.PlacedFootprint.Footprint.Format = format;



	//	テクスチャデータの作成☆
	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
	m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&textureBuffer));

	//	コピー用の設定だよ☆
	D3D12_TEXTURE_COPY_LOCATION dst = {};
	dst.pResource = textureBuffer.Get();
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;

	//	バッファーのコピーだよ☆
	m_copyCommandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(textureBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	m_copyCommandList->ResourceBarrier(1, &barrier);

	//	SRVの作成を行うよ☆
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	m_offset = m_descriptorHeap->CreateShaderResourceView(textureBuffer.Get(), srvDesc);

	//	データの登録をするよ☆
	m_textureDescriptorHeapOffsets[dataName] = m_offset;
	m_resources[m_offset] = textureBuffer;
}