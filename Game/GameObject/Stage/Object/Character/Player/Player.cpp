#include "Player.h"
#include "./../../../../Render/MaterialFactory.h"
#include "./../../Crown/System.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"
#include "./../../Crown/Object/RenderSystem/Model/FileType/Pmx.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"
#include "./../../Crown/Object/RenderSystem/TextureBuffer.h"

#include "./../../Crown/Object/RenderSystem/Animation/AnimationData.h"

#include "./PlayerState/PlayerState.h"
#include "./PlayerState/PlayerTitle.h"
#include "./PlayerState/PlayerStateStand.h"
#include "./PlayerState/PlayerStateWalk.h"
#include "./PlayerState/PlayerStateRun.h"
#include "./PlayerState/PlayerAttack.h"
#include "./PlayerState/PlayerAvoidance.h"
#include <numbers>
#include <algorithm>



Player::Player(Game* game, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotate)
	:
	GameObject(game),
	m_model(),
	m_hp(MAX_HP),
	m_inputMove(0,0),
	m_velocity(0,0,0),
	m_collider([&](float damage) {Hit(damage); })
{
	m_model.LoadPMX(L"Resource/Model/PMX/�����A/�����A.pmx");
	m_model.SetPosition(position);
	m_model.SetRotate(rotate);

	CreateMaterial();		//	�}�e���A���̐ݒ������恙
	StateSetUp();			//	�X�e�[�g�}�V����ݒ������恙
	AnimLoad();
}

Player::~Player()
{

}

void Player::OnGameUpdate(Timer& timer)
{
	m_position = m_collider.GetPosition();
	m_playerState.CallStateFunction(&PlayerState::Update, timer.GetWorldTime());
	m_camera.UpdateCameraTransform();
	m_model.SetPause(m_bone);

	ColliderUpdate();
	m_model.SetPosition(m_position);
	m_model.SetRotate(m_rotate);
}

void Player::OnPlayStartAnimation()
{
	m_playerState.CallStateFunction(&PlayerState::OnPlayStartAnimation);
}

void Player::OnPlayStart()
{
	m_playerState.CallStateFunction(&PlayerState::OnPlayStart);
}

void Player::OnInputMove(DirectX::XMFLOAT2 input)
{
	m_inputMove = input;
	m_playerState.CallStateFunction(&PlayerState::OnInputMove, input);
}

void Player::OnInputAttackDown()
{
	m_playerState.CallStateFunction(&PlayerState::OnInputAttackDown);
}

void Player::OnInputAvoidanceDown()
{
	m_playerState.CallStateFunction(&PlayerState::OnInputAvoidanceDown);
}

void Player::OnInputCamera(DirectX::XMFLOAT2 input)
{
	m_playerState.CallStateFunction(&PlayerState::OnInputCamera, input);
}

void Player::OnInputDash(bool input)
{
	m_playerState.CallStateFunction(&PlayerState::OnInputDash, input);
}

void Player::CreateMaterial()
{
	Crown::RenderObject::TextureBuffer& textureBuffer = Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer();
	MaterialFactory materialFactory;
	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> bufferData;				//	�f�[�^�\�����w�肷��z�񂾂恙
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float4);
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Float3);

	//	���̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			D3D12_RASTERIZER_DESC rasterizer = graphicsPipeline.GetState().RasterizerState;
			rasterizer.CullMode = D3D12_CULL_MODE_NONE;
			graphicsPipeline.SetRasterizerState(rasterizer);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaClothes_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	�萔�o�b�t�@�̍쐬��
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.6f, 0.5f, 0.4f));

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

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 0, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}
	
	//	���̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
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

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 7, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}

	//	�^�C�c�̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
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

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 4, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}

	//	���̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
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

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 3, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}

	//	�u�[�c�̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			D3D12_RASTERIZER_DESC rasterizer = graphicsPipeline.GetState().RasterizerState;
			rasterizer.CullMode = D3D12_CULL_MODE_NONE;
			graphicsPipeline.SetRasterizerState(rasterizer);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaClothes_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	�萔�o�b�t�@�̍쐬��
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.6f, 0.55f, 0.5f));

		//	�萔�o�b�t�@���w�聙
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	�e�N�X�`�����w�聙
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/�����A/textures/�C.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/�C.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 2, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}

	//	�u�[�c�̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
			D3D12_RASTERIZER_DESC rasterizer = graphicsPipeline.GetState().RasterizerState;
			rasterizer.CullMode = D3D12_CULL_MODE_NONE;
			graphicsPipeline.SetRasterizerState(rasterizer);
			graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/Riria_VS"));
			graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Riria/RiriaClothes_PS"));
			graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
		}

		//	�萔�o�b�t�@�̍쐬��
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		constBuffer.SetParameter(1, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.01f));
		constBuffer.SetParameter(2, DirectX::XMFLOAT3(0.6f, 0.55f, 0.5f));

		//	�萔�o�b�t�@���w�聙
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model.GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	�e�N�X�`�����w�聙
		std::vector<unsigned int> textureBufferIndexs;
		textureBufferIndexs.push_back(Crown::System::GetInstance().GetRenderSystem().GetTextureBuffer().TextureAcquisition(L"Resource/Model/PMX/�����A/textures/�C.png"));

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model.GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());
		resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Model/PMX/�����A/textures/�C.png")));

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 1, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}

	//	���̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
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

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 5, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}

	//	���̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
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

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 8, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateMaterial(graphicsPipeline, m_model, 9, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
		materialFactory.CreateMaterial(graphicsPipeline, m_model, 10, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}

	//	���ڂ̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
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

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 11, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}

	//	���̃}�e���A���̐���������恙
	{
		Crown::RenderObject::GraphicsPipeline graphicsPipeline;
		{
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

		materialFactory.CreateMaterial(graphicsPipeline, m_model, 12, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}
}

void Player::StateSetUp()
{
	m_playerState.RegisterState<PlayerTitle>(StateID::Title, this);
	m_playerState.RegisterState<PlayerStand>(StateID::Stand, this);
	m_playerState.RegisterState<PlayerWalk>(StateID::Walk, this);
	m_playerState.RegisterState<PlayerRun>(StateID::Run, this);
	m_playerState.RegisterState<PlayerAttack>(StateID::Attack, this);
	m_playerState.RegisterState<PlayerAvoidance>(StateID::Avoidance, this);

	m_playerState.SetEnterFunction(&PlayerState::Enter);
	m_playerState.SetExitFunction(&PlayerState::Exit);

	m_playerState.ChangeState(StateID::Title);
}

void Player::AnimLoad()
{
	for (int i = 0; i < 255; ++i)
	{
		m_bone[i] = DirectX::XMMatrixIdentity();
	}

	m_standAnim.LaodVMD(L"Resource/Motion/�ҋ@.vmd");
	m_walkStartAnim.LaodVMD(L"Resource/Motion/�����n��.vmd");
	m_walkAnim.LaodVMD(L"Resource/Motion/����.vmd");
	m_runAnim.LaodVMD(L"Resource/Motion/�_�b�V��.vmd");
	m_frontStepAnim.LaodVMD(L"Resource/Motion/�O�X�e�b�v.vmd");
	m_drawingSwordAttackAnim.LaodVMD(L"Resource/Motion/�����U��.vmd");
}

void Player::ColliderUpdate()
{
	m_collider.SetPosition(m_position);

	DirectX::XMMATRIX head = m_bone[m_model.GetBoneDate().Find(L"head").GetIndex()];
	DirectX::XMVECTOR scale, rotate, transform;
	DirectX::XMMatrixDecompose(&scale, &rotate, &transform, head);
	DirectX::XMFLOAT3 position;
	transform = DirectX::XMVectorAdd(transform, DirectX::XMVECTOR({ 0.0f, 15.0f, 0.0f, 0.0f }));
	DirectX::XMStoreFloat3(&position, transform);
	m_collider.SetCapsuleVector(position);
}

void Player::CameraRoll(DirectX::XMFLOAT2 input)
{
	DirectX::XMFLOAT3 cameraRotate = m_camera.GetRotate();
	cameraRotate.y = fmodf(cameraRotate.y + input.x / 1000.0f, static_cast<float>(std::numbers::pi * 2));

	cameraRotate.x = fmodf(cameraRotate.x + input.y / 1000.0f, static_cast<float>(std::numbers::pi * 2));
	cameraRotate.x = std::clamp(cameraRotate.x, static_cast<float>(-std::numbers::pi / 2), static_cast<float>(std::numbers::pi / 2));
	m_camera.SetRotate(cameraRotate);
}

void Player::Hit(float damage)
{
	m_hp.Damage(damage);

	m_hpUi.SetPlayerHPPercent(m_hp.GetHPPercent());

	if (m_hp.IsDied())
	{
		EventTrigger(&GameObject::OnPlayerDied);
	}
}