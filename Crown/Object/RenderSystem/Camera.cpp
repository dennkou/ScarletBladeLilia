#include "Camera.h"
#include "DirectX12Wraps/ResourceUploader.h"
#include "RenderCommands/RenderCommandFactory.h"
#include "DirectX12Wraps/DescriptorHeaps.h"
#include <d3dx12.h>
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"

Crown::RenderObject::Camera* Crown::RenderObject::Camera::me = nullptr;

Crown::RenderObject::Camera::Camera()
	:
	m_descriptorOffset(0)
{

}

Crown::RenderObject::Camera::~Camera()
{

}

void Crown::RenderObject::Camera::DataUpload() noexcept
{
	me->m_cpuSideCameraData.view = 
		DirectX::XMMatrixRotationX(me->m_cpuSideCameraData.rotate.x)
		* DirectX::XMMatrixRotationY(me->m_cpuSideCameraData.rotate.y)
		* DirectX::XMMatrixRotationZ(me->m_cpuSideCameraData.rotate.z)
		* DirectX::XMMatrixTranslation(me->m_cpuSideCameraData.eye.x, me->m_cpuSideCameraData.eye.y, me->m_cpuSideCameraData.eye.z);
	me->m_cpuSideCameraData.view = DirectX::XMMatrixInverse(nullptr, me->m_cpuSideCameraData.view);
	me->m_cpuSideCameraData.projection = DirectX::XMMatrixPerspectiveFovLH(me->m_cpuSideCameraData.fovAngle, me->m_cpuSideCameraData.aspect, me->m_cpuSideCameraData.nearZ, me->m_cpuSideCameraData.farZ);
	me->m_cpuSideCameraData.viewProjection = me->m_cpuSideCameraData.view * me->m_cpuSideCameraData.projection;
	ResourceUploader::GetInstance()->UploadData<CameraData>(m_resource, ResourceUploader::GetInstance()->Get255AlignmentSize<CameraData>(1), [&](CameraData* gpuSideCameraData) { *gpuSideCameraData = m_cpuSideCameraData; });
}

void Crown::RenderObject::Camera::CreateInstance(ID3D12Device* device)
{
	if (me == nullptr)
	{
		me = new Camera();

		D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ResourceUploader::GetInstance()->Get255AlignmentSize<CameraData>());
		device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&me->m_resource));

		//	ディスクリプタの作成を行うよ☆
		D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};
		constantBufferViewDesc.BufferLocation = me->m_resource->GetGPUVirtualAddress();
		constantBufferViewDesc.SizeInBytes = static_cast<UINT>(ResourceUploader::GetInstance()->Get255AlignmentSize<CameraData>());
		me->m_descriptorOffset = DescriptorHeaps::GetInstance().CreateConstantBufferView(constantBufferViewDesc);

		//	カメラを原点から
		me->m_cpuSideCameraData.eye = DirectX::XMFLOAT3(0,-10,20);
		me->m_cpuSideCameraData.rotate = DirectX::XMFLOAT3(0, 0, 0);
		me->DataUpload();
	}
}

Crown::RenderObject::Camera* Crown::RenderObject::Camera::GetInstance()
{
	assert(me);
	return me;
}

void Crown::RenderObject::Camera::DelteInstance()
{
	delete me;
}

void Crown::RenderObject::Camera::SetEye(const DirectX::XMFLOAT3& eye) noexcept
{
	m_cpuSideCameraData.eye = eye;
	DataUpload();
}

void Crown::RenderObject::Camera::SetRotate(const DirectX::XMFLOAT3& rotate) noexcept
{
	m_cpuSideCameraData.rotate = rotate;
	DataUpload();
}