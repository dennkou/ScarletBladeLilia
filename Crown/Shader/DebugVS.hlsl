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

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return mul(viewProjection, mul(player, pos));
}