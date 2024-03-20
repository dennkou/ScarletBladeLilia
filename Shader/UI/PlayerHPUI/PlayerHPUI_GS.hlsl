cbuffer materialBuffer : register(b0)
{
	float HP_Percent;
	float Inclination;
	float3 Color;
	float3 FlameColor;
	float3 backgroundColor;
	float2 Size;
	float2 Position;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

[maxvertexcount(4)]
void main(point float4 input[1] : SV_POSITION, inout TriangleStream<GSOutput> output)
{
	float2 position = Position + float2(-1, -1);
	float2 size = Size * 2;
	
	GSOutput element;
	element.pos = float4(position.x + Inclination, position.y, 0, 1);
	element.uv = float2(0, 0);
	output.Append(element);
	
	element.pos = float4(position.x + size.x + Inclination, position.y, 0, 1);
	element.uv = float2(1, 0);
	output.Append(element);
	
	element.pos = float4(position.x, position.y + size.y, 0, 1);
	element.uv = float2(0, 1);
	output.Append(element);
	
	element.pos = float4(position.x + size.x, position.y + size.y, 0, 1);
	element.uv = float2(1, 1);
	output.Append(element);
	
	output.RestartStrip();
}