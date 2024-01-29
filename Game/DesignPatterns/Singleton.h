#pragma once

#include <mutex>
#include <memory>
#include <functional>

// =====================================================================
// 
// �`�`�`�`�`�@�@�@���@�g�����@���@�@�@�`�`�`�`�`
// 
// 
// 
// class �V���O���g���ɂ������N���X : protected Singleton<�V���O���g���ɂ������N���X>
// {
// public:
//	~�V���O���g���ɂ������N���X();
// static �V���O���g���ɂ������N���X& GetInstance() { return GetInstance([](){ return new �V���O���g���ɂ������N���X; }) }
// private:
//	�V���O���g���ɂ������N���X();
// 
// }
// �ŃN���X���V���O���g�����ł���恙
// 
// 
// 
// =====================================================================

namespace Crown
{
	//	�e���v���[�g�Ŏw�聕�p���ŃV���O���g��������恙
	template<class T>
	class Singleton
	{
	protected:
		/// <summary>
		/// �V���O���g���̗B��̎��̂�Ԃ��恙
		/// </summary>
		/// <param name="create">
		/// : �V���O���g���̐����֐���
		/// </param>
		/// <returns></returns>
		static T& GetInstance(std::function<T* (void)> create)
		{
			std::call_once(m_once, Create, create);
			return *singleton.get();
		}

		/// <summary>
		/// �V���O���g�����폜����恙
		/// </summary>
		static void OptionalDeletion()
		{
			singleton.reset(nullptr);
		}

		Singleton()
		{

		}
		virtual ~Singleton()
		{

		}

		static void Create(std::function<T* (void)> create)
		{
			singleton.reset(create());
		}
	private:
		Singleton(Singleton& singleton) = delete;
		virtual Singleton& operator=(Singleton& singleton) = delete;
		Singleton(Singleton&& singleton) = delete;
		virtual Singleton& operator=(Singleton&& singleton) = delete;

		static std::unique_ptr<T> singleton;
		static std::once_flag m_once;
	};

	template<class T>
	std::unique_ptr<T> Singleton<T>::singleton = nullptr;
	template<class T>
	std::once_flag Singleton<T>::m_once;
}