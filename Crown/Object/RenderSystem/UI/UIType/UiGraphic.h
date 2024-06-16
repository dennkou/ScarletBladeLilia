#pragma once
#include "./../Ui.h"
#include <wrl.h>
#include <string>
#include "./../../BlobConstBuffer/BlobConstBuffer.h"
#include <DirectXMath.h>
namespace Crown
{
	namespace RenderObject
	{
		class UiGraphic : public Ui
		{
		public:
			UiGraphic();
			UiGraphic(std::wstring texturePath);
			virtual ~UiGraphic();

			void Load(std::wstring texturePath);

			virtual void Render(ID3D12GraphicsCommandList* commandList) override;

			inline void SetPosition(DirectX::XMFLOAT2 position) noexcept { m_pos = position; ConstBufferUpdate(); }
			inline DirectX::XMFLOAT2 GetPosition() const noexcept { return m_pos; }
			inline void SetSize(DirectX::XMFLOAT2 size) noexcept { m_size = size; ConstBufferUpdate(); }
			inline DirectX::XMFLOAT2 GetSize() const noexcept { return m_size; }
			inline void SetColor(DirectX::XMFLOAT3& color) { m_constBuffer->SetParameter(COLOR_OFFSET, color); }
			inline DirectX::XMFLOAT3 GetColor() { return *m_constBuffer->GetParameter<DirectX::XMFLOAT3>(COLOR_OFFSET); }
			inline void SetAlpha(float alpha) { m_constBuffer->SetParameter(ALPHA_OFFSET, alpha); }
			inline float GetAlpha() { return *m_constBuffer->GetParameter<float>(ALPHA_OFFSET); }
			inline void SetTextureDescriptorOffset(unsigned int descriptorOffset) { m_descriptorOffset = descriptorOffset; }

			inline operator bool() const noexcept { return m_descriptorOffset; }
		private:
			static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
			void CreatPipelineState();
			void CreatConstBuffer();
			void ConstBufferUpdate();
			
			BlobConstBuffer* m_constBuffer;
			static constexpr unsigned int MATRIX_OFFSET = 0;
			static constexpr unsigned int COLOR_OFFSET = 1;
			static constexpr unsigned int ALPHA_OFFSET = 2;

			DirectX::XMFLOAT2 m_pos;
			DirectX::XMFLOAT2 m_size;
			
			unsigned int m_descriptorOffset;							//	ディスクリプタオフセットだよ☆
			Microsoft::WRL::ComPtr<ID3D12Resource> m_textureResource;	//	テクスチャリソースのシェアポインタ☆
		};
	}
}