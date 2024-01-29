#pragma once

#include <mutex>
#include <memory>
#include <functional>

// =====================================================================
// 
// ～～～～～　　　☆　使い方　☆　　　～～～～～
// 
// 
// 
// class シングルトンにしたいクラス : protected Singleton<シングルトンにしたいクラス>
// {
// public:
//	~シングルトンにしたいクラス();
// static シングルトンにしたいクラス& GetInstance() { return GetInstance([](){ return new シングルトンにしたいクラス; }) }
// private:
//	シングルトンにしたいクラス();
// 
// }
// でクラスをシングルトン化できるよ☆
// 
// 
// 
// =====================================================================

namespace Crown
{
	//	テンプレートで指定＆継承でシングルトン化するよ☆
	template<class T>
	class Singleton
	{
	protected:
		/// <summary>
		/// シングルトンの唯一の実体を返すよ☆
		/// </summary>
		/// <param name="create">
		/// : シングルトンの生成関数☆
		/// </param>
		/// <returns></returns>
		static T& GetInstance(std::function<T* (void)> create)
		{
			std::call_once(m_once, Create, create);
			return *singleton.get();
		}

		/// <summary>
		/// シングルトンを削除するよ☆
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