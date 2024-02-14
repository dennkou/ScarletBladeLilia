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
	m_bundleCommandLists(static_cast<int>(MaterialTag::Num)),
	m_bundleCommandAllocators(static_cast<int>(MaterialTag::Num)),
	m_bundleFlag(static_cast<int>(MaterialTag::Num),false),
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
	m_bundleCommandLists(static_cast<int>(MaterialTag::Num)),
	m_bundleCommandAllocators(static_cast<int>(MaterialTag::Num)),
	m_bundleFlag(static_cast<int>(MaterialTag::Num),false),
	m_bundleResource(static_cast<int>(MaterialTag::Num)),
	m_pause()
{
	System::GetInstance().GetRenderSystem().GetModelManager().AddModel(this);
}

Crown::RenderObject::Model::~Model()
{
	System::GetInstance().GetRenderSystem().GetModelManager().DeleteModel(this);
}



void Crown::RenderObject::Model::SetPause(const DirectX::XMMATRIX* pause) noexcept
{
	for (int i = 0; i < 255; ++i)
	{
		m_pause[i] = pause[i];
	}
	m_updateFlag = true;
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

void Crown::RenderObject::Model::Draw(MaterialTag drawTag, GraphicsCommandList& commandList)
{
	//	���f�����ɕύX���������烂�f���p�萔�o�b�t�@�̍X�V���s���恙
	if (m_updateFlag)
	{
		m_updateFlag = false;
		DataUpload();
	}

	//	�`�施�߂��o���h��������Ă��Ȃ���΃o���h������
	if (!m_bundleFlag[static_cast<int>(drawTag)])
	{
		//	�g�p���郊�\�[�X���X�g����������
		m_bundleResource[static_cast<int>(drawTag)].clear();

		//	�v�����ꂽ�}�e���A���^�O�ɑΉ�����o���h���R�}���h����������Ė�����ΐ�������恙
		if (m_bundleCommandLists[static_cast<int>(drawTag)].Get() == nullptr)
		{
			System::GetInstance().GetRenderSystem().GetModelManager().CreateBundle(m_bundleCommandLists[static_cast<int>(drawTag)], m_bundleCommandAllocators[static_cast<int>(drawTag)]);
		}

		//	�o���h���R�}���h���X�g�ɃR�}���h��ςނ恙
		for (MaterialMesh& materialMesh : m_materialMeshs)
		{
			materialMesh.Draw(drawTag, m_bundleCommandLists[static_cast<int>(drawTag)].Get(), m_bundleResource[static_cast<int>(drawTag)]);
		}
		m_bundleCommandLists[static_cast<int>(drawTag)]->Close();

		m_bundleFlag[static_cast<int>(drawTag)] = true;
	}
	commandList.GetGraphicsCommandList()->ExecuteBundle(m_bundleCommandLists[static_cast<int>(drawTag)].Get());
	for (Microsoft::WRL::ComPtr<ID3D12Resource>& resource : m_bundleResource[static_cast<int>(drawTag)])
	{
		commandList.LockResource(resource);
	}
}

//	�f�[�^�̃A�b�v���[�h���s���恙
void Crown::RenderObject::Model::DataUpload()
{
	//	�A�b�v���[�h�悪������Ώ������Ȃ��恙
	if (m_resource.Get() == nullptr)
	{
		return;
	}

	//	�A�b�v���[�h�p�̃����_�����恙
	auto upload = [&](ModelData* map)
	{
		map->world = DirectX::XMMatrixRotationRollPitchYaw(m_rotate.x, m_rotate.y, m_rotate.z) * DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

		const int boneNum = m_bone.GetData().size();

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