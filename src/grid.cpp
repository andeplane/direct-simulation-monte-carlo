#include "grid.h"

Grid::Grid()
{
    // https://oeis.org/A002182
    // 1, 2, 4, 6, 12, 24, 36, 48, 60, 120, 180, 240, 360, 720, 840, 1260, 1680, 2520, 5040, 7560, 10080
    initialize(720, 720);
}

void Grid::initialize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    m_voxels.resize(width*height);
    m_normals.resize(width*height);
    m_tangents.resize(width*height);
}

void Grid::createBox()
{
    for(int i=0; i<m_width; i++) {
        for(int j=0; j<m_height; j++) {
            bool wall = i==0 || i==m_width-1 || j==0 || j==m_height-1;
            setValue(i,j,wall);
        }
    }
}

void Grid::computeNormalsAndTangents()
{
    for(unsigned int i=0; i<m_normals.size(); i++) {
        m_normals[i].setToZero();
        m_tangents[i].setToZero();
    }

    for(int i=0; i<m_width; i++) {
        for(int j=0; j<m_height; j++) {
            bool atLeastOneWallNeighbor = false;
            bool allNeighborsAreWalls = true;
            int idx = indexPeriodic(i, j);

            for(int di=-1; di<=1; di++) {
                for(int dj=-1; dj<=1; dj++) {
                    int idx2 = indexPeriodic(i+di, j+dj);
                    if(m_voxels[idx2]==+VoxelType::Empty) {
                        allNeighborsAreWalls = false;
                    } else {
                        atLeastOneWallNeighbor = true;
                    }

                    m_normals[idx][0] -= m_voxels[idx2]*di;
                    m_normals[idx][1] -= m_voxels[idx2]*dj;
                }
            }

            m_normals[idx].normalize();
            m_tangents[idx][0] = -m_normals[idx][1];
            m_tangents[idx][1] = -m_normals[idx][0];
        }
    }
}

void Grid::findSurfacePoints()
{
    for(int i=0; i<m_width; i++) {
        for(int j=0; j<m_height; j++) {
            unsigned int idx = index(i,j);
            if(m_voxels[idx] > +VoxelType::Empty && m_normals[idx].lengthSquared() > 0) {
                // We have a non-empty voxel with a non-zero normal vector meaning it is a surface voxel
                m_voxels[idx] = +VoxelType::Surface;
            }
        }
    }
}
