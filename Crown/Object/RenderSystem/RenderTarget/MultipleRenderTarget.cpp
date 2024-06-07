#include "MultipleRenderTarget.h"
#include "./../../../System.h"
#include "./../DirectX12Wraps/DescriptorHeaps.h"

Crown::RenderObject::MultipleRenderTarget::MultipleRenderTarget(MaterialTag materialTag, unsigned int width, unsigned int height, std::initializer_list<DirectX::XMFLOAT4> clearColors, DXGI_FORMAT format)
	:
	m_materialTag(materialTag),
	RTVDescriptorHandleIncrementSize(0),
	DSVDescriptorHandleIncrementSize(0)
{
	m_width = width;
	m_height = height;
	m_format = format;

	for (const DirectX::XMFLOAT4& clearColor : clearColors)
	{
		RenderTarget renderTarget;
		renderTarget.clearColor = clearColor;
		m_renderTarget.push_back(renderTarget);
	}
}

Crown::RenderObject::MultipleRenderTarget::~MultipleRenderTarget()
{

}

void Crown::RenderObject::MultipleRenderTarget::Initialize(ID3D12Device* device)
{
	//	レンダーターゲットビューのヒープを作成☆
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = static_cast<unsigned int>(m_renderTarget.size());
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_rtvHeaps));

	//深度のためのデスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = static_cast<unsigned int>(m_renderTarget.size());
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));

	//	レンダーターゲットビュー、シェーダーリソースビュー、デプスステンシルビューを作成するよ☆
	for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTarget.size()); i < size; ++i)
	{
		CreateRenderTarget(device, m_renderTarget[i], i);
		CreatDepthBuffer(device, m_renderTarget[i], i);
	}
}

void Crown::RenderObject::MultipleRenderTarget::Draw(GraphicsCommandList& commandList, ModelManager* modelManager, unsigned int index)
{
	//	描画開始☆
	{
		D3D12_RESOURCE_BARRIER resourceBarrier[MAX_RENDER_TARGET] = {};
		for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTarget.size()); i < size; ++i)
		{
			resourceBarrier[i] = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTarget[i].rtvResource.Get(), D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
			
		}
		commandList.GetGraphicsCommandList(index)->ResourceBarrier(static_cast<unsigned int>(m_renderTarget.size()), resourceBarrier);
	}

	//	描画対象の決定☆
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeap = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHeap = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList.GetGraphicsCommandList(index)->OMSetRenderTargets(static_cast<unsigned int>(m_renderTarget.size()), &rtvHeap, true, &dsvHeap);

	//	画面を初期化☆
	for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTarget.size()); i < size; ++i)
	{
		commandList.GetGraphicsCommandList(index)->ClearRenderTargetView(rtvHeap, &m_renderTarget[i].clearColor.x, 0, nullptr);
		commandList.GetGraphicsCommandList(index)->ClearDepthStencilView(dsvHeap, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		rtvHeap.ptr += RTVDescriptorHandleIncrementSize;
		dsvHeap.ptr += DSVDescriptorHandleIncrementSize;
	}

	//	ビューポートを設定するよ☆
	D3D12_VIEWPORT viewport[MAX_RENDER_TARGET] = {};
	for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTarget.size()); i < size; ++i)
	{
		viewport[i].Width = static_cast<float>(m_width);
		viewport[i].Height = static_cast<float>(m_height);
		viewport[i].TopLeftX = 0;
		viewport[i].TopLeftY = 0;
		viewport[i].MaxDepth = 1.0f;
		viewport[i].MinDepth = 0.0f;
	}
	commandList.GetGraphicsCommandList(index)->RSSetViewports(static_cast<unsigned int>(m_renderTarget.size()), viewport);

	//	シザー矩形を設定するよ☆
	D3D12_RECT scissorrect[MAX_RENDER_TARGET] = {};
	for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTarget.size()); i < size; ++i)
	{
		scissorrect[i].top = 0;
		scissorrect[i].left = 0;
		scissorrect[i].right = static_cast<unsigned int>(m_width);
		scissorrect[i].bottom = static_cast<unsigned int>(m_height);
	}
	commandList.GetGraphicsCommandList(index)->RSSetScissorRects(static_cast<unsigned int>(m_renderTarget.size()), scissorrect);

	//	描画☆
	modelManager->Draw(m_materialTag, commandList, index);

	//	描画終了～☆
	{
		D3D12_RESOURCE_BARRIER resourceBarrier[MAX_RENDER_TARGET] = {};
		for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTarget.size()); i < size; ++i)
		{
			resourceBarrier[i] = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTarget[i].rtvResource.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);
		}
		commandList.GetGraphicsCommandList(index)->ResourceBarrier(static_cast<unsigned int>(m_renderTarget.size()), resourceBarrier);
	}
}

inline void Crown::RenderObject::MultipleRenderTarget::CreateRenderTarget(ID3D12Device* device, RenderTarget& renderTarget, unsigned int index)
{
	RTVDescriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//	レンダーターゲット用のリソースを作成するよ☆
	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(m_format, m_width, m_height);
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Color[0] = renderTarget.clearColor.x;
	clearValue.Color[1] = renderTarget.clearColor.y;
	clearValue.Color[2] = renderTarget.clearColor.z;
	clearValue.Color[3] = renderTarget.clearColor.w;
	clearValue.Format = m_format;
	device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&renderTarget.rtvResource));



	//	レンダーターゲットビュー設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = m_format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += index * RTVDescriptorHandleIncrementSize;

	device->CreateRenderTargetView(renderTarget.rtvResource.Get(), &rtvDesc, rtvH);



	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = m_format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	renderTarget.texture = DescriptorHeaps::GetInstance().CreateShaderResourceView(renderTarget.rtvResource.Get(), srvDesc);
}

inline void Crown::RenderObject::MultipleRenderTarget::CreatDepthBuffer(ID3D12Device* device, RenderTarget& renderTarget, unsigned int index)
{
	DSVDescriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	//	デプス用のヒーププロパティだよ☆
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;																	//	DEFAULTだから後はUNKNOWNでよし☆
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;													//	2次元のテクスチャデータとして扱うよ☆
	depthResDesc.Width = static_cast<UINT64>(m_width);																//	幅と高さはレンダーターゲットと同じだよ☆
	depthResDesc.Height = m_height;																					//	上に同じだよ☆
	depthResDesc.DepthOrArraySize = 1;																				//	テクスチャ配列でもないし3Dテクスチャでもないよ☆
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;																	//	深度値書き込み用フォーマットだよ☆
	depthResDesc.SampleDesc.Count = 1;																				//	サンプルは1ピクセル当たり1つだよ☆
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;													//	このバッファは深度ステンシルとして使用するよ☆
	depthResDesc.MipLevels = 1;																						//	ミップマップは使わないよ☆
	depthResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;																//	
	depthResDesc.Alignment = 0;																						//	

	//	クリアバリューだよ☆
	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.0f;																		//	深さ１(最大値)でクリア☆
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;																	//	32bit深度値としてクリア☆

	//	デプスステンシルビューの作成だよ☆
	device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&renderTarget.depthResource));

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH;
	dsvH = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	dsvH.ptr += index * DSVDescriptorHandleIncrementSize;

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;																			//	デプス値に32bit使用するよ☆
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;															//	2Dテクスチャだよ☆
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;																			//	フラグは特に無いよ☆
	device->CreateDepthStencilView(renderTarget.depthResource.Get(), &dsvDesc, dsvH);								//	

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	renderTarget.depth = DescriptorHeaps::GetInstance().CreateShaderResourceView(renderTarget.depthResource.Get(), srvDesc);
}