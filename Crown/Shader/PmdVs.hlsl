#include "Data.hlsli"

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


Output main(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, min16uint2 boneIndex : BONE_INDEX, float weight : WEIGHT)
{
	float weightNormal = (float) weight / 100.0f;
	matrix boneMatrix = bones[boneIndex[0]] * weightNormal + bones[boneIndex[1]] * (1.0f - weightNormal);
		
	Output output;										//	�s�N�Z���V�F�[�_�֓n���l���恙
	pos = mul(boneMatrix, pos);							//	�{�[�����̓K�p��
	pos = mul(world, pos);								//	���f�����W�Ɉړ���
	output.svpos = mul(viewProjection, pos);			//	�`��ʒu��
	output.pos = mul(view, pos);						//	�J���������_�Ƃ������W���恙
	normal.w = 0;										//	�����d�v(���s�ړ������𖳌��ɂ���)
	output.normal = mul(world, normal);					//	�@���Ƀ��[���h�ϊ����s���恙
	output.vnormal = mul(view, output.normal);			//	��������̖@�����恙
	output.uv = uv;										//	UV�l�ɂ͕ω��Ȃ���
	output.ray = normalize(pos.xyz - eye);				//	�����x�N�g���̎Z�o��
	return output;
}