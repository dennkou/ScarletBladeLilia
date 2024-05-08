#include "RenderSystem.h"
#include <dxgi1_6.h>
#include <string>
#include "./../Window.h"
#include "DirectXTex.h"
#include "d3dx12.h"
#include "DirectX12Wraps/DescriptorHeaps.h"
#include "DirectX12Wraps/ResourceUploader.h"
#include "Shader.h"
#include "Camera.h"
#include "UI/UiManager.h"
#include <algorithm>
#include "DirectX12Wraps/DefaultRootSignature.h"

//	���C�u�����̃����N���恙
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

Crown::RenderObject::RenderSystem::RenderSystem(Crown::Window& renderTargetWindow)
	:
	m_device(nullptr),
	m_renderTargetWindow(renderTargetWindow),
	m_swapChain(),
	m_commandList(),
	m_modelManager()
{
}

Crown::RenderObject::RenderSystem::~RenderSystem()
{
	m_commandList.Finalize();
	ResourceUploader::DeleteInstance();
	Shader::DeleteInstance();
	DescriptorHeaps::DeleteDescriptorHeaps();
	Camera::DelteInstance();
}

void Crown::RenderObject::RenderSystem::Initialize()
{
	//	�f�o�b�N���C���[���N������恙
	{
		#ifdef _DEBUG
		Microsoft::WRL::ComPtr<ID3D12Debug> debugLayer;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
		debugLayer->EnableDebugLayer();
		#endif // _DEBUG
	}
	//	m_device�̍쐬���s���恙
	{
		D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;

		Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
		CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&dxgiFactory));	//	DXGIFactory�̍쐬���恙

		Microsoft::WRL::ComPtr<IDXGIAdapter> tmpAdapter;
		Microsoft::WRL::ComPtr<IDXGIAdapter> selectAdapter;
		//	GPU�f�o�C�X�̑S�擾���恙
		for (int i = 0; dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC adesc = {};
			tmpAdapter->GetDesc(&adesc);

			std::wstring strDesc = adesc.Description;
			if (strDesc.find(L"NVIDIA") != std::string::npos || strDesc.find(L"AMD") != std::string::npos)
			{
				selectAdapter = tmpAdapter;
				break;
			}
		}
		//	Direct3D�f�o�C�X�̏��������恙
		D3D12CreateDevice(selectAdapter.Get(), level, IID_PPV_ARGS(&m_device));		//	D3D12Device�̍쐬���恙
	}
	DefaultRootSignature::GetRootSignature().Create(m_device.Get());
	m_commandList.Initialize(m_device.Get(), 2, 0);
	m_swapChain.Initialize(m_device.Get(), &m_renderTargetWindow, m_commandList.GetCommandQueue(), 2);
	ResourceUploader::CreateResourceUploader(m_device.Get(), m_commandList);
	DescriptorHeaps::CreateDescriptorHeaps(m_device.Get(), 100);
	m_textureBuffer.Initialize(m_device.Get(),m_commandList.GetCopyCommandList(), &DescriptorHeaps::GetInstance());
	m_modelManager.Initialize(m_device.Get(), &m_textureBuffer);
	UiManager::GetInstance()->Initialize(m_device.Get(), &m_textureBuffer);
	Camera::CreateInstance(m_device.Get());
}

void Crown::RenderObject::RenderSystem::Finalize()
{
	m_commandList.WaitForGpu();
}

void Crown::RenderObject::RenderSystem::Update()
{
	m_commandList.Begin();

	ID3D12DescriptorHeap* descriptorHeap = DescriptorHeaps::GetInstance().GetDescriptorHeap();
	m_commandList.GetGraphicsCommandList()->SetDescriptorHeaps(1, &descriptorHeap);

	for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTargets.size()); i < size;)
	{	
		std::shared_ptr<RenderTarget> renderTarget = m_renderTargets[i].second.lock();
		if(renderTarget)
		{
			renderTarget->Draw(m_commandList, &m_modelManager);
			++i;
		}
		else
		{
			m_renderTargets.erase(m_renderTargets.begin() + i);
		}
	}

	//	�`��J�n��
	{
		CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(m_swapChain.GetBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);	//	���\�[�X�o���A���̐ݒ�����ׂĂɎg�p�\����`���Ƃ��Ďg�p�ɐ؂�ւ���恙
		m_commandList.GetGraphicsCommandList()->ResourceBarrier(1, &tmp);																									//	���\�[�X�o���A�̐ݒ�ύX��v������恙
	}

	//	�`��Ώۂ̌��聙
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = m_swapChain.GetRenderTargetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += static_cast<SIZE_T>(m_swapChain.GetBackBufferIndex()) * m_swapChain.GetRenderTargetViewSize();
	const D3D12_CPU_DESCRIPTOR_HANDLE CPU_DESCRIPTOR_HANDLE = m_swapChain.GetDepthStencilDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	//	��ʂ���������
	m_commandList.GetGraphicsCommandList()->ClearRenderTargetView(rtvH, CLEAR_COLOR, 0, nullptr);
	m_commandList.GetGraphicsCommandList()->ClearDepthStencilView(CPU_DESCRIPTOR_HANDLE, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	m_commandList.GetGraphicsCommandList()->OMSetRenderTargets(1, &rtvH, false, &CPU_DESCRIPTOR_HANDLE);																	//	�����_�[�^�[�Q�b�g���w�肷��恙

	D3D12_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(m_renderTargetWindow.GetWindowWidth() * 2);
	viewport.Height = static_cast<float>(m_renderTargetWindow.GetWindowHeight() * 2);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	D3D12_RECT scissorrect = {};
	scissorrect.top = 0;
	scissorrect.left = 0;
	scissorrect.right = static_cast<LONG>(m_renderTargetWindow.GetWindowWidth() * 2);
	scissorrect.bottom = static_cast<LONG>(m_renderTargetWindow.GetWindowHeight() * 2);

	m_commandList.GetGraphicsCommandList()->RSSetViewports(1, &viewport);
	m_commandList.GetGraphicsCommandList()->RSSetScissorRects(1, &scissorrect);

	//	�`�恙
	m_modelManager.Draw(MaterialTag::Normal,m_commandList);
	UiManager::GetInstance()->Render(m_commandList);

	//	�`��I���`��
	{
		CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(m_swapChain.GetBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);	//	���\�[�X�o���A���̐ݒ�����ׂĂɎg�p�\�ɂ���恙
		m_commandList.GetGraphicsCommandList()->ResourceBarrier(1, &tmp);
	}

	m_commandList.End();
	m_swapChain.Present(0);

	DescriptorHeaps::GetInstance().ResetDescriptorHeapFlag();
}

void Crown::RenderObject::RenderSystem::AddRenderTarget(int priority, std::weak_ptr<RenderTarget> renderTarget)
{
	renderTarget.lock()->Initialize(m_device.Get());
	m_renderTargets.push_back(std::make_pair(priority, renderTarget));
	std::sort(m_renderTargets.begin(), m_renderTargets.end(), [](std::pair<int, std::weak_ptr<RenderTarget>>& l, std::pair<int, std::weak_ptr<RenderTarget>>& r) { return l.first < r.first; });
}