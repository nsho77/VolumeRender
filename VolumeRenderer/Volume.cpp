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