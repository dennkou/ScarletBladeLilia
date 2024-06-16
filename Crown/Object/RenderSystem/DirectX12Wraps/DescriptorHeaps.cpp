#include "DescriptorHeaps.h"
#include "d3dx12.h"
#include <assert.h>



Crown::RenderObject::DescriptorHeaps* Crown::RenderObject::DescriptorHeaps::me = nullptr;



Crown::RenderObject::DescriptorHeaps::DescriptorHeaps()
	:
	m_device(nullptr),
	m_cpuDescriptorHeap(nullptr),
	m_descriptorHeap(),
	DESCRIPTOR_HANDLE_INCREMENT_SIZE(0),
	m_index(0),
	m_descriptorNum(0),
	m_maxDescriptorNum(0),
	m_changeSize(false)
{

}

Crown::RenderObject::DescriptorHeaps::~DescriptorHeaps()
{

}

void Crown::RenderObject::DescriptorHeaps::CreateDescriptorHeaps(ID3D12Device* device, unsigned int heapsSize)
{
	if (!me)
	{
		me = new DescriptorHeaps();
		assert(heapsSize > 0);
		assert(device);

		me->m_device = device;
		me->m_maxDescriptorNum = heapsSize;
		const_cast<UINT&>(me->DESCRIPTOR_HANDLE_INCREMENT_SIZE) = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		me->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_FLAG_NONE, me->m_maxDescriptorNum, &me->m_cpuDescriptorHeap);
		me->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, me->m_maxDescriptorNum, &me->m_descriptorHeap[0]);
		me->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, me->m_maxDescriptorNum, &me->m_descriptorHeap[1]);
	}
}

Crown::RenderObject::DescriptorHeaps& Crown::RenderObject::DescriptorHeaps::GetInstance()
{
	assert(me);
	return *me;
}

void Crown::RenderObject::DescriptorHeaps::DeleteDescriptorHeaps()
{
	if (me)
	{
		delete me;
	}
}

unsigned int Crown::RenderObject::DescriptorHeaps::CreateConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& constantBufferViewDesc)
{
	assert(m_device);
	assert(m_descriptorHeap[m_index]);

	D3D12_CPU_DESCRIPTOR_HANDLE createHandle = CreateDescriptor();
	m_device->CreateConstantBufferView(&constantBufferViewDesc, createHandle);
	UploadDescriptorHeap();
	return m_descriptorNum - 1;
}

unsigned int Crown::RenderObject::DescriptorHeaps::CreateShaderResourceView(ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC& shaderResourceViewDesc)
{
	assert(m_device);
	assert(m_descriptorHeap[m_index]);

	D3D12_CPU_DESCRIPTOR_HANDLE createHandle = CreateDescriptor();
	m_device->CreateShaderResourceView(resource, &shaderResourceViewDesc, createHandle);
	UploadDescriptorHeap();
	return m_descriptorNum - 1;
}

D3D12_GPU_DESCRIPTOR_HANDLE Crown::RenderObject::DescriptorHeaps::GetHandle(unsigned int m_offset) const
{
	D3D12_GPU_DESCRIPTOR_HANDLE ret = GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	ret.ptr += DESCRIPTOR_HANDLE_INCREMENT_SIZE * m_offset;
	return ret;
}



D3D12_CPU_DESCRIPTOR_HANDLE Crown::RenderObject::DescriptorHeaps::CreateDescriptor()
{
	assert(m_cpuDescriptorHeap.Get());

	//	�V�����n���h���̒l���v�Z��
	D3D12_CPU_DESCRIPTOR_HANDLE ret = m_cpuDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	ret.ptr += DESCRIPTOR_HANDLE_INCREMENT_SIZE * m_descriptorNum;
	++m_descriptorNum;

	//	�����q�[�v�̈悪����Ȃ��Ȃ�����E�E�E��
	if (m_descriptorNum > m_maxDescriptorNum)
	{
		//	�f�B�X�N���v�^�q�[�v��V������������恙
		ID3D12DescriptorHeap* newCpuDescriptorHeap = nullptr;
		unsigned int nextNum = m_maxDescriptorNum + (m_maxDescriptorNum / 3);
		//	nextNum���K����������悤�Ɂ�
		if (nextNum <= m_maxDescriptorNum)
		{
			nextNum = m_maxDescriptorNum + 1;
		}

		//	CPU���̃f�B�X�N���v�^�q�[�v�̍Đ�����
		{
			CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_FLAG_NONE, nextNum, &newCpuDescriptorHeap);

			//	�R�s�[���s���恙
			m_device->CopyDescriptorsSimple(m_maxDescriptorNum, newCpuDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_cpuDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			//	CPU�f�B�X�N���v�^�q�[�v��������
			*m_cpuDescriptorHeap.ReleaseAndGetAddressOf() = newCpuDescriptorHeap;
		}

		//	GPU���̃f�B�X�N���v�^�q�[�v�̍Đ������恙
		{
			//	���̃C���f�b�N�X���擾��
			int nextIndex = m_index;
			++nextIndex;
			nextIndex %= 2;

			//	�f�B�X�N���v�^�q�[�v���Đ�����
			CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, nextNum, m_descriptorHeap[nextIndex].ReleaseAndGetAddressOf());

			//	�R�s�[���s���恙
			m_device->CopyDescriptorsSimple(m_maxDescriptorNum, m_descriptorHeap[nextIndex]->GetCPUDescriptorHandleForHeapStart(), m_cpuDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		
			m_index = nextIndex;
		}
		m_maxDescriptorNum = nextNum;
		m_changeSize = true;

		ret = m_cpuDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		ret.ptr += DESCRIPTOR_HANDLE_INCREMENT_SIZE * (m_descriptorNum - 1);
	}
	return ret;
}

void Crown::RenderObject::DescriptorHeaps::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_FLAGS flags, UINT NumDescriptors, ID3D12DescriptorHeap** descriptorHeap)
{
	//	�Ăяo���Ă������m�F����
	assert(m_device);
	assert(descriptorHeap);
	assert(NumDescriptors);

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = flags;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = NumDescriptors;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(descriptorHeap));//������
}

void Crown::RenderObject::DescriptorHeaps::UploadDescriptorHeap()
{
	//	�Ăяo���Ă������m�F����
	assert(m_device);
	assert(m_cpuDescriptorHeap);
	assert(m_descriptorHeap[0].Get());
	assert(m_descriptorHeap[1].Get());

	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptor = m_cpuDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	cpuDescriptor.ptr += DESCRIPTOR_HANDLE_INCREMENT_SIZE * (m_descriptorNum - 1);

	D3D12_CPU_DESCRIPTOR_HANDLE gpuDescriptor = m_descriptorHeap[m_index]->GetCPUDescriptorHandleForHeapStart();
	gpuDescriptor.ptr += DESCRIPTOR_HANDLE_INCREMENT_SIZE * (m_descriptorNum - 1);

	//	�R�s�[���s���恙
	m_device->CopyDescriptorsSimple(1, gpuDescriptor, cpuDescriptor, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}