#pragma once
#include "./../../Crown/Object/RenderSystem/Model/Model.h"
class Sky
{
public:
	Sky();
	~Sky();
private:
	Crown::RenderObject::Model m_model;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
};