#pragma once
#include <d3d12.h>
#include "./../TextureBuffer.h"
namespace Crown
{
	namespace RenderObject
	{
		class Ui
		{
		public:
			Ui();
			virtual ~Ui();

			virtual void Render(ID3D12GraphicsCommandList* commandList) = 0;

			virtual int GetPriority() const noexcept final { return m_priority; }
			virtual void SetPriority(int priority) noexcept final;
		private:
			int m_priority;		//	このUIオブジェクトの優先度だよ☆
		};
	}
}