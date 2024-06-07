#include "RootSignature.h"

Crown::RenderObject::RootSignature::RootSignature()
{
}

Crown::RenderObject::RootSignature::~RootSignature()
{
}

void Crown::RenderObject::RootSignature::Create(ID3D12Device* device)
{
	//	ディスクリプタレンジの設定だよ☆
	D3D12_DESCRIPTOR_RANGE descriptorRange[CBV_NUM + SRV_NUM] = {};

	//	定数
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	for (int i = 0; i < CBV_NUM; ++i)
	{
		descriptorRange[i] = descriptorRange[0];
		descriptorRange[i].BaseShaderRegister = i;
	}

	//	テクスチャ
	descriptorRange[CBV_NUM] = descriptorRange[0];
	descriptorRange[CBV_NUM].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[CBV_NUM].BaseShaderRegister = 0;
	for (int i = 0; i < SRV_NUM; ++i)
	{
		descriptorRange[i + CBV_NUM] = descriptorRange[CBV_NUM];
		descriptorRange[i + CBV_NUM].BaseShaderRegister = i;
	}

	//	ルートパラメータの作成を行うよ☆
	static const unsigned int parameterNum = CBV_NUM + SRV_NUM;
	D3D12_ROOT_PARAMETER rootParameter[parameterNum] = {};
	//	カメラ情報だよ☆
	rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameter[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0];
	rootParameter[0].DescriptorTable.NumDescriptorRanges = 1;
	for (int i = 0; i < CBV_NUM + SRV_NUM; ++i)
	{
		rootParameter[i] = rootParameter[0];
		rootParameter[i].DescriptorTable.pDescriptorRanges = &descriptorRange[i];
	}

	//	サンプラーの設定だよ☆
	const unsigned int samplerNum = 3;
	D3D12_STATIC_SAMPLER_DESC samplerDesc[samplerNum] = {};
	samplerDesc[0].Filter = D3D12_FILTER_ANISOTROPIC;
	//samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].MaxAnisotropy = 16;
	samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc[0].MinLOD = 0.0f;
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc[0].ShaderRegister = 0;
	samplerDesc[1] = samplerDesc[0];//変更点以外をコピー
	samplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].ShaderRegister = 1;
	samplerDesc[2] = samplerDesc[0];//変更点以外をコピー
	samplerDesc[2].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc[2].ComparisonFunc = D3D12_COMPARISON_FUNC_GREATER;
	samplerDesc[2].MaxAnisotropy = 1;
	samplerDesc[2].AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc[2].AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc[2].AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerDesc[2].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	samplerDesc[2].ShaderRegister = 2;

	//	ルートシグネチャーの設定だよ☆
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParameter;
	rootSignatureDesc.NumParameters = parameterNum;
	rootSignatureDesc.pStaticSamplers = samplerDesc;
	rootSignatureDesc.NumStaticSamplers = samplerNum;

	//	ルートシグネチャーを作成するよ☆
	ID3DBlob* rootSigBlob = nullptr;
	D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, nullptr);
	device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));	//	ルートシグネチャーを作成するよ☆
	rootSigBlob->Release();
}