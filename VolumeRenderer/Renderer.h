#pragma once
#include "Volume.h"
#include <math.h>
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

	float3 operator- (float3 s2)
	{
		float3 res = float3();
		res.x = x - s2.x;
		res.y = y - s2.y;
		res.z = z - s2.z;
		return res;
	}

	float3 operator+ (float3 s2)
	{
		float3 res = float3();
		res.x = x + s2.x;
		res.y = y + s2.y;
		res.z = z + s2.z;
		return res;
	}

	float3 operator* (int i)
	{
		float3 res = float3();
		res.x = x * i;
		res.y = y * i;
		res.z = z * i;
		return res;
	}

	void normalize()
	{
		float dist = sqrt(x*x + y * y + z * z);
		if (dist > 0)
			x = x / dist; y = y / dist; z = z / dist;
	}
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
	float3 cross(float3 vec1, float3 vec2);
	void GetRayBound(float t[2], float3 start_coord, float3 view_vector);
};

