#include "stdafx.h"
#include "Volume.h"


Volume::Volume()
{
	m_volume = nullptr;
}


Volume::Volume(unsigned char* volume, int width, int height, int depth)
{
	m_volume = 
		shared_ptr<unsigned char>(new unsigned char[width*height*depth]);

	memcpy(m_volume.get(), volume, sizeof(unsigned char)*width*height*depth);
	m_width = width;
	m_height = height;
	m_depth = depth;
}


Volume::~Volume()
{
}

unsigned char Volume::GetVoxel(int x, int y, int z)
{
	///return [x][y][z]
	return m_volume.get()[z*m_width*m_height + y * m_width + x];
}

float Volume::GetVoxel(float x, float y, float z)
{
	// tri - linear interpolation
	int x_minus = x;
	int y_minus = y;
	int z_minus = z;

	int x_plus = x + 1;
	int y_plus = y + 1;
	int z_plus = z + 1;

	float x_weight =  x - static_cast<float>(x_minus);
	float y_weight =  y - static_cast<float>(y_minus);
	float z_weight =  z - static_cast<float>(z_minus);

	unsigned char coord_000 = GetVoxel(x_minus, y_minus, z_minus);
	unsigned char coord_100 = GetVoxel(x_plus, y_minus, z_minus);
	unsigned char coord_010 = GetVoxel(x_minus, y_plus, z_minus);
	unsigned char coord_110 = GetVoxel(x_plus, y_plus, z_minus);
	unsigned char coord_001 = GetVoxel(x_minus, y_minus, z_plus);
	unsigned char coord_101 = GetVoxel(x_plus, y_minus, z_plus);
	unsigned char coord_011 = GetVoxel(x_minus, y_plus, z_plus);
	unsigned char coord_111 = GetVoxel(x_plus, y_plus, z_plus);

	float res_000_100 = coord_000 * (1.f - x_weight) + coord_100 * x_weight;
	float res_010_110 = coord_010 * (1.f - x_weight) + coord_110 * x_weight;
	float res_2d_1 = res_000_100 * (1.f - y_weight) + res_010_110 * y_weight;

	float res_001_101 = coord_001 * (1.f - x_weight) + coord_101 * x_weight;
	float res_011_111 = coord_011 * (1.f - x_weight) + coord_111 * x_weight;
	float res_2d_2 = res_001_101 * (1.f - y_weight) + res_011_111 * y_weight;

	float res = res_2d_1 * (1.f - z_weight) + res_2d_2 * z_weight;

	return res;
}

int Volume::GetWidth(void)
{
	return m_width;
}

int Volume::GetHeight(void)
{
	return m_height;
}

int Volume::GetDepth(void)
{
	return m_depth;
}