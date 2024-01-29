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


cbuffer materialBuffer : register(b2)
{
	float4 diffuse;
	float4 specular; //スペキュラ
	float3 ambient; //アンビエント
};

static const float HALF_PI = 1.5707963267948966192313216916398;



float random(float2 uv)
{
	return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}



float4 main(Output input) : SV_TARGET
{
	float4 ret = float4(0, 0, 0, 1);
	
	float3 light = normalize(float3(1, -1, 1)); //光の向かうベクトル(平行光線)

	//ディフューズ計算
	float diffuseLight = (dot(-light, input.normal.xyz) + 1) / 2;
	
	float3 refLight = normalize(reflect(light, input.vnormal.xyz));	//光の反射ベクトル
	float specularB = (dot(refLight, input.ray) + 1) / 2;
	
	float4 color = tex.Sample(smp, input.uv);	//	テクスチャカラーだよ☆
	
	ret = color * float4(lerp(ambient, diffuse.rbg, diffuseLight), 1);
	
	return ret;

}