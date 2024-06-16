#pragma once
#include "./../../Crown/Object/RenderSystem/RenderTarget/IRenderTarget.h"
#include "./../../Crown/Object/RenderSystem/BlobConstBuffer/BlobConstBuffer.h"
#include <vector>



class PostEffects : public Crown::RenderObject::IRenderTarget
{
public:
	PostEffects(unsigned int x, unsigned int y, DirectX::XMFLOAT4 clearColor, DXGI_FORMAT format, std::wstring vs, std::wstring ps, std::vector<Crown::RenderObject::BlobConstBuffer::DataType> constBuffer);
	virtual ~PostEffects();

	virtual void Initialize(ID3D12Device* device) override;
	virtual void Draw(Crown::RenderObject::GraphicsCommandList& commandList, Crown::RenderObject::ModelManager* modelManager, unsigned int index) override;
	virtual void SetTargetTexture(unsigned int targetTexture) { m_targetTexture = targetTexture; }
	inline Crown::RenderObject::BlobConstBuffer& GetConstBuffer() noexcept { return *m_constBuffer.get(); }
private:
	inline void CreateRenderTarget(ID3D12Device* device, UINT width, UINT height);		//	レンダーターゲットビューの作成だよ☆
	inline void CreatDepthBuffer(ID3D12Device* device, UINT width, UINT height);			//	深度、ステンシルビューの作成だよ☆

	static std::unique_ptr<Crown::RenderObject::Vertices> vertices;

	unsigned int m_xSize;
	unsigned int m_ySize;
	DirectX::XMFLOAT4 CLEAR_COLOR;
	unsigned int m_texture;
	unsigned int m_depth;
	unsigned int m_targetTexture;
	Crown::RenderObject::GraphicsPipeline m_graphicPipeline;

	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> m_constBufferDate;
	std::unique_ptr<Crown::RenderObject::BlobConstBuffer> m_constBuffer;



	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_rtvHeaps;			//	レンダーターゲットビューのヒープ領域だよ☆
	Microsoft::WRL::ComPtr<ID3D12Resource>			m_rtvResource;		//	レンダーターゲットヒープだよ☆バックバッファーと合わさっているよ☆
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_dsvHeap;			//	デプスステンシルヒープだよ☆
	Microsoft::WRL::ComPtr<ID3D12Resource>			m_depthResource;	//	デプスステンシルバッファーだよ☆
	DXGI_FORMAT m_format;

	static constexpr unsigned int INPUTLAYOUT_NUM = 2;
	static constexpr D3D12_INPUT_ELEMENT_DESC INPUTLAYOUT[INPUTLAYOUT_NUM] =
	{
		{ "POSITION",		0,DXGI_FORMAT_R32G32B32_FLOAT,		0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "UV",				0,DXGI_FORMAT_R32G32_FLOAT,			0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
	};
};