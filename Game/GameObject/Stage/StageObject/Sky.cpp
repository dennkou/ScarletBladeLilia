#include "Sky.h"

#include "./../../Render/MaterialFactory.h"
#include "./../../Crown/System.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"
#include "./../../Crown/Object/RenderSystem/Model/FileType/Pmx.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"
#include "./../../Crown/Object/RenderSystem/TextureBuffer.h"
#include "./../../Crown/Object/RenderSystem/RenderCommands/RenderCommandFactory.h"

Sky::Sky()
{
	m_model.LoadPMX(L"Resource/Model/PMX/Stage/sky.pmx");
	m_model.SetPosition(DirectX::XMFLOAT3(0, 10, 0));

	Crown::RenderObject::TextureBuffer& textureBuffer = Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer();
	MaterialFactory materialFactory;


	//	マテリアルの生成をするよ☆
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			D3D12_RASTERIZER_DESC rasterizer = graphicsPipeline.GetState().RasterizerState;
			rasterizer.CullMode = D3D12_CULL_MODE_NONE;
			graphicsPipeline.SetRasterizerState(rasterizer);
			D3D12_DEPTH_STENCIL_DESC depthStencilState = graphicsPipeline.GetState().DepthStencilState;
			depthStencilState.DepthEnable = false;
			graphicsPipeline.SetDepthStencilState(depthStencilState);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Obj/SkyVS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Obj/SkyPS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	定数バッファを指定☆
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());

		//	テクスチャを指定☆
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/Stage/textures/sky.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/Stage/textures/sky.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 0, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}
}

Sky::~Sky()
{
}