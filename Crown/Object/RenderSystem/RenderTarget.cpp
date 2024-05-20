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
		//	�`��J�n��
	{
		CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(m_rtvResource.Get(), D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);	//	���\�[�X�o���A���̐ݒ�����ׂĂɎg�p�\����`���Ƃ��Ďg�p�ɐ؂�ւ���恙
		commandList.GetGraphicsCommandList()->ResourceBarrier(1, &tmp);																											//	���\�[�X�o���A�̐ݒ�ύX��v������恙
	}

	//	�`��Ώۂ̌��聙
	D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList.GetGraphicsCommandList()->OMSetRenderTargets(1, &rtv, false, &dsv);																								//	�����_�[�^�[�Q�b�g���w�肷��恙

	//	��ʂ���������
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

	//	�`�恙
	modelManager->Draw(m_materialTag, commandList);

	//	�`��I���`��
	{
		CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(m_rtvResource.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);	//	���\�[�X�o���A���̐ݒ�����ׂĂɎg�p�\�ɂ���恙
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
	//	�����_�[�^�[�Q�b�g�r���[�̃q�[�v���쐬��
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_rtvHeaps));
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();

	//	�����_�[�^�[�Q�b�g�p�̃��\�[�X���쐬����恙
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

	//	SRGB�����_�[�^�[�Q�b�g�r���[�ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = m_format;																	//	�t�H�[�}�b�g�̎w�肾�恙
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;
	device->CreateRenderTargetView(m_rtvResource.Get(), &rtvDesc, rtvH);												//	�����_�[�^�[�Q�b�g�r���[�̍쐬���恙
}

inline void Crown::RenderObject::RenderTarget::CreatDepthBuffer(ID3D12Device* device, UINT width, UINT height)
{
	//	�f�v�X�p�̃q�[�v�v���p�e�B���恙
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;																	//	DEFAULT��������UNKNOWN�ł悵��
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;													//	2�����̃e�N�X�`���f�[�^�Ƃ��Ĉ����恙
	depthResDesc.Width = static_cast<UINT64>(width);																//	���ƍ����̓����_�[�^�[�Q�b�g�Ɠ������恙
	depthResDesc.Height = height;																					//	��ɓ������恙
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
		IID_PPV_ARGS(&m_depthResource));

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
	device->CreateDepthStencilView(m_depthResource.Get(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());//	
}
