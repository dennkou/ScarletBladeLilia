#pragma once
#ifndef CROWN_RENDEROBJECT_MATERIALTAG
#define CROWN_RENDEROBJECT_MATERIALTAG
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// 作成日 : 2023/6/28
		// 作成者 : 服部颯澄
		// 
		// マテリアルの種別を示すタグだよ☆
		// 
		//================================================
		enum class MaterialTag
		{
			Invalid,		//	無効なタグ情報☆
			Normal,			//	通常☆
			Shadow,			//	影用だよ☆

			Num				//	タグの数だよ☆
		};
	}
}
#endif // !CROWN_RENDEROBJECT_MATERIALTAG