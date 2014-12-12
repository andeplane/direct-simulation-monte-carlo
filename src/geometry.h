#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "vec2.h"

#include <utility>
#include <vector>
class Grid;

class Geometry
{
private:
    std::vector<vec2> m_points;
    std::vector<std::pair<unsigned int, unsigned int> > m_lines;
    unsigned int m_updateId;
public:
    Geometry();
    ~Geometry();
    void generateFromGrid(Grid *grid);
    unsigned int updateId() { return m_updateId; }
    const std::vector<vec2> &points() { return m_points; }
    const std::vector<std::pair<unsigned int, unsigned int> > &lines() { return m_lines; }
};

#endif // GEOMETRY_H
