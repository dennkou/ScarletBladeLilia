#pragma once
#include <DirectXMath.h>
#include <vector>
#include "./../../Crown/Object/RenderSystem/Model/IModel.h"
#include "./../../Crown/Object/RenderSystem/Model/Model.h"
#include "./../../Crown/Object/RenderSystem/Model/MaterialTag.h"
#include "./../../Crown/Object/RenderSystem/Model/Vertices.h"
#include "./../../Crown/Object/RenderSystem/Model/MaterialMesh.h"



class StageModel : public Crown::RenderObject::IModel
{
public:
	StageModel(const Crown::RenderObject::Model& model, unsigned int instanceCount);
	~StageModel();

	void SetMatrix(unsigned int index, const DirectX::XMMATRIX& matrix);

	//	�f�[�^�̃A�b�v���[�h���s���恙
	virtual void DataUpload() override;

	virtual void Draw(Crown::RenderObject::MaterialTag drawTag, ID3D12GraphicsCommandList* commandList) override;
private:
	std::vector<DirectX::XMMATRIX> m_matrixs;

	unsigned int m_instanceCount;

	Crown::RenderObject::Vertices m_vertices;															//	���_�f�[�^��
	std::vector<Crown::RenderObject::MaterialMesh> m_materialMeshs;										//
	unsigned int m_descriptorOffset;																	//	���f���̃C���X�^���X�f�[�^�̃I�t�Z�b�g�ʒu��
	Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;													//	���f���̃C���X�^���X�f�[�^�̃������̈恙
	std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>>	m_bundleResource;				//	�e�`���̕`��Ŏg�p���郊�\�[�X�f�[�^��
};