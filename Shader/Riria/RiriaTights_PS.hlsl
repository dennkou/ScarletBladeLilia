struct Output
{
	float4 pos : POSITION;
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 vnormal : NORMAL1;
	float2 uv : TEXCOORD;
	float3 ray : VECTOR; //ベクトル
};

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
SamplerState smpToon : register(s1);

cbuffer cameraBuffer : register(b0)
{
	matrix view;
	matrix projection;
	matrix viewProjection;
	float3 eye;
};

cbuffer materialBuffer : register(b2)
{
	float4 diffuse;
	float4 specular; //スペキュラ
	float3 ambient; //アンビエント
};

float HALF_PI = 1.5707963267948966192313216916398;

float random(float2 uv)
{
	return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}

struct PSOutput
{
	float4 color : SV_TARGET;
	float4 normal : SV_TARGET1;
};

PSOutput main(Output input)
{
	float4 skinColor = float4(0.898, 0.706, 0.592, 1);
	float4 tightsColor = float4(0.373, 0.286, 0.235, 1);
	
	float min = -1.0f;
	float max = 0.3f;
	
	float t = lerp(min, max, dot(input.vnormal.xyz, float3(0, 0, -1)));
	
	PSOutput ret;

	ret.color = lerp(tightsColor, skinColor, t);
	ret.normal = input.normal;
	ret.normal.w = 1.0f;
	
	return ret;
}