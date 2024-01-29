#include "Data.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> sph : register(t1);
Texture2D<float4> spa : register(t2);
Texture2D<float4> toon : register(t3);

SamplerState smp : register(s0);
SamplerState smpToon : register(s1);

float4 main(Output input) : SV_TARGET
{
	float3 light = normalize(float3(1, -1, 1)); //���̌������x�N�g��(���s����)

	//�f�B�t���[�Y�v�Z
	float diffuseB = saturate(dot(-light, input.normal.xyz));

	//���̔��˃x�N�g��
	float3 refLight = normalize(reflect(light, input.vnormal.xyz));
	float specularB = pow(saturate(dot(refLight, input.ray)), specular.a);
	
	//�X�t�B�A�}�b�v�pUV
	float2 sphereMapUV = input.vnormal.xy;
	sphereMapUV = (sphereMapUV + float2(1, -1)) * float2(0.5, -0.5);

	float4 texColor = tex.Sample(smp, input.uv); //�e�N�X�`���J���[
	
	return max(saturate(
				toon.Sample(smpToon, float2(0, 1.0 - diffuseB)) //�P�x(�g�D�[��)
				* diffuse //�f�B�t���[�Y�F
				* texColor //�e�N�X�`���J���[
				* sph.Sample(smp, sphereMapUV)) //�X�t�B�A�}�b�v(��Z)
			+ saturate(spa.Sample(smp, sphereMapUV) * texColor //�X�t�B�A�}�b�v(���Z)
			+ float4(specularB * specular.rgb, 1)) //�X�y�L�����[
			, float4(texColor.rgb * ambient, 1)); //�A���r�G���g
}