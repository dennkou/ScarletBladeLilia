#include"EnemyHpUi.hlsli"

struct EnemyHpUiGsOutput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(EnemyHpUiGsOutput input) : SV_TARGET
{
	const float2 flameSize = float2(0.05f, 0.2);
	
	float4 f = float4(0, 0, 0, 1);
	float4 hp = lerp(float4(0, 0, 0, 0.5), float4(0.8, 0, 0, 1), step(input.uv.x, hpPercent));
	return lerp(hp, f, (step(input.uv.x, flameSize.x) + step(input.uv.y, flameSize.y) + step(1 - input.uv.x, flameSize.x) + step(1 - input.uv.y, flameSize.y)));
}