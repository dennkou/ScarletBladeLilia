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
#ifdef _DEBUG
	m_debug(),
#endif // _DEBUG
	m_debugModel(),

	GameObject(game)
	//m_color(),
	//m_blurShadowMap(),
	//m_blurShadowMapModel()
{
//#ifdef _DEBUG
	m_debugModel.LoadPMX(L"Resource/Model/PMX/bullet.pmx");
//#endif // _DEBUG
	ID3D12Device* device = Crown::System::GetInstance().GetRenderSystem().GetDevice().Get();

	//	影の描画設定だよ☆
	{
		shadowMap.reset(new Crown::RenderObject::RenderTarget(Crown::RenderObject::MaterialTag::Shadow, 2048 << 3, 2048 << 3, DirectX::XMFLOAT4(1, 1, 1, 1)));
		Crown::System::GetInstance().GetRenderSystem().AddRenderTarget(0, shadowMap);

		//　シャドウマップ用のパイプラインステートオブジェクトを作成するよ☆
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
		shadowMapGraphicsPipeline->SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		shadowMapGraphicsPipeline->SetRootSignature(Crown::RenderObject::DefaultRootSignature::GetRootSignature().GetRootSignature().Get());
		shadowMapGraphicsPipeline->Commit(device);

		std::vector<Crown::RenderObject::BlobConstBuffer::DataType> dataTypeDescriptor;
		dataTypeDescriptor.push_back(Crown::RenderObject::BlobConstBuffer::DataType::Matrix);
		shadowMapBuffer = new Crown::RenderObject::BlobConstBuffer(dataTypeDescriptor, device);



		//Crown::System::GetInstance().GetRenderSystem().AddRenderTarget(0, shadowMap);


		//	ブラー用モデルの作成だよ☆
		std::vector<Crown::RenderObject::BlobConstBuffer::DataType> dataTypes;
		dataTypes;
		//m_blurShadowMap.reset(new PostEffects(1024, 1024, DirectX::XMFLOAT4(1, 1, 1, 1), DXGI_FORMAT_R32_FLOAT, L"", L"", dataTypes));

		//m_blurGraphicsPipeline
	}

	//	ディファードレンタリング用の統合パスだよ☆
	{
		m_frameBuffer.reset(new Crown::RenderObject::MultipleRenderTarget(Crown::RenderObject::MaterialTag::FrameBuffer,
			1280,
			720,
			{
				DirectX::XMFLOAT4(1,1,1,0),
				DirectX::XMFLOAT4(1,1,1,1),
			}
		));
		Crown::System::GetInstance().GetRenderSystem().AddRenderTarget(0, m_frameBuffer);

		m_integrationPath.SetColorBuffer(m_frameBuffer->GetTexture(0));
		m_integrationPath.SetNormalBuffer(m_frameBuffer->GetTexture(1));
		m_integrationPath.SetDepthBuffer(m_frameBuffer->GetDepth(0));
		m_integrationPath.SetShadowBuffer(shadowMap->GetDepth());
		m_integrationPath.SetPriority(-1);
	}

	//	デバック表示だよ☆
#ifdef _DEBUG
	m_debug.SetTextureDescriptorOffset(shadowMap->GetDepth());
	m_debug.SetAlpha(1.0f);
	m_debug.SetPosition(DirectX::XMFLOAT2(0.75, 0.75));
	m_debug.SetSize(DirectX::XMFLOAT2(0.25, 0.25));
	m_debug.SetPriority(100);
#endif // _DEBUG
}

Render::~Render()
{
	delete shadowMapBuffer;
	delete shadowMapGraphicsPipeline;
}

void Render::OnGameUpdate(Timer& timer)
{
	timer;

	//float size = 20;
	//float size = 100;
	//float size = 500;
	float size = 1500;

	//	ライト位置の計算だよ☆
	DirectX::XMFLOAT3 position = {};
	{
		DirectX::XMFLOAT3 positionGap = DirectX::XMFLOAT3(0,size / 2,0);
		DirectX::XMFLOAT3 eye = Crown::RenderObject::Camera::GetInstance()->GetGazePoint();
		DirectX::XMFLOAT3 cameraRotate = Crown::RenderObject::Camera::GetInstance()->GetRotate();
		position.x = cos(cameraRotate.x) * sin(cameraRotate.y) * size / 2+ eye.x + positionGap.x;
		position.y = sin(-cameraRotate.x) * 0 + eye.y + positionGap.y;
		position.z = cos(cameraRotate.x) * cos(cameraRotate.y) * size / 2 + eye.z + positionGap.z;
	}

	DirectX::XMFLOAT2 lightRotate = DirectX::XMFLOAT2(60, 60);
	DirectX::XMFLOAT3 lightVector = {};
	lightVector.x = cos(lightRotate.x) * cos(lightRotate.y);
	lightVector.y = sin(lightRotate.x);
	lightVector.z = cos(lightRotate.x) * sin(lightRotate.y);

	//	影行列の計算
	DirectX::XMMATRIX lightMatrix = DirectX::XMMatrixInverse(nullptr,
		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(lightRotate.x)) *
		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(lightRotate.y)) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z)) *
		DirectX::XMMatrixOrthographicOffCenterLH(-size, size, -size, size, -size, size);
	shadowMapBuffer->SetParameter(0, lightMatrix);

	m_integrationPath.SetCameraBuffer(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
	m_integrationPath.SetShadow(lightMatrix);
	m_integrationPath.SetLightVector(lightVector);
}