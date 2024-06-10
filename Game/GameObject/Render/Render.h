#pragma once
#include "./../GameObject.h"
#include "./../../Crown/Object/RenderSystem/RenderTarget/RenderTarget.h"
#include "./../../Crown/Object/RenderSystem/RenderTarget/MultipleRenderTarget.h"
#include "./../../Crown/Object/RenderSystem/UI/UIType/UiGraphic.h"
#include <memory>
#include "./FrameRender.h"
#include "PostEffects.h"

class Render : public Game::GameObject
{
public:
	Render(Game* game);
	virtual ~Render() override;

	void OnGameUpdate(Timer& timer) override;

	static Crown::RenderObject::GraphicsPipeline* GetDefaultGraphicsPipeline() { return shadowMapGraphicsPipeline; }

	static const Crown::RenderObject::GraphicsPipeline* const GetShadowMapGraphicsPipeline() { return shadowMapGraphicsPipeline; }
	static Crown::RenderObject::BlobConstBuffer* GetShadowMapBuffer() { return shadowMapBuffer; }
	static unsigned int GetShadowMapColorBufferIndex() { return shadowMap->GetTexture(); }
	static unsigned int GetShadowMapDepthBufferIndex() { return shadowMap->GetDepth(); }
private:
	static Crown::RenderObject::GraphicsPipeline* shadowMapGraphicsPipeline;
	static std::shared_ptr<Crown::RenderObject::RenderTarget> shadowMap;
	static Crown::RenderObject::BlobConstBuffer* shadowMapBuffer;

	std::shared_ptr<Crown::RenderObject::MultipleRenderTarget> m_frameBuffer;
	FrameRender m_integrationPath;

	std::unique_ptr<PostEffects> m_blurShadowMap;
#ifdef _DEBUG
	Crown::RenderObject::UiGraphic m_debug;
#endif // _DEBUG
	Crown::RenderObject::Model m_debugModel;

};