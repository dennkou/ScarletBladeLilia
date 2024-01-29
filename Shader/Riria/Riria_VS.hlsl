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

cbuffer materialBuffer : register(b2)
{
	float4 diffuse;
	float4 specular; //スペキュラ
	float3 ambient; //アンビエント
};



Output main(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, min16uint4 boneIndex : BONE_INDEX, float4 weight : WEIGHT)
{
	matrix boneMatrix = bones[boneIndex[0]] * weight[0] + bones[boneIndex[1]] * weight[1] + bones[boneIndex[2]] * weight[2] + bones[boneIndex[3]] * weight[3];
	
	float4 localPos = mul(boneMatrix, pos);
	
	Output output;													//	ピクセルシェーダへ渡す値だよ☆
	output.pos = mul(mul(view, world), localPos);					//	カメラを原点とした座標だよ☆
	output.svpos = mul(projection, output.pos);						//	描画位置☆
	normal.w = 0;													//	平行移動成分を無効にするよ☆
	output.normal = normalize(mul(mul(world, boneMatrix), normal));	//	法線にワールド変換を行うよ☆
	output.vnormal = mul(view, output.normal);						//	視線からの法線だよ☆
	output.uv = uv;													//	UV値には変化なし☆
	output.ray = normalize(output.pos.xyz);							//	視線ベクトルの算出☆
	return output;
}