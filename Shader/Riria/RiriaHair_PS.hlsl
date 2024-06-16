struct Output
{
	float4 pos : POSITION;
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 vnormal : NORMAL1;
	float2 uv : TEXCOORD;
	float3 ray : VECTOR; //ベクトル
};

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer cameraBuffer : register(b0)
{
	matrix camaraMatrix;
	matrix shadowMatrix;
	float3 light;
};


cbuffer materialBuffer : register(b2)
{
	float4 diffuse;
	float4 specular; //スペキュラ
	float3 ambient; //アンビエント
};

static const float HALF_PI = 1.5707963267948966192313216916398;



struct PSOutput
{
	float4 color : SV_TARGET;
	float4 normal : SV_TARGET1;
};



PSOutput main(Output input)
{
	PSOutput ret;

	//ディフューズ計算
	float diffuseLight = (dot(-light, input.normal.xyz) + 1) / 2;
	
	float3 refLight = normalize(reflect(light, input.vnormal.xyz));	//光の反射ベクトル
	float specularB = (dot(refLight, input.ray) + 1) / 2;
	
	float4 color = tex.Sample(smp, input.uv);	//	テクスチャカラーだよ☆
	
	ret.color = color * float4(lerp(ambient, diffuse.rbg, diffuseLight), 1);
	ret.normal = input.normal;
	ret.normal.w = 1.0f;
	
	return ret;
}