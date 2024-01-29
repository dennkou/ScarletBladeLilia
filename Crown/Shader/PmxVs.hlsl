#include "Data.hlsli"

cbuffer cameraBuffer : register(b0)
{
	matrix view;
	matrix projection;
	matrix viewProjection;
	float3 eye;
};

cbuffer modelBuffer : register(b1)
{
	matrix world;
	matrix bones[255];
};


Output main(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, min16uint4 boneIndex : BONE_INDEX, float4 weight : WEIGHT)
{
	matrix boneMatrix = bones[boneIndex[0]] * weight[0] + bones[boneIndex[1]] * weight[1] + bones[boneIndex[2]] * weight[2] + bones[boneIndex[3]] * weight[3];
		
	Output output;								//	ピクセルシェーダへ渡す値だよ☆
	pos = mul(boneMatrix, pos);					//	ボーン情報の適用☆
	pos = mul(world, pos);						//	モデル座標に移動☆
	output.svpos = mul(viewProjection, pos);	//	描画位置☆
	output.pos = mul(view, pos);				//	カメラを原点とした座標だよ☆
	normal.w = 0;								//	ここ重要(平行移動成分を無効にする)
	output.normal = mul(world, normal);			//	法線にワールド変換を行うよ☆
	output.vnormal = mul(view, output.normal);	//	視線からの法線だよ☆
	output.uv = uv;								//	UV値には変化なし☆
	output.ray = normalize(pos.xyz - eye);		//	視線ベクトルの算出☆
	return output;
}