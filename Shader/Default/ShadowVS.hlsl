cbuffer light : register(b0)
{
	matrix light;
}

cbuffer modelBuffer : register(b1)
{
	matrix world;
	matrix bones[255];
};

float4 main(float4 pos : POSITION, min16uint4 boneIndex : BONE_INDEX, float4 weight : WEIGHT) : SV_POSITION
{
	matrix boneMatrix = bones[boneIndex[0]] * weight[0] + bones[boneIndex[1]] * weight[1] + bones[boneIndex[2]] * weight[2] + bones[boneIndex[3]] * weight[3];

	return mul(light, mul(world, mul(boneMatrix, pos))); //	ï`âÊà íuÅô
}