#pragma once
#ifndef CROWN_RENDEROBJECT_MATERIALTAG
#define CROWN_RENDEROBJECT_MATERIALTAG
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// �쐬�� : 2023/6/28
		// �쐬�� : �����D��
		// 
		// �}�e���A���̎�ʂ������^�O���恙
		// 
		//================================================
		enum class MaterialTag
		{
			Invalid,		//	�����ȃ^�O���
			Normal,			//	�ʏ큙
			Shadow,			//	�e�p���恙

			Num				//	�^�O�̐����恙
		};
	}
}
#endif // !CROWN_RENDEROBJECT_MATERIALTAG