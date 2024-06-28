#include "ShadowHeader.hlsli"

Texture2D<float4> colorMap		: register(t0);
Texture2D<float4> normalMap		: register(t1);
Texture2D<float> depthMap		: register(t2);
Texture2D<float> shadowMap		: register(t3);

SamplerState smp : register(s0);
//SamplerState shadowSmp : register(s1);

cbuffer shadowBuffer : register(b0)
{
	matrix shadowMatrix;
	float3 light;
};

cbuffer cameraBuffer : register(b1)
{
	matrix view;
	matrix projection;
	matrix viewProjection;
	float3 eye;
	float3 rotate;				//	�J�����̉�]�p���恙
	float fovAngle;				//	�J�����̉�p���恙
	float aspect;				//	�A�X�y�N�g�䂾�恙
	float nearZ;				//	�j�A�[�ʂ��恙
	float farZ;					//	�t�@�[�ʂ��恙
	matrix inverseProjection;	//	�r���[�s�񂾂恙
	matrix inverseView;			//	�v���W�F�N�V�����s�񂾂恙
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 Color(float2 uv)
{
	//�f�B�t���[�Y�v�Z
	float diffuseLight = (dot(-normalize(light), normalMap.Sample(smp, uv).xyz) + 1) / 2;
	float4 screenPos = float4(((uv + float2(-0.5f, -0.5f)) * float2(2.0f, -2.0f)), depthMap.Sample(smp, uv), 1);
	float4 world = mul(inverseView, mul(inverseProjection, screenPos));
	world.xyz = world.xyz / world.w;
	world.w = 1.0f;
	
	float4 shadowPos = mul(shadowMatrix, world);
	return colorMap.Sample(smp, uv) * float4(lerp(float3(0.6, 0.6, 0.6), float3(1, 1, 1), diffuseLight), 1) * Shadow(shadowPos, shadowMap);
}

float4 main(VSOutput input) : SV_TARGET
{
	float2 screenSize = float2(1280, 720) * 9.0f;
	
	float4 color = float4(0,0,0,0);
	
	float2 uv = input.uv;
	color += Color(uv);
	
	uv.x += 1.0f / screenSize.x;
	color += Color(uv);
	uv.y += 1.0f / screenSize.y;
	color += Color(uv);
	uv.x -= 1.0f / screenSize.x;
	color += Color(uv);
	uv.x -= 1.0f / screenSize.x;
	color += Color(uv);
	uv.y -= 1.0f / screenSize.y;
	color += Color(uv);
	uv.y -= 1.0f / screenSize.y;
	color += Color(uv);
	uv.x += 1.0f / screenSize.x;
	color += Color(uv);
	uv.x += 1.0f / screenSize.x;
	color += Color(uv);
	
	return color / 9.0f;
}