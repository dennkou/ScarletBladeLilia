#include "UI.hlsli"

UIData data : register(b0);
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);


struct Output
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(Output input) : SV_TARGET
{
	return tex.Sample(smp, input.uv) * float4(data.color, data.alpha);
}