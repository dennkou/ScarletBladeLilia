struct Output
{
	float4 pos : POSITION;
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 vnormal : NORMAL1;
	float2 uv : TEXCOORD;
	float3 ray : VECTOR; //ベクトル
};

cbuffer cameraBuffer : register(b0)
{
	matrix camaraMatrix;
	matrix shadowMatrix;
	float3 light;
};

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct PSOutput
{
	float4 color : SV_TARGET;
	float4 normal : SV_TARGET1;
};

PSOutput main(Output input)
{
	PSOutput output;
	
	//ディフューズ計算
	float diffuseB = saturate(dot(-light, input.normal.xyz));

	//光の反射ベクトル
	float3 refLight = normalize(reflect(light, input.vnormal.xyz));
	float specularB = (saturate(dot(refLight, input.ray)) + 1) / 2;
	
	output.color = tex.Sample(smp, input.uv);
	output.normal = input.normal;
	output.normal.w = 1.0f;
	
	return output;
}