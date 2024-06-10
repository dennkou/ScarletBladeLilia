#pragma once
#ifndef CROWN_RENDEROBJECT_CAMERA
#define CROWN_RENDEROBJECT_CAMERA
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include "RenderCommands/SetDescriptor.h"
#include "DirectX12Wraps/ResourceUploader.h"
#include "RenderCommands/RenderCommandQueue.h"
#include "./BlobConstBuffer/BlobConstBuffer.h"
namespace Crown
{
	namespace RenderObject
	{
		//================================================
		// 作成日 : 2023/6/28
		// 作成者 : 服部颯澄
		// 
		// カメラの制御を行うクラスだよ☆
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
			inline DirectX::XMMATRIX GetViewProjection() const noexcept { return m_cpuSideCameraData.viewProjection; }
			void SetEye(const DirectX::XMFLOAT3& eye) noexcept;
			inline DirectX::XMFLOAT3 GetEye() const noexcept { return m_cpuSideCameraData.eye; }
			void SetRotate(const DirectX::XMFLOAT3& rotate) noexcept;
			inline DirectX::XMFLOAT3 GetRotate() const noexcept { return m_cpuSideCameraData.rotate; }
			inline float GetFovAngle() const noexcept { return m_cpuSideCameraData.fovAngle; }
			void SetFovAngle(float fovAngle) noexcept { m_cpuSideCameraData.fovAngle = fovAngle; 	DataUpload(); }
			inline float GetNear() const noexcept { return m_cpuSideCameraData.nearZ; }
			inline float GetFar() const noexcept { return m_cpuSideCameraData.farZ; }

			inline DirectX::XMFLOAT3 GetGazePoint() const noexcept { return m_gazePoint; }
			inline void SetGazePoint(DirectX::XMFLOAT3 gazePoint) noexcept { m_gazePoint = gazePoint; }

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
				DirectX::XMMATRIX view;							//	ビュー行列だよ☆
				DirectX::XMMATRIX projection;					//	プロジェクション行列だよ☆
				DirectX::XMMATRIX viewProjection;				//	ビュー行列とプロジェクション行列だよ☆
				DirectX::XMFLOAT3 eye;							//	カメラ位置だよ☆
				float pad;
				DirectX::XMFLOAT3 rotate;						//	カメラの回転角だよ☆
				float pad1;
				float fovAngle;									//	カメラの画角だよ☆
				float aspect;									//	アスペクト比だよ☆
				float nearZ;									//	ニアー面だよ☆
				float farZ;										//	ファー面だよ☆
				CameraData() 
					:
					fovAngle(DirectX::XMConvertToRadians(60)),
					aspect(1280.0f / 720.0f),
					nearZ(1.0f),
					farZ(10000.0f),
					view(DirectX::XMMatrixIdentity()),
					projection(DirectX::XMMatrixIdentity()),
					viewProjection(view * projection),
					eye(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
					rotate(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f))
				{
				}
			};

			static Camera* me;

			CameraData m_cpuSideCameraData;
			DirectX::XMFLOAT3 m_gazePoint;

			Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;	//	コンストバッファだよ☆
			unsigned int m_descriptorOffset;					//	ディスクリプタオフセットだよ☆
		};
	}
}

#endif // !CROWN_RENDEROBJECT_CAMERA