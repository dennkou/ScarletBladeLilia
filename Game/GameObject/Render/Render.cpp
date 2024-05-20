#include "Render.h"
#include "./../../Crown/System.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"
#include "./../../Crown/Object/RenderSystem/DirectX12Wraps/DefaultRootSignature.h"
#include "./../../Crown/Object/RenderSystem/Model/FileType/Pmx.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"


std::shared_ptr<Crown::RenderObject::RenderTarget> Render::shadowMap;
Crown::RenderObject::GraphicsPipeline* Render::shadowMapGraphicsPipeline;
Crown::RenderObject::BlobConstBuffer* Render::shadowMapBuffer;

Render::Render(Game* game)
	:
	GameObject(game),
	m_debug()
{
	{
		Crown::RenderObject::Model* model = new Crown::RenderObject::Model();
		model->LoadPMX(L"Resource/Model/PMX/bullet.pmx");
		delete model;
	}

	ID3D12Device* device = Crown::System::GetInstance().GetRenderSystem().GetDevice().Get();


	//	影の描画設定だよ☆
	{
		shadowMap.reset(new Crown::RenderObject::RenderTarget(Crown::RenderObject::MaterialTag::Shadow, 2048 << 3, 2048 << 3, DirectX::XMFLOAT4(1, 1, 1, 1)));
		Crown::System::GetInstance().GetRenderSystem().AddRenderTarget(0, shadowMap);

		shadowMapGraphicsPipeline = new Crown::RenderObject::GraphicsPipeline();
		D3D12_RASTERIZER_DESC rasterizer = shadowMapGraphicsPipeline->GetState().RasterizerState;
		rasterizer.CullMode = D3D12_CULL_MODE_NONE;
		shadowMapGraphicsPipeline->SetRasterizerState(rasterizer);
		D3D12_BLEND_DESC blendDesc = shadowMapGraphicsPipeline->GetState().BlendState;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		shadowMapGraphicsPipeline->SetBlendState(blendDesc);
		shadowMapGraphicsPipeline->SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Default/ShadowVS"));
		shadowMapGraphicsPipeline->SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Default/ShadowPS"));
		shadowMapGraphicsPipeline->SetRootSignature(Crown::RenderObject::DefaultRootSignature::GetRootSignature().GetRootSignature().Get());
		shadowMapGraphicsPipeline->SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		shadowMapGraphicsPipeline->Commit(device);

		std::vector<Crown::RenderObject::BlobConstBuffer::DataType> dataTypeDescriptor;
		dataTypeDescriptor.push_back(Crown::RenderObject::BlobConstBuffer::DataType::Matrix);
		shadowMapBuffer = new Crown::RenderObject::BlobConstBuffer(dataTypeDescriptor, device);
	}

	//	デバック表示だよ☆
	m_debug.SetTextureDescriptorOffset(shadowMap->GetTexture());
	m_debug.SetAlpha(1.0f);
	m_debug.SetPosition(DirectX::XMFLOAT2(0.75, 0.75));
	m_debug.SetSize(DirectX::XMFLOAT2(0.25, 0.25));
	m_debug.SetPriority(100);
}

Render::~Render()
{
	delete shadowMapBuffer;
	delete shadowMapGraphicsPipeline;
}

void Render::OnGameUpdate(Timer& timer)
{
	timer;

	float size = 10;

	DirectX::XMFLOAT3 eye = Crown::RenderObject::Camera::GetInstance()->GetEye();
	DirectX::XMFLOAT3 cameraRotate = Crown::RenderObject::Camera::GetInstance()->GetRotate();
	DirectX::XMFLOAT3 position;
	position.x = cos(cameraRotate.x) * sin(cameraRotate.y)	* size + eye.x;
	position.y = sin(cameraRotate.x)						* size + eye.y;
	position.z = cos(cameraRotate.x) * cos(cameraRotate.y)	* size + eye.z;
	shadowMapBuffer->SetParameter(0, 
		DirectX::XMMatrixInverse(nullptr,
			DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(50)) *
			DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(60)) *
			//DirectX::XMMatrixTranslation(position.x, position.y, position.z) *
			DirectX::XMMatrixTranslation(0, 0, 0)) *
		DirectX::XMMatrixOrthographicOffCenterLH(-size, size, -size, size, -size, size)
	);
}