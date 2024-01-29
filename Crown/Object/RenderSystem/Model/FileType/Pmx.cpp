#include "Pmx.h"
#include "./../../../StringAlgorithm.h"
#include "./../../RenderCommands/RenderCommandFactory.h"
#include "./../../Shader.h"
#include "./../../Camera.h"



Microsoft::WRL::ComPtr<ID3D12RootSignature> Crown::RenderObject::Pmx::rootSignature = nullptr;
std::unique_ptr<Crown::RenderObject::GraphicsPipeline> Crown::RenderObject::Pmx::graphicsPipeline = nullptr;
D3D12_INPUT_LAYOUT_DESC Crown::RenderObject::Pmx::inputLayoutDesc;
const D3D12_INPUT_ELEMENT_DESC Crown::RenderObject::Pmx::inputLayout[13] =
{
	{ "POSITION",		0,DXGI_FORMAT_R32G32B32_FLOAT,		0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "NORMAL",			0,DXGI_FORMAT_R32G32B32_FLOAT,		0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "TEXCOORD",		0,DXGI_FORMAT_R32G32_FLOAT,			0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "ADDTEXCOORD",	0,DXGI_FORMAT_R32G32B32A32_FLOAT,	0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "ADDTEXCOORD",	1,DXGI_FORMAT_R32G32B32A32_FLOAT,	0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "ADDTEXCOORD",	2,DXGI_FORMAT_R32G32B32A32_FLOAT,	0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "ADDTEXCOORD",	3,DXGI_FORMAT_R32G32B32A32_FLOAT,	0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "WEIGHT_TYPE",	0,DXGI_FORMAT_R8_UINT,				0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "BONE_INDEX",		0,DXGI_FORMAT_R32G32B32A32_UINT,	0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "WEIGHT",			0,DXGI_FORMAT_R32G32B32A32_FLOAT,	0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "SDEF_C",			0,DXGI_FORMAT_R32G32B32A32_FLOAT,	0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "SDEF_R",			0,DXGI_FORMAT_R32G32B32A32_FLOAT,	0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	{ "SDEF_R",			1,DXGI_FORMAT_R32G32B32A32_FLOAT,	0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
};




void Crown::RenderObject::Pmx::Create(ID3D12Device* device)
{
	//	�f�B�X�N���v�^�����W�̐ݒ肾�恙
	D3D12_DESCRIPTOR_RANGE descriptorRange[7] = {};

	//	�J������񂾂恙
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//	���f�����p���恙
	descriptorRange[1].NumDescriptors = 1;
	descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descriptorRange[1].BaseShaderRegister = 1;
	descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//	�}�e���A���p�萔���恙
	descriptorRange[2].NumDescriptors = 1;
	descriptorRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descriptorRange[2].BaseShaderRegister = 2;
	descriptorRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//	�}�e���A���p�e�N�X�`����
	descriptorRange[3].NumDescriptors = 1;
	descriptorRange[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[3].BaseShaderRegister = 0;
	descriptorRange[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descriptorRange[4].NumDescriptors = 1;
	descriptorRange[4].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[4].BaseShaderRegister = 1;
	descriptorRange[4].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descriptorRange[5].NumDescriptors = 1;
	descriptorRange[5].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[5].BaseShaderRegister = 2;
	descriptorRange[5].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descriptorRange[6].NumDescriptors = 1;
	descriptorRange[6].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[6].BaseShaderRegister = 3;
	descriptorRange[6].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//	���[�g�p�����[�^�̍쐬���s���恙
	static const unsigned int parameterNum = 7;
	D3D12_ROOT_PARAMETER rootParameter[parameterNum] = {};

	//	�J������񂾂恙
	rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameter[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0];
	rootParameter[0].DescriptorTable.NumDescriptorRanges = 1;

	//	���f�����p���恙
	rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameter[1].DescriptorTable.pDescriptorRanges = &descriptorRange[1];
	rootParameter[1].DescriptorTable.NumDescriptorRanges = 1;

	//	�}�e���A���p���恙
	rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[2].DescriptorTable.pDescriptorRanges = &descriptorRange[2];
	rootParameter[2].DescriptorTable.NumDescriptorRanges = 1;

	rootParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[3].DescriptorTable.pDescriptorRanges = &descriptorRange[3];
	rootParameter[3].DescriptorTable.NumDescriptorRanges = 1;

	rootParameter[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[4].DescriptorTable.pDescriptorRanges = &descriptorRange[4];
	rootParameter[4].DescriptorTable.NumDescriptorRanges = 1;

	rootParameter[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[5].DescriptorTable.pDescriptorRanges = &descriptorRange[5];
	rootParameter[5].DescriptorTable.NumDescriptorRanges = 1;

	rootParameter[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[6].DescriptorTable.pDescriptorRanges = &descriptorRange[6];
	rootParameter[6].DescriptorTable.NumDescriptorRanges = 1;

	//	�T���v���[�̐ݒ肾�恙
	const unsigned int samplerNum = 2;
	D3D12_STATIC_SAMPLER_DESC samplerDesc[samplerNum] = {};
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc[0].Filter = D3D12_FILTER_ANISOTROPIC;
	samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc[0].MinLOD = 0.0f;
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc[0].ShaderRegister = 0;

	samplerDesc[1] = samplerDesc[0];//�ύX�_�ȊO���R�s�[
	samplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].ShaderRegister = 1;

	//	���[�g�V�O�l�`���[�̐ݒ肾�恙
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParameter;
	rootSignatureDesc.NumParameters = parameterNum;
	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = samplerNum;

	//	���[�g�V�O�l�`���[���쐬����恙
	ID3DBlob* rootSigBlob = nullptr;
	D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, nullptr);
	device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));	//	���[�g�V�O�l�`���[���쐬����恙
	rootSigBlob->Release();
}

void Crown::RenderObject::Pmx::Load(ID3D12Device* device, std::wstring& fileName, TextureBuffer& textureBuffer, Vertices& verticesBuffer, std::vector<MaterialMesh>& materialMeshs, BoneData& boneData, unsigned int descriptorOffset, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{
	if (rootSignature.Get() == nullptr || !graphicsPipeline)
	{
		Create(device);
		graphicsPipeline.reset(new GraphicsPipeline());
		D3D12_RASTERIZER_DESC rasterizerDesc;
		rasterizerDesc = graphicsPipeline->GetState().RasterizerState;
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
		graphicsPipeline->SetRasterizerState(rasterizerDesc);
		graphicsPipeline->SetVS(*Shader::GetInstance()->GetShader(L"PMD/PmxVs"));
		graphicsPipeline->SetPS(*Shader::GetInstance()->GetShader(L"PMD/MMDPs"));
		graphicsPipeline->SetRootSignature(rootSignature.Get());
		inputLayoutDesc.pInputElementDescs = inputLayout;
		inputLayoutDesc.NumElements = 13;
		graphicsPipeline->SetInputLayout(inputLayoutDesc);
		graphicsPipeline->Commit(*device);
	}
	FILE* file = nullptr;
	if (fileName.rfind(L"pmx") == -1)
	{
		assert(0);		//	�g���q���Ⴄ�恙
		return;
	}
	_wfopen_s(&file, fileName.data(), L"rb");
	if (!file)
	{
		assert(0);		//	�t�@�C���J���Ȃ������恙
		return;

	}

	std::vector<unsigned char> loadMode;
	//	�w�b�_�̓ǂݍ��݂��恙
	{
		static const unsigned int SIGNATURE_NUM = 4;
		char signature[SIGNATURE_NUM] = {};
		char pmx[SIGNATURE_NUM] = { 'P','M','X',' '};
		fread(signature, sizeof(signature), 1, file);
		if (!(signature[0] == pmx[0] && signature[1] == pmx[1] && signature[2] == pmx[2] && signature[3] == pmx[3]))
		{
			assert(0);
			return;
		}

		float version = 0.0f;						//	�t�@�C���̃o�[�W�������恙
		fread(&version, sizeof(version), 1, file);	//	�o�[�W�������̓ǂݍ��݂��恙
		if (!(1.0f < version && version <= 2.1f))
		{
			assert(0);
			return;
		}

		int headNum = 0;
		fread(&headNum, sizeof(unsigned char), 1, file);
		loadMode.resize(headNum);
		fread(loadMode.data(), sizeof(unsigned char), loadMode.size(), file);
	}

	//	���f�����̃X�L�b�v
	for (int i = 0; i < 4; ++i)								//	���f����,���f�����p,�R�����g,�R�����g�p�̏��ɃX�L�b�v����恙
	{
		int bufSize = 0;									//	�e�L�X�g�f�[�^�̃T�C�Y
		fread(&bufSize, sizeof(int), 1, file);				//	�e�L�X�g�f�[�^�̃T�C�Y��ǂݍ��ނ恙
		std::wstring txt;									//	�e�L�X�g�{�����恙
		txt.resize(bufSize);
		fread(txt.data(), sizeof(char), bufSize, file);		//	�e�L�X�g�̓ǂݍ��݂��恙;
	}

	std::vector<VerticesData> vertices;
	//	���_�̓ǂݍ��݂��恙
	{
		int vertexNum = 0;
		fread(&vertexNum, sizeof(int), 1, file);
		vertices.resize(vertexNum);
		for (VerticesData& vertex : vertices)
		{
			fread(&vertex.pos, sizeof(DirectX::XMFLOAT3), 1, file);
			fread(&vertex.normal, sizeof(DirectX::XMFLOAT3), 1, file);
			fread(&vertex.uv, sizeof(DirectX::XMFLOAT2), 1, file);
			for (int i = 0; i < loadMode[static_cast<int>(LoadModeIndexs::addUvNum)]; ++i)
			{
				fread(&vertex.addUv[i], sizeof(DirectX::XMFLOAT4), 1, file);
			}
			fread(&vertex.weightType, sizeof(char), 1, file);
			switch (vertex.weightType)
			{
				case 0:
					fread(&vertex.boneIndex[0], loadMode[static_cast<int>(LoadModeIndexs::boneIndexSize)], 1, file);
					vertex.boneIndex[1] = 0;
					vertex.boneIndex[2] = 0;
					vertex.boneIndex[3] = 0;
					vertex.boneWeight[0] = 1.0f;
					vertex.boneWeight[1] = 0;
					vertex.boneWeight[2] = 0;
					vertex.boneWeight[3] = 0;
					vertex.sdef_C = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
					vertex.sdef_R0 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
					vertex.sdef_R1 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
					break;
				case 1:
					fread(&vertex.boneIndex[0], loadMode[static_cast<int>(LoadModeIndexs::boneIndexSize)], 1, file);
					fread(&vertex.boneIndex[1], loadMode[static_cast<int>(LoadModeIndexs::boneIndexSize)], 1, file);
					vertex.boneIndex[2] = 0;
					vertex.boneIndex[3] = 0;
					fread(&vertex.boneWeight[0], sizeof(float), 1, file);
					vertex.boneWeight[1] = 1.0f - vertex.boneWeight[0];
					vertex.boneWeight[2] = 0;
					vertex.boneWeight[3] = 0;
					vertex.sdef_C = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
					vertex.sdef_R0 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
					vertex.sdef_R1 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
					break;
				case 2:
					fread(&vertex.boneIndex[0], loadMode[static_cast<int>(LoadModeIndexs::boneIndexSize)], 1, file);
					fread(&vertex.boneIndex[1], loadMode[static_cast<int>(LoadModeIndexs::boneIndexSize)], 1, file);
					fread(&vertex.boneIndex[2], loadMode[static_cast<int>(LoadModeIndexs::boneIndexSize)], 1, file);
					fread(&vertex.boneIndex[3], loadMode[static_cast<int>(LoadModeIndexs::boneIndexSize)], 1, file);
					fread(&vertex.boneWeight[0], sizeof(float), 1, file);
					fread(&vertex.boneWeight[1], sizeof(float), 1, file);
					fread(&vertex.boneWeight[2], sizeof(float), 1, file);
					fread(&vertex.boneWeight[3], sizeof(float), 1, file);
					vertex.sdef_C = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
					vertex.sdef_R0 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
					vertex.sdef_R1 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
					break;
				case 3:
					fread(&vertex.boneIndex[0], loadMode[static_cast<int>(LoadModeIndexs::boneIndexSize)], 1, file);
					fread(&vertex.boneIndex[1], loadMode[static_cast<int>(LoadModeIndexs::boneIndexSize)], 1, file);
					vertex.boneIndex[2] = 0;
					vertex.boneIndex[3] = 0;
					fread(&vertex.boneWeight[0], sizeof(float), 1, file);
					vertex.boneWeight[1] = 1.0f - vertex.boneWeight[0];
					vertex.boneWeight[2] = 0;
					vertex.boneWeight[3] = 0;
					fread(&vertex.sdef_C, sizeof(DirectX::XMFLOAT3), 1, file);
					fread(&vertex.sdef_R1, sizeof(DirectX::XMFLOAT3), 1, file);
					fread(&vertex.sdef_R0, sizeof(DirectX::XMFLOAT3), 1, file);
					break;
				default:
					assert(0);		//	�ُ�Ȑ��l���恙
					break;
			}
			fread(&vertex.toonEdgeScale, sizeof(float), 1, file);
		}
	}

	std::vector<unsigned int> indexes;
	//	�C���f�b�N�X�f�[�^�̓ǂݍ��݂��恙
	{
		int indexNum = 0;
		fread(&indexNum, sizeof(int), 1, file);
		indexes.resize(indexNum);
		for(unsigned int& index : indexes)
		{
			fread(&index, loadMode[static_cast<int>(LoadModeIndexs::vertexIndexSize)], 1, file);
		}
	}

	//	�o�b�t�@�[�̍쐬��
	verticesBuffer.CreateBuffer(device, vertices, indexes);

	//	�e�N�X�`���̓ǂݍ��݂��恙
	int textureNum = 0;													//	�e�N�X�`���̐����恙	
	fread(&textureNum, sizeof(int), 1, file);
	std::vector<std::wstring> texturePass(textureNum);					//	�e�N�X�`�������݂ւ̃p�X�̔z�񂾂恙
	for (int i = 0; i < textureNum; ++i)
	{
		int bufSize = 0;												//	�e�L�X�g�f�[�^�̃T�C�Y
		fread(&bufSize, sizeof(int), 1, file);							//	�e�L�X�g�f�[�^�̃T�C�Y��ǂݍ��ނ恙
		texturePass[i].resize(bufSize);
		fread(texturePass[i].data(), sizeof(char), bufSize, file);		//	�e�L�X�g�f�[�^�̓ǂݍ��݂��恙
	}

	//	�}�e���A���̓ǂݍ��݂��恙
	{
		int materialNum = 0;
		fread(&materialNum, sizeof(int), 1, file);						//	�}�e���A�����̓ǂݍ��݂��恙
		unsigned int materialIndexOffset = 0;
		for (int i = 0; i < materialNum; ++i)
		{
			std::vector<BlobConstBuffer::DataType> bufferData;			//	�f�[�^�\�����w�肷��z�񂾂恙

			//	�}�e���A���̖��O�̎擾������恙
			int bufSize = 0;
			fread(&bufSize, sizeof(int), 1, file);
			std::wstring name;
			name.resize(bufSize);
			fread(name.data(), sizeof(char), bufSize, file);
			std::string eName;
			fread(&bufSize, sizeof(int), 1, file);
			eName.resize(bufSize);
			fread(eName.data(), sizeof(char), bufSize, file);



			DirectX::XMFLOAT4 diffuse;
			fread(&diffuse, sizeof(DirectX::XMFLOAT4), 1, file);
			bufferData.emplace_back(BlobConstBuffer::DataType::Float4);

			DirectX::XMFLOAT4 specularColor;
			fread(&specularColor, sizeof(DirectX::XMFLOAT4), 1, file);
			bufferData.emplace_back(BlobConstBuffer::DataType::Float4);

			DirectX::XMFLOAT3 ambient;
			fread(&ambient, sizeof(DirectX::XMFLOAT3), 1, file);
			bufferData.emplace_back(BlobConstBuffer::DataType::Float3);

			unsigned char drawFlag = 0;
			fread(&drawFlag, sizeof(unsigned char), 1, file);
			bufferData.emplace_back(BlobConstBuffer::DataType::Int);

			DirectX::XMFLOAT4 edgeColor;
			fread(&edgeColor, sizeof(DirectX::XMFLOAT4), 1, file);
			bufferData.emplace_back(BlobConstBuffer::DataType::Float4);

			float edgeSize = 0;
			fread(&edgeSize, sizeof(float), 1, file);
			bufferData.emplace_back(BlobConstBuffer::DataType::Float);

			//	�萔�o�b�t�@�̍쐬��
			BlobConstBuffer constBuffer(bufferData, device);
			constBuffer.SetParameter(0, diffuse);
			constBuffer.SetParameter(1,specularColor);
			constBuffer.SetParameter(2,ambient);
			constBuffer.SetParameter(3,drawFlag);
			constBuffer.SetParameter(4,edgeColor);
			constBuffer.SetParameter(5,edgeSize);

			int textureIndex = 0;
			fread(&textureIndex, loadMode[static_cast<int>(LoadModeIndexs::textureIndexSize)], 1, file);
			std::wstring textureName;
			if (textureIndex == 255)
			{
				textureName = L"���e�N�X�`��";
			}
			else
			{
				textureName = fileName.erase((fileName.rfind(L'/')) + 1, fileName.size() - fileName.rfind(L'/')) + texturePass[textureIndex];
			}


			int spaIndex = 0;
			fread(&spaIndex, loadMode[static_cast<int>(LoadModeIndexs::textureIndexSize)], 1, file);

			int mode = 0;
			fread(&mode, sizeof(char), 1, file);

			bool toonFlag = true;
			fread(&toonFlag, sizeof(char), 1, file);
			std::wstring toonTexture;
			if (toonFlag)
			{
				int toon = 0;
				fread(&toon, sizeof(char), 1, file);
				toonTexture.resize(37);
				if (toon < 10)
				{
					swprintf_s(toonTexture.data(), 37, L"Resource/Texture/PmdTex/toon%02d.bmp", toon + 1);
				}
				else
				{
					toonTexture = L"���e�N�X�`��";
				};
			}
			else
			{
				int toonIndex = 0;
				fread(&toonIndex, loadMode[static_cast<int>(LoadModeIndexs::textureIndexSize)], 1, file);
				if (toonIndex == 255)
				{
					toonTexture = L"���e�N�X�`��";
				}
				else
				{
					toonTexture = fileName.erase((fileName.rfind(L'/')) + 1, fileName.size() - fileName.rfind(L'/')) + texturePass[toonIndex];
				}
			}

			//	�����̃X�L�b�v���恙
			fread(&bufSize, sizeof(int), 1, file);
			std::wstring memo;
			memo.resize(bufSize);
			fread(memo.data(), sizeof(char), bufSize, file);

			int materialIndexNum = 0;
			fread(&materialIndexNum, sizeof(int), 1, file);					//	���̃}�e���A�����g�p���钸�_�̓ǂݍ��݂��恙



			//	�}�e���A���`��̎d�������聙
			std::vector<std::shared_ptr<RenderCommand::RenderCommandBase>> renderCommands;
			RenderCommand::RenderCommandFactory::CreateSetRootSignature(renderCommands, rootSignature.Get());
			RenderCommand::RenderCommandFactory::CreateSetPipelineState(renderCommands, graphicsPipeline->GetPipelineState());
			RenderCommand::RenderCommandFactory::CreateSetPrimitiveTopology(renderCommands, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			RenderCommand::RenderCommandFactory::CreateSetVertexBuffer(renderCommands, 0, 1, verticesBuffer.GetVertexBufferView());
			RenderCommand::RenderCommandFactory::CreateSetIndexBuffer(renderCommands, verticesBuffer.GetIndexBufferView());
			RenderCommand::RenderCommandFactory::CreateSetDescriptorHeap(renderCommands);
			RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 0, Camera::GetInstance()->GetDescriptorOffset());
			RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 1, descriptorOffset);
			RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 2, constBuffer.GetDescriptorOffset());
			RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 3, textureBuffer.TextureAcquisition(textureName));
			RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 4, textureBuffer.TextureAcquisition(L"���e�N�X�`��"));
			RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 5, textureBuffer.TextureAcquisition(L"���e�N�X�`��"));
			RenderCommand::RenderCommandFactory::CreateSetDescriptor(renderCommands, 6, textureBuffer.TextureAcquisition(toonTexture));
			std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resources;
			resources.emplace_back(verticesBuffer.GetConstVertexBuffer());
			resources.emplace_back(verticesBuffer.GetConstIndexBuffer());
			resources.emplace_back(Camera::GetInstance()->GetConstConstBuffer());
			resources.emplace_back(resource);
			resources.emplace_back(constBuffer.GetBuffer());
			resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(textureName)));
			resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"���e�N�X�`��")));
			resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"���e�N�X�`��")));
			resources.emplace_back(textureBuffer.GetTextureBuffer(textureBuffer.TextureAcquisition(L"Resource/Texture/PmdTex/toon10.bmp")));
			RenderCommand::RenderCommandQueue pmdRenderCommandQueue(renderCommands, resources);

			//	�萔�z����쐬��
			std::vector<BlobConstBuffer> constbuffers;
			constbuffers.push_back(constBuffer);

			//	�}�e���A����K�p���郁�b�V���f�[�^�̍쐬���s���恙
			materialMeshs.emplace_back(materialIndexNum, materialIndexOffset);
			materialIndexOffset += materialIndexNum;
			materialMeshs.back().GetMaterial(MaterialTag::Normal).CreateData(pmdRenderCommandQueue, constbuffers);
		}
	}

	//	�{�[���̓ǂݍ��݂��恙
	{
		int boneNum = 0;
		fread(&boneNum, sizeof(int), 1, file);						//	�}�e���A�����̓ǂݍ��݂��恙
		std::unordered_map<std::wstring, Bone>& boneDataMap = boneData.GetData();
		struct Load
		{
			std::wstring name;
			int parent;
		};
		std::vector<Load> loadData;
		loadData.resize(boneNum);
		for (int i = 0; i < boneNum; ++i)
		{
			int bufSize = 0;	//	������p�̃o�b�t�@���恙
			fread(&bufSize, sizeof(int), 1, file);
			bufSize /= 2;		//	���C�h������p�ɃT�C�Y��2����1�Ɂ�
			loadData[i].name.resize(bufSize);
			fread(loadData[i].name.data(), sizeof(wchar_t), bufSize, file);
			Bone* loadBoneData = &boneDataMap[loadData[i].name];
			loadBoneData->SetIndex(i);

			//	�{�[���̉p�ꖼ�̓ǂݍ��݂��恙
			fread(&bufSize, sizeof(int), 1, file);
			std::wstring eName;	//	�{�[�������恙
			bufSize /= 2;
			eName.resize(bufSize);
			fread(eName.data(), sizeof(wchar_t), bufSize, file);

			DirectX::XMFLOAT3 pos(0,0,0);
			fread(&pos, sizeof(DirectX::XMFLOAT3), 1, file);
			loadBoneData->SetPosition(pos);

			fread(&loadData[i].parent, loadMode[5], 1, file);

			{
				int tmp; // �g��Ȃ��f�[�^���恙
				fread(&tmp, sizeof(int), 1, file);
				tmp;
			}

			int bitFlag = 0;
			fread(&bitFlag, 2, 1, file);
			if (bitFlag & 0x0001)
			{
				int tmp;
				fread(&tmp, loadMode[5], 1, file);
			}
			else
			{
				DirectX::XMFLOAT3 tmp;
				fread(&tmp, sizeof(DirectX::XMFLOAT3), 1, file);
			}

			if (bitFlag & 0x0020)
			{
				int targetIndex = 0;
				fread(&targetIndex, loadMode[5], 1, file);
				int loop = 0;
				fread(&loop, sizeof(int), 1, file);
				float angle;
				fread(&angle, sizeof(float), 1, file);
				int num = 0;
				fread(&num, sizeof(int), 1, file);
				for(int j = 0; j < num; ++j)
				{
					int bIndex = 0;
					fread(&bIndex, loadMode[5], 1, file);
					bool flag;
					fread(&flag, 1, 1, file);
					if (flag)
					{
						DirectX::XMFLOAT3 tmp;
						fread(&tmp, sizeof(DirectX::XMFLOAT3), 1, file);
						fread(&tmp, sizeof(DirectX::XMFLOAT3), 1, file);
					}
				}
			}
		}

		//	�e�{�[���̃����N��
		for (int i = 0; i < boneNum; ++i)
		{
			int parent = loadData[i].parent;
			if (parent > boneNum)
			{
				continue;
			}
			boneDataMap[loadData[parent].name].PushChildren(&boneDataMap[loadData[i].name]);
		}
	}

	fclose(file);
}