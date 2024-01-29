#include"EnemyHpUi.hlsli"

struct Output
{
	float4 pos : POSITION;
	float2 size : SIZE;
};

Output main(float4 position : POSITION)
{
	Output output;
	output.pos = mul(view, float4(pos,1));
	output.size = float2(0.1, 0.025) * l;
	return output;
}