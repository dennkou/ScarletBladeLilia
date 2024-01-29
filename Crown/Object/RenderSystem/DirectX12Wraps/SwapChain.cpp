#include "SwapChain.h"
#include "./../../Window.h"


Crown::RenderObject::SwapChain::SwapChain()
	:
	m_backBufferCount(2),		//	�o�b�N�o�b�t�@�[(+ �����_�[�^�[�Q�b�g�r���[)�̐��͕K��2�ȏゾ�恙
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
	CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&dxgiFactory));											//	DXGIFactory�̍쐬���恙
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
	swapchainDesc.Width = width * 2;																					//	�����̓E�B���h�E�T�C�Y�ɍ��킹��恙�`�挋�ʂ����炩�ɂ���ׂɓ�{�ɂ��Ă���恙
	swapchainDesc.Height = height * 2;																					//	�c���̓E�B���h�E�T�C�Y�ɍ��킹��恙�`�挋�ʂ����炩�ɂ���ׂɓ�{�ɂ��Ă���恙
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;																	//	�t�H�[�}�b�g��8*4��RGBA���恙
	swapchainDesc.Stereo = false;																						//	�S��ʕ\���͂��Ȃ��恙
	swapchainDesc.SampleDesc.Count = 1;																					//	�}���`�T���v�����O�͂��Ȃ��恙
	swapchainDesc.SampleDesc.Quality = 0;																				//	�}���`�T���v�����O�͂��Ȃ��恙
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;																	//	���\�[�X�̓o�b�N�o�b�t�@�[���恙
	swapchainDesc.BufferCount = m_backBufferCount;																		//	�o�b�t�@�[�̐��̎w�肾�恙
	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;																		//	�E�B���h�E�T�C�Y�ɍ��킹��恙
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;															//	
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;																//	�����x�̓���͎w�肵�Ȃ��恙
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;														//	

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
	fullScreenDesc.Windowed = true;
	dxgiFactory->CreateSwapChainForHwnd(commandQueue, windowHandle, &swapchainDesc, &fullScreenDesc, nullptr, (IDXGISwapChain1**)m_swapchain.GetAddressOf());	//	�S��ʐݒ�Ȃ��ŃX���b�v�`�F�[���̍쐬���s���恙
	m_backBufferIndex = m_swapchain->GetCurrentBackBufferIndex();

}

inline void Crown::RenderObject::SwapChain::CreateRenderTargetView(ID3D12Device* device)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;																		//	�����_�[�^�[�Q�b�g�r���[�Ȃ̂œ��RRTV��
	heapDesc.NodeMask = 0;																								//	�A�_�v�^�[���̐ݒ肾�恙
	heapDesc.NumDescriptors = m_backBufferCount;																		//	�o�b�t�@�[�̐��̎w�肾�恙
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;																	//	�V�F�[�_�[���猩����K�v�͂Ȃ��恙

	//	�����_�[�^�[�Q�b�g�r���[�̃q�[�v���쐬��
	device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_rtvHeaps));

	//	SRGB�����_�[�^�[�Q�b�g�r���[�ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	const_cast<unsigned int&>(RTVSIZE) = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);		//	�����_�[�^�[�Q�b�g�r���[�̃T�C�Y���擾����恙

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();								//	�f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾��
	m_backBuffers.resize(m_backBufferCount);
	for (unsigned int i = 0; i < m_backBufferCount; ++i)
	{
		m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i]));														//	�X���b�v�`�F�[���ƕR������恙
		rtvDesc.Format = m_backBuffers[i]->GetDesc().Format;															//	�t�H�[�}�b�g�̎w�肾�恙
		device->CreateRenderTargetView(m_backBuffers[i].Get(), &rtvDesc, rtvH);											//	�����_�[�^�[�Q�b�g�r���[�̍쐬���恙
		rtvH.ptr += RTVSIZE;																							//	�n���h��������炷�恙
	}
}

inline void Crown::RenderObject::SwapChain::CreatDepthBuffer(ID3D12Device* device, UINT width, UINT height)
{
	//	�f�v�X�p�̃q�[�v�v���p�e�B���恙
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;																	//	DEFAULT��������UNKNOWN�ł悵��
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;													//	2�����̃e�N�X�`���f�[�^�Ƃ��Ĉ����恙
	depthResDesc.Width = static_cast<UINT64>(width) * 2;															//	���ƍ����̓����_�[�^�[�Q�b�g�Ɠ������恙
	depthResDesc.Height =height * 2;																				//	��ɓ������恙
	depthResDesc.DepthOrArraySize = 1;																				//	�e�N�X�`���z��ł��Ȃ���3D�e�N�X�`���ł��Ȃ��恙
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;																	//	�[�x�l�������ݗp�t�H�[�}�b�g���恙
	depthResDesc.SampleDesc.Count = 1;																				//	�T���v����1�s�N�Z��������1���恙
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;													//	���̃o�b�t�@�͐[�x�X�e���V���Ƃ��Ďg�p����恙
	depthResDesc.MipLevels = 1;																						//	�~�b�v�}�b�v�͎g��Ȃ��恙
	depthResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;																//	
	depthResDesc.Alignment = 0;																						//	

	//	�N���A�o�����[���恙
	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.0f;																		//	�[���P(�ő�l)�ŃN���A��
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;																	//	32bit�[�x�l�Ƃ��ăN���A��

	//	�f�v�X�X�e���V���r���[�̍쐬���恙
	device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&m_depthBuffer));

	//�[�x�̂��߂̃f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;																					//	�[�x�r���[1�݂̂��恙
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;																//	�f�v�X�X�e���V���r���[�Ƃ��Ďg���恙

	device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));											//	�f�B�X�N���v�^�q�[�v�̍쐬���恙

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;																			//	�f�v�X�l��32bit�g�p����恙
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;															//	2D�e�N�X�`�����恙
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;																			//	�t���O�͓��ɖ����恙
	device->CreateDepthStencilView(m_depthBuffer.Get(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());	//	
}