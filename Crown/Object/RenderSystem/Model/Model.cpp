#include "Model.h"
#include "./../../../System.h"
#include "ModelLoader.h"
#include "./../DirectX12Wraps/ResourceUploader.h"

Crown::RenderObject::Model::Model()
	:
	m_position(DirectX::XMFLOAT3(0, 0, 0)),
	m_rotate(DirectX::XMFLOAT3(0, 0, 0)),
	m_materialMeshs(),
	m_descriptorOffset(0),
	m_updateFlag(false),
	m_bundleResource(static_cast<int>(MaterialTag::Num))
{
	System::GetInstance().GetRenderSystem().GetModelManager().AddModel(this);
	for (int i = 0; i < 255; ++i)
	{
		m_pause[i] = DirectX::XMMatrixIdentity();
	}
}

Crown::RenderObject::Model::Model(const Model& model)
	:
	m_position(model.m_position),
	m_rotate(model.m_rotate),
	m_materialMeshs(),
	m_descriptorOffset(0),
	m_updateFlag(false),
	m_bundleResource(static_cast<int>(MaterialTag::Num)),
	m_pause()
{
	System::GetInstance().GetRenderSystem().GetModelManager().AddModel(this);
}

Crown::RenderObject::Model::~Model()
{
	System::GetInstance().GetRenderSystem().GetModelManager().DeleteModel(this);
}

void Crown::RenderObject::Model::Copy(const Model& model)
{
	CopyModel copyModel(*this, model);
	System::GetInstance().GetRenderSystem().GetModelManager().LoadModel(&copyModel);
}

void Crown::RenderObject::Model::SetPause(const DirectX::XMMATRIX* pause) noexcept
{
	for (int i = 0; i < 255; ++i)
	{
		m_pause[i] = pause[i];
	}
	StackDataUploadQueue();
}

void Crown::RenderObject::Model::SetDrawFlag(bool flag)
{
	if (flag)
	{
		System::GetInstance().GetRenderSystem().GetModelManager().AddModel(this);
	}
	else
	{
		System::GetInstance().GetRenderSystem().GetModelManager().DeleteModel(this);
	}
}

void Crown::RenderObject::Model::LoadPMD(const std::wstring& fileName)
{
	ModelLoader loader(fileName, ModelLoader::LoadFile::PMD, *this);
	System::GetInstance().GetRenderSystem().GetModelManager().LoadModel(&loader);
}

void Crown::RenderObject::Model::LoadPMX(const std::wstring& fileName)
{
	ModelLoader loader(fileName, ModelLoader::LoadFile::PMX, *this);
	System::GetInstance().GetRenderSystem().GetModelManager().LoadModel(&loader);
}

void Crown::RenderObject::Model::Create(const std::initializer_list<ColliderAlgorithm::Triangle>& collider, DirectX::XMFLOAT4 color)
{
	CreateModel create(collider, color, *this);
	System::GetInstance().GetRenderSystem().GetModelManager().LoadModel(&create);
}

void Crown::RenderObject::Model::Draw(MaterialTag drawTag, ID3D12GraphicsCommandList* commandList)
{
	//	�g�p���郊�\�[�X���X�g����������
	m_bundleResource[static_cast<int>(drawTag)].clear();

	//	�o���h���R�}���h���X�g�ɃR�}���h��ςނ恙
	for (MaterialMesh& materialMesh : m_materialMeshs)
	{
		materialMesh.Draw(drawTag, commandList, m_bundleResource[static_cast<int>(drawTag)]);
	}
}

void Crown::RenderObject::Model::StackDataUploadQueue()
{
	m_updateFlag = true;
	System::GetInstance().GetRenderSystem().GetModelManager().StackDataUploadQueue(this);
}

//	�f�[�^�̃A�b�v���[�h���s���恙
void Crown::RenderObject::Model::DataUpload()
{
	//	�A�b�v���[�h�悪������Ώ������Ȃ��恙
	if (m_resource.Get() == nullptr || !m_updateFlag)
	{
		return;
	}
	m_updateFlag = false;

	//	�A�b�v���[�h�p�̃����_�����恙
	auto upload = [&](ModelData* map)
	{
		map->world = DirectX::XMMatrixRotationRollPitchYaw(m_rotate.x, m_rotate.y, m_rotate.z) * DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

		const int boneNum = static_cast<int>(m_bone.GetData().size());

		if(boneNum < 0 || 65535 < boneNum)
		{
			return;
		}

		std::vector<DirectX::XMMATRIX> bone(boneNum);
		for (int i = 0; i < boneNum; ++i)
		{
			bone[i] = m_pause[i];
		}
		if (m_bone.IsActive())
		{
			m_bone.Find(L"�S�Ă̐e").RecursiveMatrixMultiply(bone.data());
		}
		for (int i = 0; i < boneNum; ++i)
		{
			map->bone[i] = bone[i];
		}
	};

	//	�A�b�v���[�h��
	ResourceUploader::GetInstance()->UploadData<Model::ModelData>(m_resource, ResourceUploader::GetInstance()->Get255AlignmentSize<Model::ModelData>(1), upload);
}