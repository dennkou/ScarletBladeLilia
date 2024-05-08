#pragma once
#include "RootSignature.h"

namespace Crown
{
	namespace RenderObject
	{
		class DefaultRootSignature
		{
		public:
			static RootSignature& GetRootSignature();

		private:
			DefaultRootSignature() = default;
			~DefaultRootSignature() = default;
			DefaultRootSignature(DefaultRootSignature& defaultRootSignature) = default;
			DefaultRootSignature& operator=(DefaultRootSignature& defaultRootSignature) = delete;

			class Instance
			{
			public:
				Instance() = default;
				~Instance()
				{
					if (m_defaultRootSignature)
					{
						delete m_defaultRootSignature;
					}
				}

				DefaultRootSignature* GetInstance()
				{
					if (!m_defaultRootSignature)
					{
						m_defaultRootSignature = new DefaultRootSignature();
					}
					return m_defaultRootSignature;
				}
			private:
				DefaultRootSignature* m_defaultRootSignature;
			};

			static Instance instance;
			RootSignature m_RootSignature;
		};


	}
}