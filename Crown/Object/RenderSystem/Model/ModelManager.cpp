#include "ModelManager.h"
#include "Model.h"
#include "ModelLoader.h"
#include "./../DirectX12Wraps/DefaultRootSignature.h"
#include "./../DirectX12Wraps/DefaultRootSignature.h"

Crown::RenderObject::ModelManager::ModelManager()
	:
	m_device(),
	m_textureBuffer(),
	m_bundleCommandLists(static_cast<unsigned int>(MaterialTag::Num)),
	m_bundleCommandAllocators(static_cast<unsigned int>(MaterialTag::Num)),
	m_bundle(static_cast<unsigned int>(MaterialTag::Num))
{
}

Crown::RenderObject::ModelManager::~ModelManager()
{
}

void Crown::RenderObject::ModelManager::Initialize(ID3D12Device* device, TextureBuffer* textureBuffer)
{
	m_device = device;
	m_textureBuffer = textureBuffer;

	for (unsigned int i = 0; i < static_cast<unsigned int>(MaterialTag::Num); ++i)
	{
		m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&m_bundleCommandAllocators[i]));
		m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, m_bundleCommandAllocators[i].Get(), nullptr, IID_PPV_ARGS(&m_bundleCommandLists[i]));
		m_bundleCommandLists[i]->Close();
	}
	ResetBundle();
}

void Crown::RenderObject::ModelManager::AddModel(Model* newModel)
{
	//	Ç∑Ç≈Ç…Ç†Ç¡ÇΩÇÁìoò^ÇµÇ»Ç¢Åô
	if (std::find(m_models.begin(), m_models.end(), newModel) == m_models.end())
	{
		m_models.push_back(newModel);
	}
	ResetBundle();
}

void Crown::RenderObject::ModelManager::DeleteModel(Model* deleteModel)
{
	for (int i = 0, size = static_cast<int>(m_models.size()); i < size; ++i)
	{
		if (m_models[i] == deleteModel) 
		{
			m_models.erase(m_models.begin() + i);
			break;
		}
	}
	for (int i = 0, size = static_cast<int>(m_uploadQueue.size()); i < size;)
	{
		if (m_uploadQueue[i] == deleteModel)
		{
			m_uploadQueue[i]->DataUpload();
 			m_uploadQueue.erase(m_uploadQueue.begin() + i);
			--size;
		}
		else
		{
			++i;
		}
	}
	ResetBundle();
}

void Crown::RenderObject::ModelManager::DataCopy()
{
	for (Model* model : m_uploadQueue)
	{
		model->DataUpload();
	}
	m_uploadQueue.clear();
}

void Crown::RenderObject::ModelManager::Draw(MaterialTag drawTag, GraphicsCommandList& commandList)
{
	if (!m_bundle[static_cast<unsigned int>(drawTag)])
	{
		commandList.WaitForGpu();
		m_bundleCommandAllocators[static_cast<unsigned int>(drawTag)].Get()->Reset();
		m_bundleCommandLists[static_cast<unsigned int>(drawTag)].Get()->Reset(m_bundleCommandAllocators[static_cast<unsigned int>(drawTag)].Get(), nullptr);

		ID3D12DescriptorHeap* descriptorHeap = DescriptorHeaps::GetInstance().GetDescriptorHeap();
		m_bundleCommandLists[static_cast<unsigned int>(drawTag)]->SetDescriptorHeaps(1, &descriptorHeap);
		m_bundleCommandLists[static_cast<unsigned int>(drawTag)]->SetGraphicsRootSignature(DefaultRootSignature::GetRootSignature().GetRootSignature().Get());
		m_bundleCommandLists[static_cast<unsigned int>(drawTag)]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		for (Model* model : m_models)
		{
			model->Draw(drawTag, m_bundleCommandLists[static_cast<unsigned int>(drawTag)].Get());
		}
		m_bundleCommandLists[static_cast<unsigned int>(drawTag)].Get()->Close();
		m_bundle[static_cast<unsigned int>(drawTag)] = true;
	}
	commandList.GetGraphicsCommandList()->ExecuteBundle(m_bundleCommandLists[static_cast<unsigned int>(drawTag)].Get());
}

void Crown::RenderObject::ModelManager::LoadModel(Model::ModelLoader* modelLoader)
{
	modelLoader->Load(m_device, m_textureBuffer);
}

void Crown::RenderObject::ModelManager::StackDataUploadQueue(Model* model)
{
	m_uploadQueue.push_back(model);
}

void Crown::RenderObject::ModelManager::ResetBundle()
{
	unsigned long long size = m_bundle.size();
	for (unsigned int i = 0; i < size; ++i)
	{
		m_bundle[i] = false;
	}
}
