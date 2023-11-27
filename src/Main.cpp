#include <stdlib.h>

#include "Common.h"
#include "Graphics.h"
#include "Input.h"
#include "Math.h"
#include "Utils.h"

constexpr int numberOfPoints = 9 * 9 * 9;
Vec3f cubePoints[numberOfPoints];
Vec2f projectedPoints[numberOfPoints];
float fovFactor = 0.f;

Vec3f cameraPosition = { 0, 0, -5 };

static bool quit = false;

bool ShouldQuit(void)
{
	return quit;
}

void Quit(void)
{
	quit = true;
}

static void InitCube()
{
	int ptsCount = 0;
	for (float x = -1.f; x <= 1.f; x += 0.25f)
	{
		for (float y = -1.f; y <= 1.f; y += 0.25f)
		{
			for (float z = -1.f; z <= 1.f; z += 0.25f)
			{
				cubePoints[ptsCount++] = { x, y, z };
			}
		}
	}
}

static void Init(void)
{
	InitGraphics("SoftwareRenderer");
	InitCube();
	fovFactor = (float)GetWindowData().height / 2.f;
}

static void Deinit(void)
{
	DeinitGraphics();
}

static Vec2f Project(Vec3f point)
{
	return Vec2f
	{
		.x = (fovFactor * point.x) / point.z,
		.y = (fovFactor * point.y) / point.z,
	};
}

static void Update(void)
{
	for (int i = 0; i < numberOfPoints; i++)
	{
		Vec3f point = cubePoints[i];
		point.z -= cameraPosition.z;
		Vec2f projectedPoint = Project(point);
		projectedPoints[i] = projectedPoint;
	}
}

static void Draw(void)
{
	const WindowData& window = GetWindowData();

	BeginDraw();
	DrawGrid(0, 0, window.width, window.height, Color::white, 10);
	for (int i = 0; i < numberOfPoints; i++)
	{
		Vec2f pt = projectedPoints[i];
		DrawRect(
			pt.x + window.width / 2,
			pt.y + window.height / 2,
			4, 4,
			Color::yellow);
	}
	EndDraw();
}

int main(void)
{
	// atexit won't work properly if the renderer goes into a DLL.
	atexit(Deinit);

	Init();

	while (!ShouldQuit())
	{
		HandleInput();
		Update();
		Draw();
	}

	return 0;
}
