#include "matrix.h"

#include <cstdlib>
#include <ctime>

using namespace std;

namespace matrix
{

vector<vector<int>> generateMatrix(int nRows, int nColumns, int maxValue)
{
    // use the current time as the RNG seed
    // with this, each run will have slightly different results
    // which is good for statistical analysis
    srand( time(0) );
    vector<vector<int>> v(nRows, vector<int>(nColumns));
    for (int i=0; i<nRows; i++)
        for (int j=0; j<nColumns; j++)
            v[i][j] = random() % maxValue;
    return v;
}

vector<vector<vector<int>>>
generateTensor3D(int nChannels, int nRows, int nColumns, int maxValue)
{
    vector<vector<vector<int>>> v;
    for (int i=0; i<nChannels; i++) v.push_back( generateMatrix(nRows, nColumns, maxValue) );
    return v;
}

vector<vector<vector<vector<int>>>>
generateTensor4D(int nInputs, int nChannels, int nRows, int nColumns, int maxValue)
{
    vector<vector<vector<vector<int>>>> v;
    for (int i=0; i<nInputs; i++) v.push_back( generateTensor3D(nChannels, nRows, nColumns, maxValue) );
    return v;
}

} // matrix
