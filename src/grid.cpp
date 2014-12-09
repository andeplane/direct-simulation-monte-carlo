#include "grid.h"
#include "system.h"
#include <cassert>

Grid::Grid()
{
    // https://oeis.org/A002182
    // 1, 2, 4, 6, 12, 24, 36, 48, 60, 120, 180, 240, 360, 720, 840, 1260, 1680, 2520, 5040, 7560, 10080
}

vector<vec2> Grid::cornersForVoxelWithIndex(unsigned int voxelIndex)
{
    unsigned int i,j;
    index2D(voxelIndex, i, j);
    vector<vec2> points(4);

    points[0] = vec2(i*m_voxelSize[0], j*m_voxelSize[1]);
    points[1] = vec2( (i+1)*m_voxelSize[0], j*m_voxelSize[1]);
    points[2] = vec2( (i+1)*m_voxelSize[0], (j+1)*m_voxelSize[1]);
    points[3] = vec2(i*m_voxelSize[0], (j+1)*m_voxelSize[1]);

    return points;
}

void Grid::initialize(unsigned int width, unsigned int height, System *system)
{
    m_width = width;
    m_height = height;
    m_voxels.resize(width*height, 0);
    m_normals.resize(width*height);
    m_tangents.resize(width*height);

    m_oneOverSystemSizeTimesNumVoxels = vec2(m_width/system->size().x(), m_height/system->size().y());
    m_voxelSize[0] = system->size().x() / m_width;
    m_voxelSize[1] = system->size().y() / m_height;
#ifdef DSMC_DEBUG
    std::cout << "Voxel size: " << m_voxelSize << std::endl;
#endif
}

void Grid::createSphere()
{
    int radiusSquared = (m_width / 4);
    radiusSquared *= radiusSquared;
    int numberOfWalls = 0;
    for(int i=0; i<m_width; i++) {
        for(int j=0; j<m_height; j++) {
            int dx = i - m_width/2;
            int dy = j - m_height/2;
            int dr2 = dx*dx + dy*dy;

            bool wall = dr2 < radiusSquared;
            numberOfWalls += wall;
            setValue(i,j,wall);
        }
    }

    std::cout << "We have " << numberOfWalls << " walls giving ratio of: " << float(numberOfWalls)/(m_width*m_height) << std::endl;
    computeNormalsAndTangents();
    findSurfacePoints();
}

void Grid::createBox()
{
    for(int i=0; i<m_width; i++) {
        for(int j=0; j<m_height; j++) {
            bool wall = i==0 || i==m_width-1 || j==0 || j==m_height-1;
            setValue(i,j,wall);
        }
    }

    computeNormalsAndTangents();
    findSurfacePoints();
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
            m_tangents[idx][1] = m_normals[idx][0];
        }
    }
}

void Grid::findSurfacePoints()
{
    for(unsigned int i=0; i<m_width; i++) {
        for(unsigned int j=0; j<m_height; j++) {
            unsigned int idx = index(i,j);
            if(m_voxels[idx] > +VoxelType::Empty && m_normals[idx].lengthSquared() > 0) {
                // We have a non-empty voxel with a non-zero normal vector meaning it is a surface voxel
                m_voxels[idx] = +VoxelType::Surface;
            }
        }
    }
}

void Grid::willCollideWithLine(vec2 p0, vec2 p1, vec2 r0, vec2 v, double &t, bool &willCollide)
{
    p1.subtract(p0);
    t = (p0 - r0).cross(p1) / v.cross(p1);
    double u = (r0 - p0).cross(v) / p1.cross(v);
    willCollide = (t >= 0) && (u>= 0 && u<=1);
}

bool Grid::isInsideWallVoxel(double x, double y)
{
    return (m_voxels[index(x,y)] > +VoxelType::Empty);
}

float Grid::timeUntilCollision(unsigned int voxelIndex, vec2 r0, vec2 v)
{
    unsigned int i,j;
    index2D(voxelIndex, i, j);
    vec2 points[4];
    double times[4];
    bool willCollide[4];

    points[0] = vec2(i*m_voxelSize[0], j*m_voxelSize[1]);
    points[1] = vec2( (i+1)*m_voxelSize[0], j*m_voxelSize[1]);
    points[2] = vec2( (i+1)*m_voxelSize[0], (j+1)*m_voxelSize[1]);
    points[3] = vec2(i*m_voxelSize[0], (j+1)*m_voxelSize[1]);

    willCollideWithLine(points[0], points[1], r0, v, times[0], willCollide[0]);
    willCollideWithLine(points[1], points[2], r0, v, times[1], willCollide[1]);
    willCollideWithLine(points[2], points[3], r0, v, times[2], willCollide[2]);
    willCollideWithLine(points[3], points[0], r0, v, times[3], willCollide[3]);
    double lowestT = 1e9;

    if(willCollide[0] && times[0] < lowestT) lowestT = times[0];
    if(willCollide[1] && times[1] < lowestT) lowestT = times[1];
    if(willCollide[2] && times[2] < lowestT) lowestT = times[2];
    if(willCollide[3] && times[3] < lowestT) lowestT = times[3];

    assert( (willCollide[0] || willCollide[1] || willCollide[2] || willCollide[3]) && "Did not collide with voxel.");
    return lowestT;
}
