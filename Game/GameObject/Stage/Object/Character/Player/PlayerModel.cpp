#include "PlayerModel.h"
#include "./../../../../Render/MaterialFactory.h"
#include "./../../Crown/System.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"
#include "./../../Crown/Object/RenderSystem/Model/FileType/Pmx.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"
#include "./../../Crown/Object/RenderSystem/TextureBuffer.h"
#include "./../../../../Render/Render.h"
#include "./../../Crown/Object/RenderSystem/Animation/AnimationData.h"


PlayerModel::PlayerModel()
{
	m_model.LoadPMX(L"Resource/Model/PMX/リリア/リリア.pmx");
	AnimLoad();
	CreateMaterial();
}

PlayerModel::~PlayerModel()
{

}

void PlayerModel::Update()
{
	m_model.SetPause(m_bone);
}

void PlayerModel::AnimLoad()
{
	m_standAnim.LaodVMD(L"Resource/Motion/待機.vmd");
	m_walkStartAnim.LaodVMD(L"Resource/Motion/歩き始め.vmd");
	m_walkAnim.LaodVMD(L"Resource/Motion/歩き.vmd");
	//m_runStartAnim.LaodVMD(L"Resource/Motion/歩き.vmd");
	m_runAnim.LaodVMD(L"Resource/Motion/ダッシュ.vmd");
	m_frontStepAnim.LaodVMD(L"Resource/Motion/前ステップ.vmd");
	m_drawingSwordAttackAnim.LaodVMD(L"Resource/Motion/抜刀攻撃.vmd");
	m_slashAttackAnim.LaodVMD(L"Resource/Motion/リリアモーション攻撃.vmd");
}

void PlayerModel::CreateMaterial()
{
	Crown::RenderObject::TextureBuffer& textureBuffer = Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer();
	MaterialFactory materialFactory;
	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> bufferData;				//	データ構造を指定する配列だよ☆
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float3);

	//	服のマテリアルの生成をするよ☆
	{
		//materialFactory.CreateMaterial(graphicsPipeline, m_model, 0, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 0);
		materialFactory.CreateDefaultMaterial(m_model, 0, textureBuffer.TextureAcquisition(L"Resource/Model/PMX/リリア/textures/服.png"), DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f), DirectX::XMFLOAT3(0.6f, 0.5f, 0.4f));
	}

	//	髪のマテリアルの生成をするよ☆
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaHair_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	定数バッファの作成☆
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	定数バッファを指定☆
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	テクスチャを指定☆
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/リリア/textures/kami.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/リリア/textures/kami.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 7, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 7);
	}

	//	タイツのマテリアルの生成をするよ☆
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaTights_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	定数バッファの作成☆
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	定数バッファを指定☆
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	テクスチャを指定☆
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/リリア/textures/肌.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/リリア/textures/肌.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 4, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 4);
	}

	//	肌のマテリアルの生成をするよ☆
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaSkin_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	定数バッファの作成☆
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	定数バッファを指定☆
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	テクスチャを指定☆
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/リリア/textures/肌.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/リリア/textures/肌.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 3, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 3);
	}

	//	ブーツのマテリアルの生成をするよ☆
	{
		materialFactory.CreateShadow(m_model, 2);
		materialFactory.CreateDefaultMaterial(m_model, 2, textureBuffer.TextureAcquisition(L"Resource/Model/PMX/リリア/textures/靴.png"), DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f), DirectX::XMFLOAT3(0.6f, 0.5f, 0.4f));
	}

	//	ブーツのマテリアルの生成をするよ☆
	{
		materialFactory.CreateShadow(m_model, 1);
		materialFactory.CreateDefaultMaterial(m_model, 1, textureBuffer.TextureAcquisition(L"Resource/Model/PMX/リリア/textures/靴.png"), DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f), DirectX::XMFLOAT3(0.6f, 0.5f, 0.4f));
	}

	//	眉のマテリアルの生成をするよ☆
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaEye_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	定数バッファの作成☆
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	定数バッファを指定☆
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	テクスチャを指定☆
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/リリア/textures/眉.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/リリア/textures/眉.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 6, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 6);
	}

	//	剣のマテリアルの生成をするよ☆
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaHair_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	定数バッファの作成☆
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	定数バッファを指定☆
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	テクスチャを指定☆
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/リリア/textures/sword.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/リリア/textures/sword.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 8, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateMaterial(graphicsPipeline, m_model, 9, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateMaterial(graphicsPipeline, m_model, 10, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 8);
		materialFactory.CreateShadow(m_model, 9);
		materialFactory.CreateShadow(m_model, 10);
	}

	//	白目のマテリアルの生成をするよ☆
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaEye_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	定数バッファの作成☆
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	定数バッファを指定☆
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	テクスチャを指定☆
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"白テクスチャ"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/リリア/textures/sword.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 11, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}

	//	瞳のマテリアルの生成をするよ☆
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaEye_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	定数バッファの作成☆
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	定数バッファを指定☆
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	テクスチャを指定☆
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/リリア/textures/瞳.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/リリア/textures/瞳.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 12, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}
}