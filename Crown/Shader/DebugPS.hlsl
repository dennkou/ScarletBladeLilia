cbuffer materialBuffer : register(b0)
{
	float4 color;
	matrix player;
};

cbuffer cameraBuffer : register(b1)
{
	matrix view;
	matrix projection;
	matrix viewProjection;
	float3 eye;
};

float4 main() : SV_TARGET
{
	return color;
}