#pragma once

#include "./../../Crown/Object/RenderSystem/UI/Ui.h"
#include <wrl.h>
#include <string>
#include "./../../Crown/Object/RenderSystem/BlobConstBuffer/BlobConstBuffer.h"
#include <DirectXMath.h>

class PlayerHpUi : public Crown::RenderObject::Ui
{
public:
	PlayerHpUi();
	virtual ~PlayerHpUi();

	void SetAlpha(float alpha);
	void SetPlayerHPPercent(float hpPercent);

	virtual void Render(ID3D12GraphicsCommandList* commandList) override;
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
	Crown::RenderObject::BlobConstBuffer* m_constBuffer;

	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> BUFFER_DATA
	{
		Crown::RenderObject::BlobConstBuffer::DataType::Float,
		Crown::RenderObject::BlobConstBuffer::DataType::Float,
		Crown::RenderObject::BlobConstBuffer::DataType::Float3,
		Crown::RenderObject::BlobConstBuffer::DataType::Float3,
		Crown::RenderObject::BlobConstBuffer::DataType::Float3,
		Crown::RenderObject::BlobConstBuffer::DataType::Float2,
		Crown::RenderObject::BlobConstBuffer::DataType::Float2,
		Crown::RenderObject::BlobConstBuffer::DataType::Float,
	};

	enum class ConstBufferOffset : unsigned int
	{
		HP_PERCENT_OFFSET,
		INCLINATION_OFFSET,
		COLOR_OFFSET,
		FLAME_COLOR_OFFSET,
		BACKGROUND_COLOR_OFFSET,
		SIZE_OFFSET,
		POSITION_OFFSET,
		ALPHA_OFFSET,
	};
};