#include "geometry.h"
#include "grid.h"
#include <map>
using std::map; using std::pair; using std::make_pair;

Geometry::Geometry() :
    m_updateId(0)
{

}

Geometry::~Geometry()
{

}

void Geometry::generateFromGrid(Grid *grid)
{
    m_points.clear();
    m_lines.clear();
    const unsigned int numVoxels = grid->numberOfVoxels();
    vec2 voxelSize = grid->voxelSize();
    map<unsigned int, unsigned int> m_indexMap;
    std::cout << "Will create geometry" << std::endl;
    for(unsigned int idx1=0; idx1<numVoxels; idx1++) {
        if(grid->voxelWithIndex(idx1) != +VoxelType::Surface) continue; // Not a surface
        //Find 2D index
        unsigned int i, j;
        grid->index2D(idx1, i, j);

        vec2 voxel1Center = vec2(i*voxelSize.x(), j*voxelSize.y());

        // Find index of this voxel in the indexMap (they only contain surface voxels, so the mapping isn't trivial).
        // Assume it doesn't exist already and look in the indexMap if it does
        unsigned int voxel1CenterIndex = m_points.size();

        auto iterator = m_indexMap.find(idx1);
        if(iterator != m_indexMap.end()) {
            // We already have this voxel (i,j) in the list
            voxel1CenterIndex = iterator->second;
        } else {
            // We didn't have it, add it to the map
            m_points.push_back(voxel1Center);
            m_indexMap[idx1] = voxel1CenterIndex;
        }

        for(int di=-1; di<=1; di++) {
            for(int dj=-1; dj<=1; dj++) {
                unsigned int idx2 = grid->indexPeriodic(int(i)+di, int(j)+dj);
                unsigned int i2, j2;
                grid->index2D(idx2, i2, j2);

                if(idx2 <= idx1) continue; // We have checked this pair before
                if(grid->voxelWithIndex(idx2) != +VoxelType::Surface) continue; // Not a surface

                vec2 voxel2Center = vec2(i2*voxelSize.x(), j2*voxelSize.y());

                // Find index of this voxel in the indexMap (they only contain surface voxels, so the mapping isn't trivial).
                // Assume it doesn't exist already and look in the indexMap if it does
                unsigned int voxel2CenterIndex = m_points.size();

                auto iterator = m_indexMap.find(idx2);
                if(iterator != m_indexMap.end()) {
                    // We already have this voxel (i,j) in the list
                    voxel2CenterIndex = iterator->second;
                } else {
                    // We didn't have it, add it to the map
                    m_points.push_back(voxel2Center);
                    m_indexMap[idx2] = voxel2CenterIndex;
                }
                pair<unsigned int, unsigned int> thisPair = make_pair(voxel1CenterIndex, voxel2CenterIndex);
                m_lines.push_back(thisPair);
            }
        }
    }

    m_updateId++;
    std::cout << "Did create geometry. We have " << m_lines.size() << " lines connecting " << m_points.size() << " points." << std::endl;
}

