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

float4 main(Output input) : SV_TARGET
{
	float3 light = normalize(float3(1, -1, 1)); //���̌������x�N�g��(���s����)

	//�f�B�t���[�Y�v�Z
	float diffuseB = saturate(dot(-light, input.normal.xyz));

	//���̔��˃x�N�g��
	float3 refLight = normalize(reflect(light, input.vnormal.xyz));
	float specularB = (saturate(dot(refLight, input.ray)) + 1) / 2;
	
	return lerp(tex.Sample(smp, input.uv), float4(1.0f, 1.0f, 1.0f, 1.0f), step(0.6, specularB));
}