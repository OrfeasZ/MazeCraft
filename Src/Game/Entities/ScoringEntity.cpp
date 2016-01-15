#include "ScoringEntity.h"

using namespace Game::Entities;

ScoringEntity::ScoringEntity(uint32_t p_Size) :
	m_Size(p_Size),
	m_Moves(0),
	m_Hammers(0)
{
}

ScoringEntity::~ScoringEntity()
{
}

void ScoringEntity::Init()
{
}

void ScoringEntity::Update(double p_Delta)
{
}

void ScoringEntity::AddMove()
{
	++m_Moves;
}

void ScoringEntity::AddHammer()
{
	++m_Hammers;
}

void ScoringEntity::ResetScore()
{
	m_Hammers = 0;
	m_Moves = 0;
}

int64_t ScoringEntity::GetScore()
{
	return (m_Size * m_Size) - (m_Moves * 10) - (m_Hammers * 50);
}