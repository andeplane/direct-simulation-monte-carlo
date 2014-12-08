#pragma once
#include <vector>

class Grid
{
private:
    int m_width;
    int m_height;
    std::vector<unsigned char> m_grid;

public:
    Grid();
    unsigned int index(unsigned int i, unsigned int j) { return i + j*m_width; }
    unsigned int indexPeriodic(int i, int j) { return ((i + m_width) % m_width) + ((j + m_height) % m_height)*m_width; }
    void setValue(unsigned int i, unsigned int j, unsigned char value) { m_grid[index(i,j)] = value; }
    void setValue(unsigned int index, unsigned char value) { m_grid[index] = value; }
    void initialize(unsigned int width, unsigned int height);
    void createBox();
};
