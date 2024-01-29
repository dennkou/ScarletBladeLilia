#include "PlayerHpUi.hlsli"
Texture2D<float4> tex : register(t1);
Texture2D<float4> tex2 : register(t0);
SamplerState smp : register(s0);

struct Input
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};



float4 main(Input input) : SV_TARGET
{
	const float2 flameSize = float2(0.01f, 0.2);
	
	float4 frameColor = float4(0, 0, 0, alpha);
	float4 barColor = lerp(float4(0, 0, 0, 0.5 * alpha), float4(0.8 + (0.2 * colorRate), colorRate, colorRate, alpha), step(input.uv.x, hpRate));
	return lerp(barColor, frameColor, (step(input.uv.x, flameSize.x) + step(input.uv.y, flameSize.y) + step(1 - input.uv.x, flameSize.x) + step(1 - input.uv.y, flameSize.y)));
}