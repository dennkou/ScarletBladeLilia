struct VSOutput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

VSOutput main(float4 pos : POSITION, float2 uv : UV)
{
	VSOutput output;
	
	output.pos = pos;
	output.uv = uv;
	
	return output;
}