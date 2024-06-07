struct Input
{
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct Output
{
	float4 color : SV_TARGET;
	float4 normal : SV_TARGET1;
};

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);


cbuffer materialBuffer : register(b2)
{
	float4 diffuse;
	float3 ambient; //アンビエント
};

static const float HALF_PI = 1.5707963267948966192313216916398;


Output main(Input input)
{
	Output output;
	output.color = tex.Sample(smp, input.uv);
	output.normal = float4(input.normal, 1.0f);
	return output;
}