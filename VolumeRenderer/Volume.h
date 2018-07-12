#pragma once
// shared_ptr 사용하려면 추가
#include <memory>
using namespace std;

class Volume
{
private:
	shared_ptr<unsigned char>m_volume;
	int m_width;
	int m_height;
	int m_depth;

public:
	Volume();
	// volume 을 받으면 메모리 생성하고 volume 의 내용을 카피한다.
	Volume(unsigned char* volume, int width, int height, int depth);
	~Volume();

public:
	// 인자 좌표에 해당하는 voxel 을 반환한다.
	unsigned char getVoxel(int x, int y, int z);
	int getWidth();
	int getHeight();
	int getDepth();
};

