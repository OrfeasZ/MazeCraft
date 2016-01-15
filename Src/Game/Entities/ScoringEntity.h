#pragma once

#include "IEntity.h"

namespace Game
{
	namespace Entities
	{
		class ScoringEntity :
			public IEntity
		{
		public:
			ScoringEntity(uint32_t p_Size);
			virtual ~ScoringEntity() override;

		public:
			virtual void Init() override;
			virtual void Update(double p_Delta) override;

		public:
			void AddMove();
			void AddHammer();
			void ResetScore();
			int64_t GetScore();

		protected:
			uint32_t m_Size;
			uint64_t m_Moves;
			uint64_t m_Hammers;
		};
	}
}