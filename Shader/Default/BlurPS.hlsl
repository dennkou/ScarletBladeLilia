

Texture2D<float4> targetMap : register(t0);


float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}