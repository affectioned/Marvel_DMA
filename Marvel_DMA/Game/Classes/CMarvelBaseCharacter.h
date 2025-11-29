#pragma once
#include "CActor.h"
#include "SDK/Marvel_classes.hpp"

class CMarvelBaseCharacter : public CActor
{
public:
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CActor::PrepareRead_1(vmsh);
	}
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
	{
		CActor::PrepareRead_2(vmsh);
	}
	void Finalize()
	{
		CActor::Finalize();
	}
};