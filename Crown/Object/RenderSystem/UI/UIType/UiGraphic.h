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

			inline void SetPosition(const DirectX::XMFLOAT2& pos) { m_constBuffer->SetParameter(POSITION_OFFSET, pos); }
			inline DirectX::XMFLOAT2& GetPosition() { return *m_constBuffer->GetParameter<DirectX::XMFLOAT2>(POSITION_OFFSET); }
			inline void SetSize(const DirectX::XMFLOAT2& size) { m_constBuffer->SetParameter(SIZE_OFFSET, size); }
			inline DirectX::XMFLOAT2& GetSize() {  return *m_constBuffer->GetParameter<DirectX::XMFLOAT2>(SIZE_OFFSET); }
			inline void SetRotate(float rotate) { m_constBuffer->SetParameter(ROTATE_OFFSET, rotate); }
			inline float GetRotate() { return *m_constBuffer->GetParameter<float>(ROTATE_OFFSET); }
			inline void SetStrip(DirectX::XMFLOAT4& strip) { m_constBuffer->SetParameter(STRIP_OFFSET, strip); }
			inline DirectX::XMFLOAT4& GetStrip() { return *m_constBuffer->GetParameter<DirectX::XMFLOAT4>(STRIP_OFFSET); }
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
			
			BlobConstBuffer* m_constBuffer;
			static constexpr unsigned int POSITION_OFFSET = 0;
			static constexpr unsigned int SIZE_OFFSET = 1;
			static constexpr unsigned int ROTATE_OFFSET = 2;
			static constexpr unsigned int STRIP_OFFSET = 3;
			static constexpr unsigned int COLOR_OFFSET = 4;
			static constexpr unsigned int ALPHA_OFFSET = 5;
			
			unsigned int m_descriptorOffset;							//	ディスクリプタオフセットだよ☆
			Microsoft::WRL::ComPtr<ID3D12Resource> m_textureResource;	//	テクスチャリソースのシェアポインタ☆
		};
	}
}