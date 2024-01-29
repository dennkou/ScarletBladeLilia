#include "UI.hlsli"

UIData data : register(b0);

float4 main(float4 pos : POSITION) : SV_POSITION
{
	return pos + float4(data.pos, 0, 0);
}