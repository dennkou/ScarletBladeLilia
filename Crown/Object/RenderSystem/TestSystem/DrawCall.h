#pragma once
#include <d3d12.h>
#include <wrl.h>
/// <summary>
/// 描画呼び出しだよ☆
/// </summary>
class DrawCall
{
public:
	DrawCall();
	~DrawCall();


private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;

};