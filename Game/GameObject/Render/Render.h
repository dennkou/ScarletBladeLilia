#pragma once
#include "./../GameObject.h"
#include "./../../Crown/Object/RenderSystem/RenderTarget.h"
#include "./../../Crown/Object/RenderSystem/UI/UIType/UiGraphic.h"
#include <memory>

class Render : public Game::GameObject
{
public:
	Render(Game* game);
	virtual ~Render() override;

	void OnGameUpdate(Timer& timer) override;

	static Crown::RenderObject::GraphicsPipeline* GetDefaultGraphicsPipeline() { return shadowMapGraphicsPipeline; }

	static Crown::RenderObject::GraphicsPipeline* GetShadowMapGraphicsPipeline() { return shadowMapGraphicsPipeline; }
	static Crown::RenderObject::BlobConstBuffer* GetShadowMapBuffer() { return shadowMapBuffer; }
	static unsigned int GetShadowMapColorBufferIndex() { return shadowMap->GetTexture(); }
	static unsigned int GetShadowMapDepthBufferIndex() { return shadowMap->GetDepth(); }
private:
	static std::shared_ptr<Crown::RenderObject::RenderTarget> shadowMap;
	static Crown::RenderObject::GraphicsPipeline* shadowMapGraphicsPipeline;
	static Crown::RenderObject::BlobConstBuffer* shadowMapBuffer;
	Crown::RenderObject::UiGraphic m_debug;
};