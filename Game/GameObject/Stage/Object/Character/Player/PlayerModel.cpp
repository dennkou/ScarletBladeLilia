#include "PlayerModel.h"
#include "./../../../../Render/MaterialFactory.h"
#include "./../../Crown/System.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"
#include "./../../Crown/Object/RenderSystem/Model/FileType/Pmx.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"
#include "./../../Crown/Object/RenderSystem/TextureBuffer.h"
#include "./../../../../Render/Render.h"
#include "./../../Crown/Object/RenderSystem/Animation/AnimationData.h"

#include "PlayerAnimState/PlayerAnimState.h"
#include "PlayerAnimState/PlayerAnimStand.h"
#include "PlayerAnimState/PlayerAnimStandToWalk.h"
#include "PlayerAnimState/PlayerAnimWalk.h"

PlayerModel::PlayerModel()
{
	m_model.LoadPMX(L"Resource/Model/PMX/�����A/�����A.pmx");
	CreateMaterial();
	AnimLoad();
	StateSetUp();
}

PlayerModel::~PlayerModel()
{

}

void PlayerModel::Update(float time)
{
	m_state.CallStateFunction(&PlayerAnimState::Update, time);
	m_model.SetPause(m_bone);
}

void PlayerModel::Attack()
{
	m_state.CallStateFunction(&PlayerAnimState::Attack);
}

void PlayerModel::Avoidance()
{
	m_state.CallStateFunction(&PlayerAnimState::Avoidance);
}

void PlayerModel::Move(DirectX::XMFLOAT2 move)
{
	m_move = move;
	m_state.CallStateFunction(&PlayerAnimState::Move, move);
}

float PlayerModel::GetAnimTime()
{
	return m_state.CallStateFunction(&PlayerAnimState::GetAnimTime);
}

float PlayerModel::GetMaxFrame()
{
	return m_state.CallStateFunction(&PlayerAnimState::GetMaxFrame);
}

void PlayerModel::CreateMaterial()
{
	Crown::RenderObject::TextureBuffer& textureBuffer = Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer();
	MaterialFactory materialFactory;
	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> bufferData;				//	�f�[�^�\�����w�肷��z�񂾂恙
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float3);

	//	���̃}�e���A���̐���������恙
	{
		//materialFactory.CreateMaterial(graphicsPipeline, m_model, 0, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 0);
		materialFactory.CreateDefaultMaterial(m_model, 0, textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/��.png"), DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f), DirectX::XMFLOAT3(0.6f, 0.5f, 0.4f));
	}

	//	���̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaHair_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	�萔�o�b�t�@�̍쐬��
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	�萔�o�b�t�@���w�聙
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	�e�N�X�`�����w�聙
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/�����A/textures/kami.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/kami.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 7, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 7);
	}

	//	�^�C�c�̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaTights_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	�萔�o�b�t�@�̍쐬��
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	�萔�o�b�t�@���w�聙
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	�e�N�X�`�����w�聙
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/�����A/textures/��.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/��.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 4, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 4);
	}

	//	���̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaSkin_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	�萔�o�b�t�@�̍쐬��
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	�萔�o�b�t�@���w�聙
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	�e�N�X�`�����w�聙
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/�����A/textures/��.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/��.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 3, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 3);
	}

	//	�u�[�c�̃}�e���A���̐���������恙
	{
		materialFactory.CreateShadow(m_model, 2);
		materialFactory.CreateDefaultMaterial(m_model, 2, textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/�C.png"), DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f), DirectX::XMFLOAT3(0.6f, 0.5f, 0.4f));
	}

	//	�u�[�c�̃}�e���A���̐���������恙
	{
		materialFactory.CreateShadow(m_model, 1);
		materialFactory.CreateDefaultMaterial(m_model, 1, textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/�C.png"), DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f), DirectX::XMFLOAT3(0.6f, 0.5f, 0.4f));
	}

	//	���̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaHair_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	�萔�o�b�t�@�̍쐬��
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	�萔�o�b�t�@���w�聙
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	�e�N�X�`�����w�聙
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/�����A/textures/��.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/��.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 5, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 5);
	}

	//	���̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaHair_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	�萔�o�b�t�@�̍쐬��
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	�萔�o�b�t�@���w�聙
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	�e�N�X�`�����w�聙
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/�����A/textures/sword.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/sword.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 8, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateMaterial(graphicsPipeline, m_model, 9, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateMaterial(graphicsPipeline, m_model, 10, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateShadow(m_model, 8);
		materialFactory.CreateShadow(m_model, 9);
		materialFactory.CreateShadow(m_model, 10);
	}

	//	���ڂ̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaHair_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	�萔�o�b�t�@�̍쐬��
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	�萔�o�b�t�@���w�聙
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	�e�N�X�`�����w�聙
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/�����A/textures/sword.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/sword.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 11, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}

	//	���̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			graphicsPipeline.SetNumRenderTargets(2);
			graphicsPipeline.SetRTVFormats(1, DXGI_FORMAT_R8G8B8A8_UNORM);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaEye_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	�萔�o�b�t�@�̍쐬��
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));

		//	�萔�o�b�t�@���w�聙
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	�e�N�X�`�����w�聙
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/�����A/textures/��.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/��.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 12, Crown::RenderObject::MaterialTag::FrameBuffer, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}
}

void PlayerModel::StateSetUp()
{
	m_state.RegisterState<PlayerAnimStand>(AnimState::Stand, this);
	m_state.RegisterState<PlayerAnimStandToWalk>(AnimState::StandToWalk, this);
	m_state.RegisterState<PlayerAnimWalk>(AnimState::Walk, this);
	m_state.ChangeState(AnimState::Stand);
}

void PlayerModel::AnimLoad()
{
	for (int i = 0; i < 255; ++i)
	{
		m_bone[i] = DirectX::XMMatrixIdentity();
	}


	m_runAnim.LaodVMD(L"Resource/Motion/�_�b�V��.vmd");
	m_frontStepAnim.LaodVMD(L"Resource/Motion/�O�X�e�b�v.vmd");
	m_drawingSwordAttackAnim.LaodVMD(L"Resource/Motion/�����U��.vmd");
	m_slashAttackAnim.LaodVMD(L"Resource/Motion/�����A���[�V�����U��.vmd");
}