struct Output
{
	float4 pos : POSITION;
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 vnormal : NORMAL1;
	float2 uv : TEXCOORD;
	float3 ray : VECTOR; //�x�N�g��
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
	float4 specular; //�X�y�L����
	float3 ambient; //�A���r�G���g
};



Output main(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, min16uint4 boneIndex : BONE_INDEX, float4 weight : WEIGHT)
{
	matrix boneMatrix = bones[boneIndex[0]] * weight[0] + bones[boneIndex[1]] * weight[1] + bones[boneIndex[2]] * weight[2] + bones[boneIndex[3]] * weight[3];
	
	float4 localPos = mul(boneMatrix, pos);
	
	Output output;													//	�s�N�Z���V�F�[�_�֓n���l���恙
	output.pos = mul(mul(view, world), localPos);					//	�J���������_�Ƃ������W���恙
	output.svpos = mul(projection, output.pos);						//	�`��ʒu��
	normal.w = 0;													//	���s�ړ������𖳌��ɂ���恙
	output.normal = normalize(mul(mul(world, boneMatrix), normal));	//	�@���Ƀ��[���h�ϊ����s���恙
	output.vnormal = mul(view, output.normal);						//	��������̖@�����恙
	output.uv = uv;													//	UV�l�ɂ͕ω��Ȃ���
	output.ray = normalize(output.pos.xyz);							//	�����x�N�g���̎Z�o��
	return output;
}