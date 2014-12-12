#pragma once
#include <vector>
#include "vec2.h"
class System;
using std::vector;

enum class VoxelType{ Empty = 0, Wall = 1, Surface = 2 };
inline int operator + (VoxelType val) {
    return static_cast<int>(val);
}
class Grid
{
private:
    vec2 m_oneOverSystemSizeTimesNumVoxels;
    int m_width;
    int m_height;
    std::vector<unsigned char> m_voxels;
    std::vector<vec2> m_normals;
    std::vector<vec2> m_tangents;
    vec2 m_voxelSize;
public:
    Grid();
    unsigned char voxelPeriodic(double x, double y) {
        return m_voxels[indexPeriodic(x,y)];
    }

    unsigned int index(double x, double y) {
        unsigned int i = x*m_oneOverSystemSizeTimesNumVoxels[0];
        unsigned int j = y*m_oneOverSystemSizeTimesNumVoxels[1];
        return index(i,j);
    }

    unsigned int indexPeriodic(double x, double y) {
        int i = x*m_oneOverSystemSizeTimesNumVoxels[0];
        int j = y*m_oneOverSystemSizeTimesNumVoxels[1];
        return indexPeriodic(i,j);
    }
    vector<vec2> cornersForVoxelWithIndex(unsigned int voxelIndex);
    unsigned char voxelWithIndex(unsigned int idx) { return m_voxels[idx]; }
    unsigned int index(unsigned int i, unsigned int j) { return i + j*m_width; }
    unsigned int index(int i, int j) { return i + j*m_width; }
    unsigned int indexPeriodic(int i, int j) { return ((i + m_width) % m_width) + ((j + m_height) % m_height)*m_width; }
    void index2D(unsigned int idx, unsigned int &i, unsigned int &j) { j = idx / m_width; i = idx % m_width; }
    void setValue(unsigned int i, unsigned int j, unsigned char value) { m_voxels[index(i,j)] = value; }
    void setValue(unsigned int index, unsigned char value) { m_voxels[index] = value; }
    void initialize(unsigned int width, unsigned int height, System *system);
    void createBox();
    void computeNormalsAndTangents();
    void findSurfacePoints();
    void willCollideWithLine(vec2 p0, vec2 p1, vec2 r0, vec2 v, double &t, bool &willCollide);
    bool isInsideWallVoxel(double x, double y);
    float normal(unsigned int voxelIndex, unsigned int component) { return m_normals[voxelIndex][component]; }
    float tangent(unsigned int voxelIndex, unsigned int component) { return m_tangents[voxelIndex][component]; }
    unsigned int width() { return m_width; }
    unsigned int height() { return m_height; }
    unsigned int numberOfVoxels() { return m_width*m_height; }
    float timeUntilCollision(unsigned int voxelIndex, vec2 r0, vec2 v);
    void createSphere();
    vec2 voxelSize() { return m_voxelSize; }
};
