struct Output
{
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

cbuffer cameraBuffer : register(b0)
{
	matrix view;
	matrix projection;
	matrix viewProjection;
	float3 eye;
};

cbuffer modelBuffer : register(b1)
{
	matrix world;
	matrix bones[255];
};

cbuffer materialBuffer : register(b2)
{
	float4 diffuse;
	float3 ambient; //�A���r�G���g
};



Output main(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, min16uint4 boneIndex : BONE_INDEX, float4 weight : WEIGHT)
{
	matrix boneMatrix = bones[boneIndex[0]] * weight[0] + bones[boneIndex[1]] * weight[1] + bones[boneIndex[2]] * weight[2] + bones[boneIndex[3]] * weight[3];
	
	float4 localPos = mul(boneMatrix, pos);
	
	Output output; //	�s�N�Z���V�F�[�_�֓n���l���恙
	output.svpos = mul(projection, mul(mul(view, world), localPos)); //	�`��ʒu��
	normal.w = 0;
	output.normal = normalize(mul(mul(world, boneMatrix), normal)); //	�@���Ƀ��[���h�ϊ����s���恙
	output.uv = uv; //	UV�l�ɂ͕ω��Ȃ���
	return output;
}