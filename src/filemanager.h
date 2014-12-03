#pragma once

#include <fstream>
using std::ifstream;
using std::ofstream;

class Particles;
class FileManager
{
private:
    ofstream m_xyzFile;
public:
    FileManager();
    void writeXyz(Particles *particles);
};
