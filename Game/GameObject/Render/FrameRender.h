#pragma once
#include "./../../Crown/Object/RenderSystem/UI/Ui.h"
#include "./../../Crown/Object/RenderSystem/DirectX12Wraps/GraphicPipeline.h"
#include "./../../Crown/Object/RenderSystem/BlobConstBuffer/BlobConstBuffer.h"
#include <DirectXMath.h>
class FrameRender : Crown::RenderObject::Ui
{
public:
	FrameRender();
	~FrameRender() override;

	virtual void Render(ID3D12GraphicsCommandList* commandList) override;

	inline void SetColorBuffer(unsigned int colorBuffer) noexcept { m_colorBuffer = colorBuffer; }
	inline void SetNormalBuffer(unsigned int normalBuffer) noexcept { m_normalBuffer = normalBuffer; }
	inline void SetDepthBuffer(unsigned int depthBuffer) noexcept { m_depthBuffer = depthBuffer; }
	inline void SetShadowBuffer(unsigned int shadowBuffer) noexcept { m_shadowBuffer = shadowBuffer; }
	inline void SetCameraBuffer(unsigned int cameraBuffer) noexcept { m_camera = cameraBuffer; }
	inline void SetShadow(DirectX::XMMATRIX shadow) { m_constBuffer->SetParameter(static_cast<unsigned int>(Parameter::Shadow), shadow); }
	inline void SetLightVector(DirectX::XMFLOAT3 vec) { m_constBuffer->SetParameter(static_cast<unsigned int>(Parameter::Light), vec); }
private:
	//	定数バッファ用オフセット位置だよ☆
	enum class Parameter
	{
		Shadow,
		Light,
	};


	Crown::RenderObject::GraphicsPipeline m_graphicsPipeline;
	Crown::RenderObject::BlobConstBuffer* m_constBuffer;

	unsigned int m_colorBuffer;
	unsigned int m_normalBuffer;
	unsigned int m_depthBuffer;
	unsigned int m_shadowBuffer;
	unsigned int m_camera;
};

