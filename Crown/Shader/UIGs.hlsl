#include "UI.hlsli"

UIData data : register(b0);

struct Output
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

[maxvertexcount(4)]
void main(point float4 input[1] : SV_POSITION, inout TriangleStream<Output> output)
{
	float ca = cos(data.rotate);
	float sa = sin(data.rotate);

  // Ry-Rx-Rz (Yaw Pitch Roll)
	matrix roll = float4x4(
		ca, -sa, 0, 0,
		sa,  ca, 0, 0,
		 0,   0, 1, 0,
		 0,   0, 0, 1
  );
	
	Output element;
	
	// ����
	element.pos = input[0] + mul(float4(-data.size.x, data.size.y, 0, 0), roll);
	element.uv.x = data.strip.x;
	element.uv.y = data.strip.z;
	output.Append(element);
	
	// �E��
	element.pos = input[0] + mul(float4(data.size.x, data.size.y, 0, 0), roll);
	element.uv.x = data.strip.y;
	element.uv.y = data.strip.z;
	output.Append(element);
	
	// ����
	element.pos = input[0] + mul(float4(-data.size.x, -data.size.y, 0, 0), roll);
	element.uv.x = data.strip.x;
	element.uv.y = data.strip.w;
	output.Append(element);
	
	// �E��
	element.pos = input[0] + mul(float4(data.size.x, -data.size.y, 0, 0), roll);
	element.uv.x = data.strip.y;
	element.uv.y = data.strip.w;
	output.Append(element);

	
	output.RestartStrip();
}