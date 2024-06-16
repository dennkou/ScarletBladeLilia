cbuffer materialBuffer : register(b0)
{
	float4 color;
	matrix player;
	matrix viewProjection;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return mul(viewProjection, mul(player, pos));
}