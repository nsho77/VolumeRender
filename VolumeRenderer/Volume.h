#pragma once
// shared_ptr ����Ϸ��� �߰�
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
	// volume �� ������ �޸� �����ϰ� volume �� ������ ī���Ѵ�.
	Volume(unsigned char* volume, int width, int height, int depth);
	~Volume();

public:
	// ���� ��ǥ�� �ش��ϴ� voxel �� ��ȯ�Ѵ�.
	unsigned char getVoxel(int x, int y, int z);
	int getWidth();
	int getHeight();
	int getDepth();
};

