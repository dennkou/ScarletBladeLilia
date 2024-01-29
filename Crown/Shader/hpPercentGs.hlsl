#include"EnemyHpUi.hlsli"

struct EnemyHpUiGsOutput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

struct Input
{
	float4 pos : POSITION;
	float2 size : SIZE;
};



[maxvertexcount(4)]
void main(point Input input[1], inout TriangleStream<EnemyHpUiGsOutput> output)
{
	EnemyHpUiGsOutput element;
	
	//	í∏ì_ÇëùÇ‚Ç∑ÇÊÅô
	element.pos = input[0].pos + float4(-input[0].size, 0, 1);
	element.uv = float2(0, 1);
	output.Append(element);
	
	element.pos = input[0].pos + float4(-input[0].size.x, input[0].size.y, 0, 1);
	element.uv = float2(0, 0);
	output.Append(element);
	
	element.pos = input[0].pos + float4(input[0].size.x, -input[0].size.y, 0, 1);
	element.uv = float2(1, 1);
	output.Append(element);

	element.pos = input[0].pos + float4(input[0].size, 0, 1);
	element.uv = float2(1, 0);
	output.Append(element);
	
	output.RestartStrip();
}