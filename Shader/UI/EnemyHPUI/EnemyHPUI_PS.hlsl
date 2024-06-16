cbuffer materialBuffer : register(b0)
{
	matrix ViewProjection;
	float3 Position;
	float hp_percent;
	float drawFlag;
	float2 Size;
	float3 color;
	float3 backgroundColor;
	float3 flameColor;
};

float4 main(float4 pos : SV_POSITION, float2 uv : UV, float front : DrawFlag) : SV_TARGET
{
	float3 outputColor = float3(0, 1, 1);
	
	float2 FlameSize = float2(0.01, 0.15);
	
	int isInFlame = uv.x < FlameSize.x ||
					uv.x > 1.0 - FlameSize.x ||
					uv.y < FlameSize.y ||
					uv.y > 1.0 - FlameSize.y;
	
	outputColor = lerp(lerp(float3(0.6,0.0,0.0), backgroundColor, uv.x > hp_percent), flameColor, isInFlame);
	
	return lerp(float4(0, 0, 0, 0), float4(outputColor, 0.8), drawFlag * front);
}