#include "pch.h"
#include "CMarvelBaseCharacter.h"
#include "Game/Marvel.h"

void CMarvelBaseCharacter::Finalize()
{
	CPawn::Finalize();

	if (IsLocallyControlled())
		Marvel::SetLocalTeamID(m_TeamID);
}

const bool CMarvelBaseCharacter::IsFriendly() const
{
	return Marvel::IsFriendly(m_TeamID);
}
