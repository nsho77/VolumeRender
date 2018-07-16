#pragma once
#include "Volume.h"

struct float3
{
	float x;
	float y;
	float z;

	float3()
	{
		x = 0.f; y = 0.f; z = 0.f;
	};
	float3(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	};
};

class Renderer
{
private:
	shared_ptr<Volume> m_pVolume;

public:
	Renderer();
	Renderer(unsigned char* volume, int width, int height, int depth);
	~Renderer();

public:
	bool RenderSliceXDirection(unsigned char* image, 
		const int img_width, const int img_height,
		const int depth);
	bool RenderSliceYDirection(unsigned char* image,
		const int img_width, const int img_height,
		const int depth);
	bool RenderSliceZDirection(unsigned char* image,
		const int img_width, const int img_height,
		const int depth);

	bool RenderMIPXDirection(unsigned char* image,
		const int img_width, const int img_height);
	bool RenderMIPYDirection(unsigned char* image,
		const int img_width, const int img_height);
	bool RenderMIPZDirection(unsigned char* image,
	const int img_width, const int img_height);
	bool RenderMIPAnyDirection(unsigned char* image,
		const int img_width, const int img_height);
};

