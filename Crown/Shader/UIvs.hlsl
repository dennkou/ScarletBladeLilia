
cbuffer UIData : register(b0)
{
	matrix conversion;
	float3 color;
	float alpha;
};

struct Output
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

Output main(float4 pos : POSITION, float2 uv : UV)
{
	Output output;
	output.pos = mul(conversion, pos);
	output.uv = uv;
	return output;
}