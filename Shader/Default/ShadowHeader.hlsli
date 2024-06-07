
SamplerState shadowSmp : register(s1);
//SamplerComparisonState shadowSmp : register(s1);


float4 Shadow(float4 shadowPos, Texture2D<float> shadowDepth)
{
	float3 shadowSamplePos = shadowPos.xyz / shadowPos.w;
	float2 shadowUV = (shadowSamplePos.xy + float2(1, -1)) * float2(0.5, -0.5);
	
	//float shadow = shadowDepth.SampleCmpLevelZero(shadowSmp, shadowUV, shadowSamplePos.z - 0.001f);
	//return lerp(1.0, float4(0.7, 0.7, 0.7, 1), shadow);
	return lerp(1.0, float4(0.7, 0.7, 0.7, 1), step(shadowDepth.Sample(shadowSmp, shadowUV) + 0.0001, shadowSamplePos.z));
}