#include "grid.h"

Grid::Grid()
{
}

void Grid::initialize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    m_grid.resize(width*height);
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
