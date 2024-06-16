cbuffer materialBuffer : register(b0)
{
	float hp_percent;
	float inclination;
	float3 color;
	float3 flameColor;
	float3 backgroundColor;
	float2 size;
	float2 position;
	float alpha;
};

float4 main(float4 pos : SV_POSITION, float2 uv : UV) : SV_TARGET
{
	float3 outputColor = float3(0,1,1);
	
	float2 FlameSize = float2(0.01, 0.15);
	
	int isInFlame = uv.x < FlameSize.x ||
					uv.x > 1.0 - FlameSize.x ||
					uv.y < FlameSize.y ||
					uv.y > 1.0 - FlameSize.y;
	
	outputColor = lerp(lerp(color, backgroundColor, uv.x > hp_percent), flameColor, isInFlame);
	
	return float4(outputColor, alpha);
}