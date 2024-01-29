#include "PlayerHpUi.hlsli"
struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};



[maxvertexcount(4)]
void main(point float4 input[1] : POSITION, inout TriangleStream<GSOutput> output)
{
	GSOutput element;


	
	element.pos = float4(slope - size.x, -size.y, 0, 0) + input[0];
	element.uv = float2(0, 1);
	output.Append(element);
	
	element.pos = float4(-size.x, size.y, 0, 0) + input[0];
	element.uv = float2(0, 0);
	output.Append(element);
	
	element.pos = float4(slope + size.x, -size.y, 0, 0) + input[0];
	element.uv = float2(1, 1);
	output.Append(element);
	
	element.pos = float4(size.x, size.y, 0, 0) + input[0];
	element.uv = float2(1, 0);
	output.Append(element);
	
	output.RestartStrip();
}