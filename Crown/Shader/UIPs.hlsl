Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

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

float4 main(Output input) : SV_TARGET
{
	return tex.Sample(smp, input.uv) * float4(color, alpha);
	//return float4(1,1,1, 1);
}