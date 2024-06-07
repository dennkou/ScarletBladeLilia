#include "StageModel.h"
#include "./../../Crown/System.h"

StageModel::StageModel(const Crown::RenderObject::Model& model, unsigned int instanceCount)
{
	m_instanceCount = instanceCount;
	Crown::System::GetInstance().GetRenderSystem().GetModelManager().AddModel(this);
}

StageModel::~StageModel()
{
	Crown::System::GetInstance().GetRenderSystem().GetModelManager().DeleteModel(this);
}

void StageModel::SetMatrix(unsigned int index, const DirectX::XMMATRIX& matrix)
{
	m_matrixs[index] = matrix;
}

void StageModel::DataUpload()
{
	//	アップロード先が無ければ処理しないよ☆
	if (m_resource.Get() == nullptr || m_matrixs.empty())
	{
		return;
	}

	//	アップロード用のラムダ式だよ☆
	auto upload = [&](DirectX::XMMATRIX* map)
		{
			for (unsigned int i = 0, size = static_cast<unsigned int>(m_matrixs.size()); i < size; ++i)
			{
				*(map + i) = m_matrixs[i];
			}
		};

	//	アップロード☆
	Crown::RenderObject::ResourceUploader::GetInstance()->UploadData<DirectX::XMMATRIX>(m_resource, Crown::RenderObject::ResourceUploader::GetInstance()->Get255AlignmentSize<DirectX::XMMATRIX>(m_matrixs.size()), upload);
}

void StageModel::Draw(Crown::RenderObject::MaterialTag drawTag, ID3D12GraphicsCommandList* commandList)
{

}