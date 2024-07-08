#include "PostEffects.h"
#include "../../Crown/Object/RenderSystem/DirectX12Wraps/DefaultRootSignature.h"
#include "../../Crown/Object/RenderSystem/DirectX12Wraps/GraphicPipeline.h"
#include "../../Crown/Object/RenderSystem/Shader.h"


std::unique_ptr<Crown::RenderObject::Vertices> PostEffects::vertices;


PostEffects::PostEffects(unsigned int x, unsigned int y, DirectX::XMFLOAT4 clearColor, DXGI_FORMAT format, std::wstring vs, std::wstring ps, std::vector<Crown::RenderObject::BlobConstBuffer::DataType> constBuffer)
	:
	CLEAR_COLOR(clearColor),
	m_xSize(x),
	m_ySize(y),
	m_texture(0),
	m_depth(0),
	m_format(format),
	m_constBufferDate(constBuffer)
{
	{
		m_graphicPipeline.SetRootSignature(Crown::RenderObject::DefaultRootSignature::GetRootSignature().GetRootSignature().Get());

		//	Z�o�b�t�@�̐ݒ肾�恙
		D3D12_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = false;
		desc.StencilEnable = false;
		desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		m_graphicPipeline.SetDepthStencilState(desc);

		//	�C���v�b�g���C�A�E�g�̐ݒ肾�恙
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
		inputLayoutDesc.pInputElementDescs = INPUTLAYOUT;
		inputLayoutDesc.NumElements = INPUTLAYOUT_NUM;
		m_graphicPipeline.SetInputLayout(inputLayoutDesc);

		D3D12_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
		m_graphicPipeline.SetRasterizerState(rasterizerDesc);

		m_graphicPipeline.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		m_graphicPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(vs));
		m_graphicPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(ps));
	}
}

PostEffects::~PostEffects()
{

}

void PostEffects::Initialize(ID3D12Device* device)
{
	CreateRenderTarget(device, m_xSize, m_ySize);
	CreatDepthBuffer(device, m_xSize, m_ySize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = m_format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	m_texture = Crown::RenderObject::DescriptorHeaps::GetInstance().CreateShaderResourceView(m_rtvResource.Get(), srvDesc);

	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	m_depth = Crown::RenderObject::DescriptorHeaps::GetInstance().CreateShaderResourceView(m_depthResource.Get(), srvDesc);

	m_graphicPipeline.Commit(device);

	if (vertices)
	{
		vertices.reset(new Crown::RenderObject::Vertices);
		struct Vertices
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 uv;
		};
		std::vector<Vertices> v(4);
		v[0].position = DirectX::XMFLOAT3(-1, -1, 0);
		v[0].uv = DirectX::XMFLOAT2(0, 1);
		v[1].position = DirectX::XMFLOAT3(-1,  1, 0);
		v[1].uv = DirectX::XMFLOAT2(0, 0);
		v[2].position = DirectX::XMFLOAT3( 1, -1, 0);
		v[2].uv = DirectX::XMFLOAT2(1, 1);
		v[3].position = DirectX::XMFLOAT3( 1,  1, 0);
		v[3].uv = DirectX::XMFLOAT2(1, 0);

		std::vector<unsigned int> index{ 0, 1, 2, 1, 2, 3 };
		vertices->CreateBuffer(device, v, index);
	}
	m_constBuffer.reset(new Crown::RenderObject::BlobConstBuffer(m_constBufferDate, device));
}

void PostEffects::Draw(Crown::RenderObject::GraphicsCommandList& commandList, Crown::RenderObject::ModelManager* modelManager, unsigned int index)
{
	modelManager;
	ID3D12GraphicsCommandList* graphicsCommandList = commandList.GetGraphicsCommandList(index);

	//	�`��J�n��
	{
		CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(m_rtvResource.Get(), D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);	//	���\�[�X�o���A���̐ݒ�����ׂĂɎg�p�\����`���Ƃ��Ďg�p�ɐ؂�ւ���恙
		graphicsCommandList->ResourceBarrier(1, &tmp);																											//	���\�[�X�o���A�̐ݒ�ύX��v������恙
	}

	//	�`��Ώۂ̌��聙
	D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	graphicsCommandList->OMSetRenderTargets(1, &rtv, false, &dsv);																								//	�����_�[�^�[�Q�b�g���w�肷��恙

	//	��ʂ���������
	graphicsCommandList->ClearRenderTargetView(m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(), &CLEAR_COLOR.x, 0, nullptr);
	graphicsCommandList->ClearDepthStencilView(m_dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

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

	graphicsCommandList->RSSetViewports(1, &viewport);
	graphicsCommandList->RSSetScissorRects(1, &scissorrect);

	//	�`�恙
	ID3D12DescriptorHeap* descriptorHeap = Crown::RenderObject::DescriptorHeaps::GetInstance().GetDescriptorHeap();
	graphicsCommandList->SetDescriptorHeaps(1, &descriptorHeap);
	graphicsCommandList->SetGraphicsRootSignature(Crown::RenderObject::DefaultRootSignature::GetRootSignature().GetRootSignature().Get());
	graphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphicsCommandList->IASetIndexBuffer(vertices->GetIndexBufferView());
	graphicsCommandList->IASetVertexBuffers(0, 1, vertices->GetVertexBufferView());
	graphicsCommandList->SetPipelineState(m_graphicPipeline.GetPipelineState().Get());
	graphicsCommandList->SetGraphicsRootDescriptorTable(0, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_constBuffer->GetDescriptorOffset()));
	graphicsCommandList->SetGraphicsRootDescriptorTable(1, Crown::RenderObject::DescriptorHeaps::GetInstance().GetHandle(m_targetTexture));
	graphicsCommandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	//	�`��I���`��
	{
		CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(m_rtvResource.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);	//	���\�[�X�o���A���̐ݒ�����ׂĂɎg�p�\�ɂ���恙
		graphicsCommandList->ResourceBarrier(1, &tmp);
	}
}

inline void PostEffects::CreateRenderTarget(ID3D12Device* device, UINT width, UINT height)
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
	device->CreateRenderTargetView(m_rtvResource.Get(), &rtvDesc, rtvH);						//	�����_�[�^�[�Q�b�g�r���[�̍쐬���恙
}

inline void PostEffects::CreatDepthBuffer(ID3D12Device* device, UINT width, UINT height)
{
	//	�f�v�X�p�̃q�[�v�v���p�e�B���恙
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;												//	DEFAULT��������UNKNOWN�ł悵��
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