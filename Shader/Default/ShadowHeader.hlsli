
SamplerState shadowSmp : register(s1);
//SamplerComparisonState shadowSmp : register(s1);

float IsIn(float val)
{
	return step(0.01f, val) - step(0.99f, val);
}

float4 Shadow(float4 shadowPos, Texture2D<float> shadowDepth)
{
	float3 shadowSamplePos = shadowPos.xyz / shadowPos.w;
	float2 shadowUV = (shadowSamplePos.xy + float2(1, -1)) * float2(0.5, -0.5);
	
	bool t = step(shadowDepth.Sample(shadowSmp, shadowUV) + 0.0001, shadowSamplePos.z) * IsIn(shadowUV.x) * IsIn(shadowUV.y) * IsIn(shadowSamplePos.z);

	float4 ret = lerp(1.0, float4(0.7, 0.7, 0.7, 1), t);
	
	
	
	//float shadow = shadowDepth.SampleCmpLevelZero(shadowSmp, shadowUV, shadowSamplePos.z - 0.001f);
	//return lerp(1.0, float4(0.7, 0.7, 0.7, 1), shadow);
	return ret;
}