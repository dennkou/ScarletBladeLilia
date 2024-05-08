#include "Aisle.h"

#include "./../../Render/MaterialFactory.h"
#include "./../../Crown/System.h"
#include "./../../Crown/Object/RenderSystem/Shader.h"
#include "./../../Crown/Object/RenderSystem/Model/FileType/Pmx.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"
#include "./../../Crown/Object/RenderSystem/TextureBuffer.h"
#include "./../../../../MathLibrary.h"

Aisle::Aisle()
{

}

Aisle::~Aisle()
{
}

void Aisle::Create(DirectX::XMFLOAT3 position, float rotate, const Crown::RenderObject::Model* model, unsigned int num)
{
	rotate = DirectX::XMConvertToRadians(rotate);
	const Crown::RenderObject::Model& copyModel = *model;
	m_model.resize(num);
	for (unsigned int i = 0; i < num; ++i)
	{
		m_model[i].Copy(copyModel);
		m_model[i].SetPosition(VectorAdd(position, DirectX::XMFLOAT3(sin(rotate) * (i * 100.0f), 0, cos(rotate) * (i * 100.0f))));
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

	for (unsigned int i = 0, size = static_cast<unsigned int>(m_model.size()); i < size; ++i)
	{
		m_model[i].SetDrawFlag(VectorDistance(Crown::RenderObject::Camera::GetInstance()->GetEye(), m_model[i].GetPosition()) < 10000.0f);
	}
}

void Aisle::CreateMaterial()
{
	MaterialFactory materialFactory;
	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> bufferData;			//	�f�[�^�\�����w�肷��z�񂾂恙
	bufferData.emplace_back(Crown::RenderObject::BlobConstBuffer::DataType::Int);


	//	�}�e���A���̐���������恙
	Crown::RenderObject::GraphicsPipeline graphicsPipeline;
	{
		graphicsPipeline.SetVS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Obj/Obj_VS"));
		graphicsPipeline.SetGS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Obj/Obj_GS"));
		graphicsPipeline.SetPS(*Crown::RenderObject::Shader::GetInstance()->GetShader(L"Obj/Obj_PS"));
		graphicsPipeline.SetInputLayout(Crown::RenderObject::Pmx::GetInputLayout());
	}

	for (unsigned int i = 0, size = static_cast<unsigned int>(m_model.size()); i < size; ++i)
	{
		//	�萔�o�b�t�@�̍쐬��
		Crown::RenderObject::BlobConstBuffer constBuffer(bufferData, Crown::System::GetInstance().GetRenderSystem().GetDevice().Get());
		constBuffer.SetParameter(0, 100);

		//	�萔�o�b�t�@���w�聙
		std::vector<unsigned int> constBufferIndexs;
		constBufferIndexs.push_back(Crown::RenderObject::Camera::GetInstance()->GetDescriptorOffset());
		constBufferIndexs.push_back(m_model[i].GetDescriptorOffest());
		constBufferIndexs.push_back(constBuffer.GetDescriptorOffset());

		//	�e�N�X�`�����w�聙
		std::vector<unsigned int> textureBufferIndexs;

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
		resources.emplace_back(Crown::RenderObject::Camera::GetInstance()->GetConstConstBuffer());
		resources.emplace_back(m_model[i].GetModelData());
		resources.emplace_back(constBuffer.GetBuffer());

		std::vector<Crown::RenderObject::BlobConstBuffer> constBuffers;
		constBuffers.push_back(constBuffer);

		materialFactory.CreateMaterial(graphicsPipeline, m_model[i], 0, Crown::RenderObject::MaterialTag::Normal, constBufferIndexs, textureBufferIndexs, resources, constBuffers);
	}
}