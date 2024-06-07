#include "Aisle.h"

#include "./../../Render/MaterialFactory.h"
#include "./../../Crown/System.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"
#include "./../../Crown/Object/RenderSystem/Model/FileType/Pmx.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"
#include "./../../Crown/Object/RenderSystem/TextureBuffer.h"
#include "./../../../../MathLibrary.h"
#include "./../../Render/Render.h"
#include "./../../Crown/Object/RenderSystem/DirectX12Wraps/DefaultRootSignature.h"

Aisle::Aisle()
{

}

Aisle::~Aisle()
{
}

void Aisle::Create(DirectX::XMFLOAT3 position, float rotate, const Crown::RenderObject::Model* model, unsigned int num)
{
	m_stageModel.reset(new StageModel(*model, 1));
	rotate = DirectX::XMConvertToRadians(rotate);
	const Crown::RenderObject::Model& copyModel = *model;
	m_model.resize(num);
	for (unsigned int i = 0; i < num; ++i)
	{
		m_model[i].Copy(copyModel);
		m_model[i].SetPosition(VectorAdd(position, DirectX::XMFLOAT3(sin(rotate) * (i * size), 0, cos(rotate) * (i * size))));
		m_model[i].SetRotate(DirectX::XMFLOAT3(0, rotate, 0));
		m_model[i].SetDrawFlag(false);
	}

	CreateMaterial();
	m_collider.SetPosition(position);
	m_collider.SetRotate(rotate);
	m_collider.SetAisleNum(num);
}

void Aisle::Update()
{
	bool drawFlag = false;
	Crown::RenderObject::Camera* camera = Crown::RenderObject::Camera::GetInstance();
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 cameraPositon = camera->GetEye();
	DirectX::XMFLOAT3 eyeVector;
	eyeVector.x = cos(camera->GetRotate().x) * sin(camera->GetRotate().y);
	eyeVector.y = sin(camera->GetRotate().x);
	eyeVector.z = cos(camera->GetRotate().x) * cos(camera->GetRotate().y);
	float distance;
	for (unsigned int i = 0, size = static_cast<unsigned int>(m_model.size()); i < size; ++i)
	{
		drawFlag = false;
		position = m_model[i].GetPosition();
		distance = VectorDistance(cameraPositon, position);
		//	オブジェクトはカメラに近い？☆
		if (distance < size)
		{
			drawFlag = true;
		}
		else
		{
			if (distance < Crown::RenderObject::Camera::GetInstance()->GetFar())
			{
				if (VectorInnerProduct(VectorNormalize(VectorSub(cameraPositon, position)), eyeVector) < cos(camera->GetFovAngle()))
				{
					drawFlag = true;
				}
			}
		}
		m_model[i].SetDrawFlag(drawFlag);
	}
}

void Aisle::CreateMaterial()
{
	MaterialFactory materialFactory;
	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> bufferData;			//	データ構造を指定する配列だよ☆
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Int);

	//	マテリアルの生成をするよ☆

	Crown::RenderObject::GraphicsPipeline graphicsPipeline;
	{
		D3D12_RASTERIZER_DESC rasterizer = graphicsPipeline.GetState().RasterizerState;
		rasterizer.CullMode = D3D12_CULL_MODE_BACK;
		graphicsPipeline.SetRasterizerState(rasterizer);
		graphicsPipeline.SetNumRenderTargets(2);
		graphicsPipeline.SetRTVFormats(0, DXGI_FORMAT_R8G8B8A8_UNORM);
		graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
		graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Obj/Obj_VS"));
		graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Obj/Obj_PS"));
		graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		graphicsPipeline.SetRootSignature(Crown::RenderObject::DefaultRootSignature::GetRootSignature().GetRootSignature().Get());
		graphicsPipeline.Commit(Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
	}
	Crown::RenderObject::GraphicsPipeline shadowGraphicsPipeline;
	{
		D3D12_RASTERIZER_DESC rasterizer = shadowGraphicsPipeline.GetState().RasterizerState;
		rasterizer.CullMode = D3D12_CULL_MODE_NONE;
		shadowGraphicsPipeline.SetRasterizerState(rasterizer);
		D3D12_BLEND_DESC blendDesc = shadowGraphicsPipeline.GetState().BlendState;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		shadowGraphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Obj/Obj_Shadow_VS"));
		shadowGraphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Obj/Obj_Shadow_PS"));
		shadowGraphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
	}

	for (unsigned int i = 0, size = static_cast<unsigned int>(m_model.size()); i < size; ++i)
	{
		//	定数バッファを指定☆
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model[i].GetDescriptorOffest());
		constBufferIndexs.push_back(Render::GetShadowMapBuffer()->GetDescriptorOffset());

		//	テクスチャを指定☆
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"グレイグラデーションテクスチャ"));
		//textureBufferIndexs.push_back(Render::GetShadowMapColorBufferIndex());
		//textureBufferIndexs.push_back(Render::GetShadowMapDepthBufferIndex());


		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model[i].GetModelData());

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;

		materialFactory.CreateMaterial(graphicsPipeline, m_model[i], 0, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateMaterial(shadowGraphicsPipeline, m_model[i], 0, Crown::RenderObject::MaterialTag::Shadow, constBufferIndexs, textureBufferIndexs, resources, constBuffers);

		//materialFactory.CreateDefaultMaterial(m_model[i], 0, 0, DirectX::XMFLOAT4(), DirectX::XMFLOAT3());
	}
}