struct Output
{
	float4 pos : POSITION;
	float4 svpos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 vnormal : NORMAL1;
	float2 uv : TEXCOORD;
	float3 ray : VECTOR; //�x�N�g��
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
	float4 specular; //�X�y�L����
	float3 ambient; //�A���r�G���g
};

static const float HALF_PI = 1.5707963267948966192313216916398;

struct PSOutput
{
	float4 color : SV_TARGET;
	float4 normal : SV_TARGET1;
};

PSOutput main(Output input)
{
	PSOutput output;
	
	float4 skinColor = lerp(float4(0.898, 0.706, 0.592, 1), 1, 0.0);
	float4 LightColor = lerp(skinColor, float4(0, 0, 0, 1), -0.2);

	//�f�B�t���[�Y�v�Z
	float diffuseLight = (dot(-light, input.normal.xyz) + 1) / 2;
	
	float3 refLight = normalize(reflect(light, input.vnormal.xyz)); //���̔��˃x�N�g��
	float specularB = (dot(refLight, input.ray) + 1) / 2;
	
	output.color = lerp(skinColor, LightColor, max(0, specularB));
	output.normal = input.normal;
	output.normal.w = 1.0f;
	//output.normal = 1.0f;
	
	return output;
}