#include "RenderTarget.h"
#include "./../../System.h"
#include "DirectX12Wraps/DescriptorHeaps.h"

Crown::RenderObject::RenderTarget::RenderTarget(MaterialTag materialTag, unsigned int x, unsigned int y, DirectX::XMFLOAT4 clearColor, DXGI_FORMAT format)
	:
	CLEAR_COLOR(clearColor),
	m_materialTag(materialTag),
	m_xSize(x),
	m_ySize(y),
	m_texture(0),
	m_depth(0),
	m_format(format)
{
}

Crown::RenderObject::RenderTarget::~RenderTarget()
{

}

void Crown::RenderObject::RenderTarget::Initialize(ID3D12Device* device)
{
	CreateRenderTargetView(device, m_xSize, m_ySize);
	CreatDepthBuffer(device, m_xSize, m_ySize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = m_format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	m_texture = DescriptorHeaps::GetInstance().CreateShaderResourceView(m_rtvResource.Get(), srvDesc);

	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	m_depth = DescriptorHeaps::GetInstance().CreateShaderResourceView(m_depthResource.Get(), srvDesc);
}

void Crown::RenderObject::RenderTarget::Draw(GraphicsCommandList& commandList, ModelManager* modelManager)
{
		//	描画開始☆
	{
		CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(m_rtvResource.Get(), D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);	//	リソースバリアをの設定をすべてに使用可能から描画先として使用に切り替えるよ☆
		commandList.GetGraphicsCommandList()->ResourceBarrier(1, &tmp);																											//	リソースバリアの設定変更を要求するよ☆
	}

	//	描画対象の決定☆
	D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList.GetGraphicsCommandList()->OMSetRenderTargets(1, &rtv, false, &dsv);																								//	レンダーターゲットを指定するよ☆

	//	画面を初期化☆
	commandList.GetGraphicsCommandList()->ClearRenderTargetView(m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(), &CLEAR_COLOR.x, 0, nullptr);
	commandList.GetGraphicsCommandList()->ClearDepthStencilView(m_dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	D3D12_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(m_xSize);
	viewport.Height = static_cast<float>(m_ySize);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	D3D12_RECT scissorrect = {};
	scissorrect.top = 0;
	scissorrect.left = 0;
	scissorrect.right = m_xSize;
	scissorrect.bottom = m_ySize;

	commandList.GetGraphicsCommandList()->RSSetViewports(1, &viewport);
	commandList.GetGraphicsCommandList()->RSSetScissorRects(1, &scissorrect);

	//	描画☆
	modelManager->Draw(m_materialTag, commandList);

	//	描画終了～☆
	{
		CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(m_rtvResource.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);	//	リソースバリアをの設定をすべてに使用可能にするよ☆
		commandList.GetGraphicsCommandList()->ResourceBarrier(1, &tmp);
	}
}

unsigned int Crown::RenderObject::RenderTarget::GetTexture() const
{
	return m_texture;
}

unsigned int Crown::RenderObject::RenderTarget::GetDepth() const
{
	return m_depth;
}

inline void Crown::RenderObject::RenderTarget::CreateRenderTargetView(ID3D12Device* device, UINT width, UINT height)
{
	//	レンダーターゲットビューのヒープを作成☆
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_rtvHeaps));
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();

	//	レンダーターゲット用のリソースを作成するよ☆
	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(m_format, width, height);
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Color[0] = CLEAR_COLOR.x;
	clearValue.Color[1] = CLEAR_COLOR.y;
	clearValue.Color[2] = CLEAR_COLOR.z;
	clearValue.Color[3] = CLEAR_COLOR.w;
	clearValue.Format = m_format;
	device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&m_rtvResource));

	//	SRGBレンダーターゲットビュー設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = m_format;																	//	フォーマットの指定だよ☆
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;
	device->CreateRenderTargetView(m_rtvResource.Get(), &rtvDesc, rtvH);												//	レンダーターゲットビューの作成だよ☆
}

inline void Crown::RenderObject::RenderTarget::CreatDepthBuffer(ID3D12Device* device, UINT width, UINT height)
{
	//	デプス用のヒーププロパティだよ☆
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;																	//	DEFAULTだから後はUNKNOWNでよし☆
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;													//	2次元のテクスチャデータとして扱うよ☆
	depthResDesc.Width = static_cast<UINT64>(width);																//	幅と高さはレンダーターゲットと同じだよ☆
	depthResDesc.Height = height;																					//	上に同じだよ☆
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
		IID_PPV_ARGS(&m_depthResource));

	//深度のためのデスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;																					//	深度ビュー1つのみだよ☆
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;																//	デプスステンシルビューとして使うよ☆

	device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));											//	ディスクリプタヒープの作成だよ☆

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;																			//	デプス値に32bit使用するよ☆
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;															//	2Dテクスチャだよ☆
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;																			//	フラグは特に無いよ☆
	device->CreateDepthStencilView(m_depthResource.Get(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());//	
}
