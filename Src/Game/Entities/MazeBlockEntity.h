#pragma once

#include "IEntity.h"

namespace Rendering
{
	namespace Objects
	{
		class Model;
	}
}

namespace Game
{
	namespace Entities
	{
		class MazeBlockEntity :
			public IEntity
		{
		public:
			enum BlockType
			{
				Red,
				Green,
				Blue,
				Texture1,
				Texture2,
				Texture3,
				WarpZone,
				Destroyed,
				Empty
			};

		public:
			MazeBlockEntity(BlockType p_Type, int32_t p_X, int32_t p_Y, int32_t p_Level);
			virtual ~MazeBlockEntity() override;

		public:
			virtual void Init() override;
			virtual void Update(double p_Delta) override;

		public:
			void Transparent(bool p_Transparent);
			void Destroy();

		public:
			inline BlockType Type() const { return m_Type; }
			inline int32_t X() const { return m_X; }
			inline int32_t Y() const { return m_Y; }
			inline int32_t Level() const { return m_Level; }

		protected:
			BlockType m_Type;
			int32_t m_X;
			int32_t m_Y;
			int32_t m_Level;
			Rendering::Objects::Model* m_Model;
			Rendering::Objects::Model* m_ExtraModel01;
			Rendering::Objects::Model* m_ExtraModel02;
		};
	}
}