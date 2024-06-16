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
	//	�����_�[�^�[�Q�b�g�r���[�̃q�[�v���쐬��
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = static_cast<unsigned int>(m_renderTarget.size());
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_rtvHeaps));

	//�[�x�̂��߂̃f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = static_cast<unsigned int>(m_renderTarget.size());
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));

	//	�����_�[�^�[�Q�b�g�r���[�A�V�F�[�_�[���\�[�X�r���[�A�f�v�X�X�e���V���r���[���쐬����恙
	for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTarget.size()); i < size; ++i)
	{
		CreateRenderTarget(device, m_renderTarget[i], i);
		CreatDepthBuffer(device, m_renderTarget[i], i);
	}
}

void Crown::RenderObject::MultipleRenderTarget::Draw(GraphicsCommandList& commandList, ModelManager* modelManager, unsigned int index)
{
	//	�`��J�n��
	{
		D3D12_RESOURCE_BARRIER resourceBarrier[MAX_RENDER_TARGET] = {};
		for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTarget.size()); i < size; ++i)
		{
			resourceBarrier[i] = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTarget[i].rtvResource.Get(), D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
			
		}
		commandList.GetGraphicsCommandList(index)->ResourceBarrier(static_cast<unsigned int>(m_renderTarget.size()), resourceBarrier);
	}

	//	�`��Ώۂ̌��聙
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeap = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHeap = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList.GetGraphicsCommandList(index)->OMSetRenderTargets(static_cast<unsigned int>(m_renderTarget.size()), &rtvHeap, true, &dsvHeap);

	//	��ʂ���������
	for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTarget.size()); i < size; ++i)
	{
		commandList.GetGraphicsCommandList(index)->ClearRenderTargetView(rtvHeap, &m_renderTarget[i].clearColor.x, 0, nullptr);
		commandList.GetGraphicsCommandList(index)->ClearDepthStencilView(dsvHeap, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		rtvHeap.ptr += RTVDescriptorHandleIncrementSize;
		dsvHeap.ptr += DSVDescriptorHandleIncrementSize;
	}

	//	�r���[�|�[�g��ݒ肷��恙
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

	//	�V�U�[��`��ݒ肷��恙
	D3D12_RECT scissorrect[MAX_RENDER_TARGET] = {};
	for (unsigned int i = 0, size = static_cast<unsigned int>(m_renderTarget.size()); i < size; ++i)
	{
		scissorrect[i].top = 0;
		scissorrect[i].left = 0;
		scissorrect[i].right = static_cast<unsigned int>(m_width);
		scissorrect[i].bottom = static_cast<unsigned int>(m_height);
	}
	commandList.GetGraphicsCommandList(index)->RSSetScissorRects(static_cast<unsigned int>(m_renderTarget.size()), scissorrect);

	//	�`�恙
	modelManager->Draw(m_materialTag, commandList, index);

	//	�`��I���`��
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

	//	�����_�[�^�[�Q�b�g�p�̃��\�[�X���쐬����恙
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



	//	�����_�[�^�[�Q�b�g�r���[�ݒ�
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

	//	�f�v�X�p�̃q�[�v�v���p�e�B���恙
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;																	//	DEFAULT��������UNKNOWN�ł悵��
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;													//	2�����̃e�N�X�`���f�[�^�Ƃ��Ĉ����恙
	depthResDesc.Width = static_cast<UINT64>(m_width);																//	���ƍ����̓����_�[�^�[�Q�b�g�Ɠ������恙
	depthResDesc.Height = m_height;																					//	��ɓ������恙
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
		IID_PPV_ARGS(&renderTarget.depthResource));

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH;
	dsvH = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	dsvH.ptr += index * DSVDescriptorHandleIncrementSize;

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;																			//	�f�v�X�l��32bit�g�p����恙
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;															//	2D�e�N�X�`�����恙
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;																			//	�t���O�͓��ɖ����恙
	device->CreateDepthStencilView(renderTarget.depthResource.Get(), &dsvDesc, dsvH);								//	

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	renderTarget.depth = DescriptorHeaps::GetInstance().CreateShaderResourceView(renderTarget.depthResource.Get(), srvDesc);
}