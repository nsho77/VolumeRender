#include "stdafx.h"
#include "Volume.h"


Volume::Volume()
{
	m_volume = nullptr;
}

Volume::Volume(unsigned char* volume, int width, int height, int depth)
{
	m_volume = shared_ptr<unsigned char>(
		new unsigned char[width*height*depth]);
	// m_volume.get() 하면 shared_ptr이 가리키는 배열의 주소가 반환된다.
	memcpy(m_volume.get(), volume, sizeof(unsigned char)*width*height*depth);
	m_width = width;
	m_height = height;
	m_depth = depth;
}


Volume::~Volume()
{
}

unsigned char Volume::getVoxel(int x, int y, int z)
{
	// return m_volume[z][y][x]
	return m_volume.get()[m_width*m_height*z + m_width * y + x];
}

int Volume::getWidth()
{
	return m_width;
}

int Volume::getHeight()
{
	return m_height;
}

int Volume::getDepth()
{
	return m_depth;
}