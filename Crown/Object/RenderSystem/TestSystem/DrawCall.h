#pragma once
#include <d3d12.h>
#include <wrl.h>
/// <summary>
/// �`��Ăяo�����恙
/// </summary>
class DrawCall
{
public:
	DrawCall();
	~DrawCall();


private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;

};