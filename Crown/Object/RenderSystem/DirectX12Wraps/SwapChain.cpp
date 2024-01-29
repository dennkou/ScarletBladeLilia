#include "SwapChain.h"
#include "./../../Window.h"


Crown::RenderObject::SwapChain::SwapChain()
	:
	m_backBufferCount(2),		//	バックバッファー(+ レンダーターゲットビュー)の数は必ず2以上だよ☆
	m_backBufferIndex(0),
	m_swapchain(nullptr),
	m_rtvHeaps(nullptr),
	m_dsvHeap(nullptr),
	RTVSIZE(0),
	m_depthBuffer(nullptr)
{
}

Crown::RenderObject::SwapChain::~SwapChain()
{
}

void Crown::RenderObject::SwapChain::Initialize(ID3D12Device* device, const Crown::Window* const window, ID3D12CommandQueue* commandQueue, unsigned int bufferNum)
{
	m_backBufferCount = bufferNum;
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
	CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&dxgiFactory));											//	DXGIFactoryの作成だよ☆
	CreateSwapChain(dxgiFactory.Get(), window->GetWindowWidth(), window->GetWindowHeight(), window->GetWindowHandle(), commandQueue);
	CreateRenderTargetView(device);
	CreatDepthBuffer(device, window->GetWindowWidth(), window->GetWindowHeight());
}

void Crown::RenderObject::SwapChain::Present(UINT syncInterval)
{
	m_swapchain->Present(syncInterval, 0);
	m_backBufferIndex = m_swapchain->GetCurrentBackBufferIndex();
}

inline void Crown::RenderObject::SwapChain::CreateSwapChain(IDXGIFactory2* dxgiFactory, UINT width, UINT height, HWND windowHandle, ID3D12CommandQueue* commandQueue)
{
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = width * 2;																					//	横幅はウィンドウサイズに合わせるよ☆描画結果を滑らかにする為に二倍にしているよ☆
	swapchainDesc.Height = height * 2;																					//	縦幅はウィンドウサイズに合わせるよ☆描画結果を滑らかにする為に二倍にしているよ☆
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;																	//	フォーマットは8*4のRGBAだよ☆
	swapchainDesc.Stereo = false;																						//	全画面表示はしないよ☆
	swapchainDesc.SampleDesc.Count = 1;																					//	マルチサンプリングはしないよ☆
	swapchainDesc.SampleDesc.Quality = 0;																				//	マルチサンプリングはしないよ☆
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;																	//	リソースはバックバッファーだよ☆
	swapchainDesc.BufferCount = m_backBufferCount;																		//	バッファーの数の指定だよ☆
	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;																		//	ウィンドウサイズに合わせるよ☆
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;															//	
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;																//	透明度の動作は指定しないよ☆
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;														//	

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
	fullScreenDesc.Windowed = true;
	dxgiFactory->CreateSwapChainForHwnd(commandQueue, windowHandle, &swapchainDesc, &fullScreenDesc, nullptr, (IDXGISwapChain1**)m_swapchain.GetAddressOf());	//	全画面設定なしでスワップチェーンの作成を行うよ☆
	m_backBufferIndex = m_swapchain->GetCurrentBackBufferIndex();

}

inline void Crown::RenderObject::SwapChain::CreateRenderTargetView(ID3D12Device* device)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;																		//	レンダーターゲットビューなので当然RTV☆
	heapDesc.NodeMask = 0;																								//	アダプター数の設定だよ☆
	heapDesc.NumDescriptors = m_backBufferCount;																		//	バッファーの数の指定だよ☆
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;																	//	シェーダーから見える必要はないよ☆

	//	レンダーターゲットビューのヒープを作成☆
	device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_rtvHeaps));

	//	SRGBレンダーターゲットビュー設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	const_cast<unsigned int&>(RTVSIZE) = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);		//	レンダーターゲットビューのサイズを取得するよ☆

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();								//	ディスクリプタヒープの先頭ハンドルを取得☆
	m_backBuffers.resize(m_backBufferCount);
	for (unsigned int i = 0; i < m_backBufferCount; ++i)
	{
		m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i]));														//	スワップチェーンと紐ずけるよ☆
		rtvDesc.Format = m_backBuffers[i]->GetDesc().Format;															//	フォーマットの指定だよ☆
		device->CreateRenderTargetView(m_backBuffers[i].Get(), &rtvDesc, rtvH);											//	レンダーターゲットビューの作成だよ☆
		rtvH.ptr += RTVSIZE;																							//	ハンドルを一つずらすよ☆
	}
}

inline void Crown::RenderObject::SwapChain::CreatDepthBuffer(ID3D12Device* device, UINT width, UINT height)
{
	//	デプス用のヒーププロパティだよ☆
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;																	//	DEFAULTだから後はUNKNOWNでよし☆
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;													//	2次元のテクスチャデータとして扱うよ☆
	depthResDesc.Width = static_cast<UINT64>(width) * 2;															//	幅と高さはレンダーターゲットと同じだよ☆
	depthResDesc.Height =height * 2;																				//	上に同じだよ☆
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
		IID_PPV_ARGS(&m_depthBuffer));

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
	device->CreateDepthStencilView(m_depthBuffer.Get(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());	//	
}