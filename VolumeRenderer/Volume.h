#pragma once
#include <memory>
using namespace std;

class Volume
{
private:
	shared_ptr<unsigned char> m_volume;
	int m_width;
	int m_height;
	int m_depth;

public:
	Volume();
	Volume(unsigned char* volume, int width, int height, int depth);
	~Volume();

public:
	unsigned char GetVoxel(int x, int y, int z);
	float GetVoxel(float x, float y, float z);
	int GetWidth(void);
	int GetHeight(void);
	int GetDepth(void);
};

