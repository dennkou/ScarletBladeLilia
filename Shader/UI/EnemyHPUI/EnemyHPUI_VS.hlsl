cbuffer materialBuffer : register(b0)
{
	matrix ViewProjection;
	float3 Position;
	float HP_Percent;
	float3 Color;
	float3 FlameColor;
	float3 backgroundColor;
	float2 Size;
};

float4 main(float4 pos : POSITION) : POSITION
{
	return pos;
}