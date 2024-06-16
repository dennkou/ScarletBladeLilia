#pragma once
#include "./../../Crown/Object/RenderSystem/UI/Ui.h"
#include <DirectXMath.h>
#include "./../../Crown/Object/RenderSystem/BlobConstBuffer/BlobConstBuffer.h"

class EnemyUi : public Crown::RenderObject::Ui
{
public:
	EnemyUi();
	virtual ~EnemyUi();

	virtual void Render(ID3D12GraphicsCommandList* commandList) override;

	void SetPositionOffset(DirectX::XMFLOAT3 positionOffset);

	void SetHPPercent(float hpPercent);
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetDrawFlag(bool flag);
private:
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	Crown::RenderObject::BlobConstBuffer* m_constBuffer;
	DirectX::XMFLOAT3 m_positionOffset;

	std::vector<Crown::RenderObject::BlobConstBuffer::DataType> BUFFER_DATA
	{
		Crown::RenderObject::BlobConstBuffer::DataType::Matrix,
		Crown::RenderObject::BlobConstBuffer::DataType::Float3,
		Crown::RenderObject::BlobConstBuffer::DataType::Float,
		Crown::RenderObject::BlobConstBuffer::DataType::Float,
		Crown::RenderObject::BlobConstBuffer::DataType::Float2,
		Crown::RenderObject::BlobConstBuffer::DataType::Float3,
		Crown::RenderObject::BlobConstBuffer::DataType::Float3,
		Crown::RenderObject::BlobConstBuffer::DataType::Float3
	};

	enum class ConstBufferOffset : unsigned int
	{
		VIEW_PROJECTION_OFFSET,
		POSITION_OFFSET,
		HP_PERCENT_OFFSET,
		DRAW_FLAG,
		SIZE_OFFSET,
		COLOR_OFFSET,
		FLAME_COLOR_OFFSET,
		BACKGROUND_COLOR_OFFSET,
	};

};