cbuffer material : register(b2)
{
	float4 color;
}

float4 main(float4 pos : SV_POSITION) : SV_TARGET
{
	return float4(0.7, 0.7, 0.7, 1);
}