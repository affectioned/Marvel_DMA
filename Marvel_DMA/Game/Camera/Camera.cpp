#include "pch.h"

#include "Camera.h"

#include "Game/Marvel.h"

#include <numbers>
#include <d3d9types.h>

SDK::FMinimalViewInfo Camera::GetViewInfo()
{
	std::scoped_lock lock(m_LocalPlayerMutex);
	return m_ViewInfo;
}

void Camera::FullUpdate(DMA_Connection* Conn)
{
	if (!Marvel::m_LocalPlayerAddress) [[unlikely]]
		return;

	uintptr_t PlayerControllerPtr = Marvel::m_LocalPlayerAddress + offsetof(SDK::ULocalPlayer, PlayerController);
	uintptr_t PlayerControllerAddr = Marvel::RivalsProc.ReadMem<uintptr_t>(Conn, PlayerControllerPtr);

	if (!PlayerControllerAddr) [[unlikely]]
		return;

	uintptr_t PlayerCameraManagerPtr = PlayerControllerAddr + offsetof(SDK::APlayerController, PlayerCameraManager);
	uintptr_t PlayerCameraManagerAddr = Marvel::RivalsProc.ReadMem<uintptr_t>(Conn, PlayerCameraManagerPtr);
	if (!PlayerCameraManagerAddr) [[unlikely]]
		return;

	m_PlayerCameraManagerAddr = PlayerCameraManagerAddr;

	QuickUpdate(Conn);
}

void Camera::QuickUpdate(DMA_Connection* Conn)
{
	if (!Marvel::m_LocalPlayerAddress) [[unlikely]]
		return;

	SDK::FMinimalViewInfo ViewInfo{};
	ViewInfo = Marvel::RivalsProc.ReadMem<SDK::FMinimalViewInfo>(Conn, m_PlayerCameraManagerAddr + offsetof(SDK::APlayerCameraManager, ViewTarget) + offsetof(SDK::FTViewTarget, POV));

	std::scoped_lock lock(m_LocalPlayerMutex);
	m_ViewInfo = ViewInfo;
}

D3DMATRIX Matrix(SDK::FRotator rot, SDK::FVector origin)
{
	double radPitch = (rot.Pitch * double(std::numbers::pi) / 180.f);
	double radYaw = (rot.Yaw * double(std::numbers::pi) / 180.f);
	double radRoll = (rot.Roll * double(std::numbers::pi) / 180.f);

	double SP = sinf(radPitch);
	double CP = cosf(radPitch);
	double SY = sinf(radYaw);
	double CY = cosf(radYaw);
	double SR = sinf(radRoll);
	double CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.X;
	matrix.m[3][1] = origin.Y;
	matrix.m[3][2] = origin.Z;
	matrix.m[3][3] = 1.f;

	return matrix;
}

double DotProduct(SDK::FVector a, SDK::FVector b)
{
	double Return = a.X * b.X
		+ a.Y * b.Y
		+ a.Z * b.Z;

	return Return;
}

bool Camera::WorldToScreen(SDK::FVector& WorldLocation, Vector2& ScreenLocation)
{
	auto ViewInfo = GetViewInfo();

	auto& cameraPOVLocation = ViewInfo.Location;
	auto& cameraPOVRotation = ViewInfo.Rotation;
	const auto& cameraFOV = ViewInfo.FOV;

	auto WindowSize = ImVec2(1920, 1080);
	const double halfScreenWidth = static_cast<double>(WindowSize.x) / 2.0;
	const double halfScreenHeight = static_cast<double>(WindowSize.y) / 2.0;

	auto vDelta = WorldLocation - cameraPOVLocation;

	const double fovRadians = cameraFOV * (std::numbers::pi / 360.0);
	const double tanHalfFOV = tan(fovRadians);

	D3DMATRIX tempMatrix = Matrix(cameraPOVRotation, {0,0,0});

	SDK::FVector vAxisX(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	SDK::FVector vAxisY(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	SDK::FVector vAxisZ(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	SDK::FVector vTransformed(
		DotProduct(vDelta, vAxisY),
		DotProduct(vDelta, vAxisZ),
		DotProduct(vDelta, vAxisX)
	);

	if (vTransformed.Z < 1.0)
		return false;

	ScreenLocation.x = halfScreenWidth + vTransformed.X * (halfScreenWidth / tanHalfFOV) / vTransformed.Z;
	ScreenLocation.y = halfScreenHeight - vTransformed.Y * (halfScreenWidth / tanHalfFOV) / vTransformed.Z;

	return true;
}