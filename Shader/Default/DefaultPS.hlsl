#include "ShadowHeader.hlsli"

struct Output
{
	float4 pos : POSITION;
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 vnormal : NORMAL1;
	float2 uv : TEXCOORD;
	float3 ray : VECTOR; //�x�N�g��
	float4 shadowPos : SHADOW_POSITION;
};

Texture2D<float4> tex : register(t0);
//Texture2D<float4> shadowColor : register(t1);
//Texture2D<float> shadowDepth : register(t2);
SamplerState smp : register(s0);


cbuffer materialBuffer : register(b2)
{
	float4 diffuse;
	float3 ambient; //�A���r�G���g
};

static const float HALF_PI = 1.5707963267948966192313216916398;

float4 main(Output input) : SV_TARGET
{
	float3 light = normalize(float3(1, -1, 1)); //���̌������x�N�g��(���s����)

	//�f�B�t���[�Y�v�Z
	float diffuseLight = (dot(-light, input.normal.xyz) + 1) / 2;
	
	return tex.Sample(smp, input.uv) * float4(lerp(ambient, diffuse.rbg, diffuseLight), 1)/* * Shadow(input.shadowPos, shadowColor, shadowDepth)*/;
}