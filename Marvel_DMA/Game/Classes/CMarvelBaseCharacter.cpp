#include "pch.h"
#include "CMarvelBaseCharacter.h"
#include "Game/Marvel.h"

void CMarvelBaseCharacter::Finalize()
{
	CPawn::Finalize();
}

const bool CMarvelBaseCharacter::IsFriendly() const
{
	return m_TeamID == Marvel::m_LocalTeamID;
}
