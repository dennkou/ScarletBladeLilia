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

	//	データのアップロードを行うよ☆
	virtual void DataUpload() override;

	virtual void Draw(Crown::RenderObject::MaterialTag drawTag, ID3D12GraphicsCommandList* commandList) override;
private:
	std::vector<DirectX::XMMATRIX> m_matrixs;

	unsigned int m_instanceCount;

	Crown::RenderObject::Vertices m_vertices;															//	頂点データ☆
	std::vector<Crown::RenderObject::MaterialMesh> m_materialMeshs;										//
	unsigned int m_descriptorOffset;																	//	モデルのインスタンスデータのオフセット位置☆
	Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;													//	モデルのインスタンスデータのメモリ領域☆
	std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>>	m_bundleResource;				//	各描画種の描画で使用するリソースデータ☆
};