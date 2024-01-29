#include "StageObject.h"

#include "./../../Render/MaterialFactory.h"
#include "./../../Crown/System.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"
#include "./../../Crown/Object/RenderSystem/Model/FileType/Pmx.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"
#include "./../../Crown/Object/RenderSystem/TextureBuffer.h"

StageObject::StageObject(Game* game, DirectX::XMFLOAT3 position)
	:
	Object(game, position, DirectX::XMFLOAT3(0,0,0), L"Resource/Model/PMX/ステージ.pmx")
{
	CreateMaterial();
}

StageObject::~StageObject()
{
}

void StageObject::CreateMaterial()
{
	Crown::RenderObject::TextureBuffer& textureBuffer = Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer();
	MaterialFactory materialFactory;
	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> bufferData;			//	データ構造を指定する配列だよ☆
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float3);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Int);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float);

	//	服のマテリアルの生成をするよ☆
	Crown::RenderObject::GraphicsPipeline graphicsPipeline;
	{
		graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
		graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaClothes_PS"));
		graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
	}

	//	定数バッファの作成☆
	Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
	constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
	constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
	constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.6f, 0.5f, 0.4f));
	constBuffer.SetParameter(3, 0);
	constBuffer.SetParameter(4, DirectX::XMFLOAT4());
	constBuffer.SetParameter(5, 0.0f);

	//	定数バッファを指定☆
	std::vector<unsigned int> constBufferIndexs;
	constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
	constBufferIndexs.push_back(GetModel().GetDescriptorOffest());
	constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

	//	テクスチャを指定☆
	std::vector<unsigned int> textureBufferIndexs;
	textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/リリア/textures/服.png"));

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
	resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
	resources.emplace_back(GetModel().GetModelData());
	resources.emplace_back(constBuffer.GetBuffer());
	resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/リリア/textures/服.png")));

	std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
	constBuffers.push_back(constBuffer);

	materialFactory.CreateMaterial(graphicsPipeline, GetModel(), 0, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
}