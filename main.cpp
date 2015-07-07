#include <iostream>
#include <vector>

#include "bayesian.h"

#define GRIDROWS 10
#define GRIDCOLS 10

using namespace std;

int main(int argc, char **argv) {
    bof::VelocityDistribution xVelDist(-6, 7, 2, -2);
    bof::VelocityDistribution yVelDist(-6, 7, 2, 0);

    /* Initialize occupancy grid */
    vector<vector<bof::Cell> > occGrid;
    for (int y = 0; y < GRIDROWS; ++y) {
        vector<bof::Cell> occRow;
        for (int x = 0; x < GRIDCOLS; ++x) {
            int occupancy = 0;
            if (x == y)
                occupancy = 1;
            bof::Cell cell(xVelDist, yVelDist, occupancy, x, y);
            occRow.push_back(cell);
        }
        occGrid.push_back(occRow);
    }

    /* Update occupancy grid */
    vector<vector<bof::Cell> > prevOccGrid = occGrid;

    for (int i = 0; i < GRIDROWS; ++i) {
        for (int j = 0; j < GRIDCOLS; ++j) {
            occGrid[i][j].updateDistributions(prevOccGrid);
            cout << "[" << i << "][" << j << "]: ";
            occGrid[i][j].toString();
        }
    }

    return 0;
}
