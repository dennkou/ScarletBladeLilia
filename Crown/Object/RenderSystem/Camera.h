#pragma once
#ifndef CROWN_RENDEROBJECT_CAMERA
#define CROWN_RENDEROBJECT_CAMERA
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include "RenderCommands/SetDescriptor.h"
#include "DirectX12Wraps/ResourceUploader.h"
#include "RenderCommands/RenderCommandQueue.h"
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// �쐬�� : 2023/6/28
		// �쐬�� : �����D��
		// 
		// �J�����̐�����s���N���X���恙
		// 
		//================================================
		class Camera
		{
		public:
			static void CreateInstance(ID3D12Device* device);
			static Camera* GetInstance();
			static void DelteInstance();

			inline DirectX::XMMATRIX GetView() const noexcept { return m_cpuSideCameraData.view; }
			inline DirectX::XMMATRIX GetProjection() const noexcept { return m_cpuSideCameraData.projection; }
			void SetEye(const DirectX::XMFLOAT3& eye) noexcept;
			inline DirectX::XMFLOAT3 GetEye() const noexcept { return m_cpuSideCameraData.eye; }
			void SetRotate(const DirectX::XMFLOAT3& rotate) noexcept;
			inline DirectX::XMFLOAT3 GetRotate() const noexcept { return m_cpuSideCameraData.rotate; }
			inline float GetFovAngle() const noexcept { return m_cpuSideCameraData.fovAngle; }
			void SetFovAngle(float fovAngle) noexcept { m_cpuSideCameraData.fovAngle = fovAngle; 	DataUpload(); }

			inline unsigned int GetDescriptorOffset() { return m_descriptorOffset; }
			inline const Microsoft::WRL::ComPtr<ID3D12Resource> GetConstConstBuffer() { return m_resource; }
		private:
			Camera();
			~Camera();

			Camera(Camera& camera) = delete;
			Camera& operator=(Camera& camera) = delete;

			void DataUpload() noexcept;

			struct CameraData
			{
				DirectX::XMMATRIX view;							//	�r���[�s�񂾂恙
				DirectX::XMMATRIX projection;					//	�v���W�F�N�V�����s�񂾂恙
				DirectX::XMMATRIX viewProjection;				//	�r���[�s��ƃv���W�F�N�V�����s�񂾂恙
				DirectX::XMFLOAT3 eye;							//	�J�����ʒu���恙
				DirectX::XMFLOAT3 rotate;						//	�J�����̉�]�p���恙
				float fovAngle;									//	�J�����̉�p���恙
				float aspect;									//	�A�X�y�N�g�䂾�恙
				float nearZ;									//	�j�A�[�ʂ��恙
				float farZ;										//	�t�@�[�ʂ��恙
				CameraData() 
					:
					fovAngle(DirectX::XMConvertToRadians(60)),
					aspect(1280.0f / 720.0f),
					nearZ(1.0f),
					farZ(10000.0f),
					view(DirectX::XMMatrixIdentity()),
					projection(DirectX::XMMatrixIdentity()),
					viewProjection(projection * view),
					eye(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
					rotate(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f))
				{
				}
			};

			static Camera* me;

			CameraData m_cpuSideCameraData;

			Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;	//	�R���X�g�o�b�t�@���恙
			unsigned int m_descriptorOffset;					//	�f�B�X�N���v�^�I�t�Z�b�g���恙
		};
	}
}

#endif // !CROWN_RENDEROBJECT_CAMERA