#pragma once
#include <vector>
#include "vec2.h"

enum class VoxelType{ Empty = 0, Wall = 1, Surface = 2 };
inline int operator + (VoxelType val) {
    return static_cast<int>(val);
}

class Grid
{
private:
    int m_width;
    int m_height;
    std::vector<unsigned char> m_voxels;
    std::vector<vec2> m_normals;
    std::vector<vec2> m_tangents;
public:
    Grid();
    unsigned int index(unsigned int i, unsigned int j) { return i + j*m_width; }
    unsigned int indexPeriodic(int i, int j) { return ((i + m_width) % m_width) + ((j + m_height) % m_height)*m_width; }
    void setValue(unsigned int i, unsigned int j, unsigned char value) { m_voxels[index(i,j)] = value; }
    void setValue(unsigned int index, unsigned char value) { m_voxels[index] = value; }
    void initialize(unsigned int width, unsigned int height);
    void createBox();
    void computeNormalsAndTangents();
    void findSurfacePoints();
};
