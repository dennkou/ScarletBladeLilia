#include "ShadowHeader.hlsli"

Texture2D<float4> colorMap		: register(t0);
Texture2D<float4> normalMap		: register(t1);
Texture2D<float> depthMap		: register(t2);
Texture2D<float> shadowMap		: register(t3);

SamplerState smp : register(s0);
//SamplerState shadowSmp : register(s1);

cbuffer cameraBuffer : register(b0)
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
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4x4 Inverse(float4x4 m)
{
	float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
	float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
	float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
	float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

	float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
	float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
	float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
	float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

	float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
	float idet = 1.0f / det;

	float4x4 ret;

	ret[0][0] = t11 * idet;
	ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
	ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
	ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

	ret[1][0] = t12 * idet;
	ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
	ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
	ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

	ret[2][0] = t13 * idet;
	ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
	ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
	ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

	ret[3][0] = t14 * idet;
	ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
	ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
	ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

	return ret;
}

float4 Color(float2 uv)
{
	//ディフューズ計算
	float diffuseLight = (dot(-light, normalMap.Sample(smp, uv).xyz) + 1) / 2;
	float4 screenPos = float4(((uv + float2(-0.5f, -0.5f)) * float2(2.0f, -2.0f)), depthMap.Sample(smp, uv), 1);
	float4 world = mul(Inverse(view), mul(Inverse(projection), screenPos));
	world.xyz = world.xyz / world.w;
	world.w = 1.0f;
	
	
	
	float4 shadowPos = mul(shadowMatrix, world);
	//return world;
	//return (pointMap.Sample(smp, input.uv) + float4(eye, 0));
	return colorMap.Sample(smp, uv) * float4(lerp(float3(0.6, 0.6, 0.6), float3(1, 1, 1), diffuseLight), 1) * Shadow(shadowPos, shadowMap);
	//return colorMap.Sample(smp, input.uv) * lerp(1.0, float4(0.7, 0.7, 0.7, 1), step(shadowMap.Sample(shadowSmp, shadowUV), shadowPos.z - 0.0001));
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