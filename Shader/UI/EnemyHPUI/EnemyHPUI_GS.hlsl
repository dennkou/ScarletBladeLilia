cbuffer materialBuffer : register(b0)
{
	matrix ViewProjection;
	float3 Position;
	float HP_Percent;
	float2 Size;
	float3 Color;
	float3 FlameColor;
	float3 backgroundColor;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	bool drawFlag : DrawFlag;
};

[maxvertexcount(4)]
void main(point float4 input[1] : POSITION, inout TriangleStream<GSOutput> output)
{
	GSOutput element;
	
	float4 position = float4(Position, 1.0);
	position = mul(ViewProjection, position);
	
	element.drawFlag = position.w < 0;
	
	position.xyz /= position.w;
	position.w = 1.0f;
	
	element.pos = float4(-Size.x, -Size.y, 0.5, 0) + position;
	element.uv = float2(0, 0);
	output.Append(element);
	
	element.pos = float4(Size.x, -Size.y, 0.5, 0) + position;
	element.uv = float2(1, 0);
	output.Append(element);
	
	element.pos = float4(-Size.x, Size.y, 0.5, 0) + position;
	element.uv = float2(0, 1);
	output.Append(element);
	
	element.pos = float4(Size.x, Size.y, 0.5, 0) + position;
	element.uv = float2(1, 1);
	output.Append(element);
	
	output.RestartStrip();
}