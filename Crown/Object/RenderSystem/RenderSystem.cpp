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

//	ライブラリのリンクだよ☆
#ifdef _DEBUG
#include <pix3.h>
#pragma comment(lib,"WinPixEventRuntime.lib")
#endif // _DEBUG
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#ifdef _DEBUG
typedef void(WINAPI* BeginEventOnCommandList)(ID3D12GraphicsCommandList* commandList, UINT64 color, _In_ PCSTR formatString);
typedef void(WINAPI* EndEventOnCommandList)(ID3D12GraphicsCommandList* commandList);
typedef void(WINAPI* SetMarkerOnCommandList)(ID3D12GraphicsCommandList* commandList, UINT64 color, _In_ PCSTR formatString);

HMODULE module = LoadLibrary(L"WinPixEventRuntime.dll");

BeginEventOnCommandList pixBeginEventOnCommandList = (BeginEventOnCommandList)GetProcAddress(module, "PIXBeginEventOnCommandList");
EndEventOnCommandList   pixEndEventOnCommandList = (EndEventOnCommandList)GetProcAddress(module, "PIXEndEventOnCommandList");
SetMarkerOnCommandList  pixSetMarkerOnCommandList = (SetMarkerOnCommandList)GetProcAddress(module, "PIXSetMarkerOnCommandList");
#endif // _DEBUG

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
	ResourceUploader::DeleteInstance();
	Shader::DeleteInstance();
	DescriptorHeaps::DeleteDescriptorHeaps();
	Camera::DelteInstance();
}

void Crown::RenderObject::RenderSystem::Initialize()
{
	//	デバックレイヤーを起動するよ☆
	{
		#ifdef _DEBUG
		Microsoft::WRL::ComPtr<ID3D12Debug> debugLayer;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
		debugLayer->EnableDebugLayer();
		#endif // _DEBUG
	}
	//	m_deviceの作成を行うよ☆
	{
		D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;

		Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
		CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&dxgiFactory));	//	DXGIFactoryの作成だよ☆

		Microsoft::WRL::ComPtr<IDXGIAdapter> tmpAdapter;
		Microsoft::WRL::ComPtr<IDXGIAdapter> selectAdapter;
		//	GPUデバイスの全取得だよ☆
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
		//	Direct3Dデバイスの初期化だよ☆
		D3D12CreateDevice(selectAdapter.Get(), level, IID_PPV_ARGS(&m_device));		//	D3D12Deviceの作成だよ☆
	}

	DefaultRootSignature::GetRootSignature().Create(m_device.Get());
	m_commandList.Initialize(m_device.Get(), 10, 0);
#ifdef _DEBUG
	PIXBeginEvent(m_commandList.GetCommandQueue(), 0xff0000, "Initialize");
#endif // _DEBUG
	m_swapChain.Initialize(m_device.Get(), &m_renderTargetWindow, m_commandList.GetCommandQueue(), 3);
	ResourceUploader::CreateResourceUploader(m_device.Get(), m_commandList);
	DescriptorHeaps::CreateDescriptorHeaps(m_device.Get(), 100);
	m_textureBuffer.reset(new TextureBuffer(m_device.Get(),m_commandList.GetCopyCommandList(), &DescriptorHeaps::GetInstance()));
	m_modelManager.Initialize(m_device.Get(), m_textureBuffer.get());
	UiManager::GetInstance()->Initialize(m_device.Get(), m_textureBuffer.get());
	Camera::CreateInstance(m_device.Get());
#ifdef _DEBUG
	PIXEndEvent(m_commandList.GetCommandQueue());
#endif // _DEBUG
}

void Crown::RenderObject::RenderSystem::Finalize()
{
	m_commandList.WaitForGpu();
}

void Crown::RenderObject::RenderSystem::Update()
{
	m_swapChain.Present(0);
#ifdef _DEBUG
	PIXBeginEvent(m_commandList.GetCommandQueue(), PIX_COLOR(0,1,1), "DataCopy");
#endif // _DEBUG

	m_modelManager.DataCopy();
	m_commandList.CopyExecute();

	//	同じ優先度のレンダーターゲットをまとめてキューに送信するよ☆
	if(!m_renderTargets.empty())
	{
		int targetPriority = m_renderTargets.front().first;
		int commandListIndex = 0;
		for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTargets.size()); i < size;)
		{
#ifdef _DEBUG
			PIXEndEvent(m_commandList.GetCommandQueue());
			std::string text = "DrawRenderTargets : " + std::to_string(i);
			PIXBeginEvent(m_commandList.GetCommandQueue(), PIX_COLOR(0, 255, 255), text.c_str());
#endif // _DEBUG
			if (targetPriority == m_renderTargets[i].first)
			{
				ID3D12DescriptorHeap* descriptorHeap = DescriptorHeaps::GetInstance().GetDescriptorHeap();
				m_commandList.GetGraphicsCommandList(commandListIndex)->SetDescriptorHeaps(1, &descriptorHeap);

				if (m_renderTargets[i].second)
				{
					m_renderTargets[i].second->Draw(m_commandList, &m_modelManager, commandListIndex);
					++i;
				}
				else
				{
					m_renderTargets.erase(m_renderTargets.begin() + i);
				}
				++commandListIndex;
			}
			else
			{
				targetPriority = m_renderTargets[i].first;
				commandListIndex = 0;
				m_commandList.DrawExecute();
			}
		}
	}

	m_commandList.DrawExecute();

	ID3D12DescriptorHeap* descriptorHeap = DescriptorHeaps::GetInstance().GetDescriptorHeap();
	m_commandList.GetGraphicsCommandList(0)->SetDescriptorHeaps(1, &descriptorHeap);

#ifdef _DEBUG
	PIXEndEvent(m_commandList.GetCommandQueue());
	PIXBeginEvent(m_commandList.GetCommandQueue(), 0xff0000, "Update");
#endif // _DEBUG

	//	描画開始☆
	{
		CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(m_swapChain.GetBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);	//	リソースバリアをの設定をすべてに使用可能から描画先として使用に切り替えるよ☆
		m_commandList.GetGraphicsCommandList(0)->ResourceBarrier(1, &tmp);																									//	リソースバリアの設定変更を要求するよ☆
	}

	//	描画対象の決定☆
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = m_swapChain.GetRenderTargetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += static_cast<SIZE_T>(m_swapChain.GetBackBufferIndex()) * m_swapChain.GetRenderTargetViewSize();
	const D3D12_CPU_DESCRIPTOR_HANDLE CPU_DESCRIPTOR_HANDLE = m_swapChain.GetDepthStencilDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	//	画面を初期化☆
	m_commandList.GetGraphicsCommandList(0)->OMSetRenderTargets(1, &rtvH, false, &CPU_DESCRIPTOR_HANDLE);																	//	レンダーターゲットを指定するよ☆

	m_commandList.GetGraphicsCommandList(0)->ClearRenderTargetView(rtvH, CLEAR_COLOR, 0, nullptr);
	m_commandList.GetGraphicsCommandList(0)->ClearDepthStencilView(CPU_DESCRIPTOR_HANDLE, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	D3D12_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(m_renderTargetWindow.GetWindowWidth());
	viewport.Height = static_cast<float>(m_renderTargetWindow.GetWindowHeight());
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	D3D12_RECT scissorrect = {};
	scissorrect.top = 0;
	scissorrect.left = 0;
	scissorrect.right = static_cast<LONG>(m_renderTargetWindow.GetWindowWidth());
	scissorrect.bottom = static_cast<LONG>(m_renderTargetWindow.GetWindowHeight());

	m_commandList.GetGraphicsCommandList(0)->RSSetViewports(1, &viewport);
	m_commandList.GetGraphicsCommandList(0)->RSSetScissorRects(1, &scissorrect);

	//	描画☆
	//m_modelManager.Draw(MaterialTag::Normal,m_commandList);
	UiManager::GetInstance()->Render(m_commandList);

	//	描画終了～☆
	{
		CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(m_swapChain.GetBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);	//	リソースバリアをの設定をすべてに使用可能にするよ☆
		m_commandList.GetGraphicsCommandList(0)->ResourceBarrier(1, &tmp);
	}

	m_commandList.DrawExecute();
#ifdef _DEBUG
	PIXEndEvent(m_commandList.GetCommandQueue());
#endif // _DEBUG
	DescriptorHeaps::GetInstance().ResetDescriptorHeapFlag();
}

void Crown::RenderObject::RenderSystem::AddRenderTarget(int priority, std::shared_ptr<IRenderTarget> renderTarget)
{
	renderTarget->Initialize(m_device.Get());
	m_renderTargets.push_back(std::make_pair(priority, renderTarget));
	std::sort(m_renderTargets.begin(), m_renderTargets.end(), [](std::pair<int, std::shared_ptr<IRenderTarget>>& l, std::pair<int, std::shared_ptr<IRenderTarget>>& r) { return l.first < r.first; });
}