#include <iostream>
#include <vector>

#include "bayesian.h"

using namespace std;

int main(int argc, char **argv) {
    bof::VelocityDistribution xVelDist(-10, 11, 2);
    xVelDist.setVelocityProbability(-2, 1);

    bof::VelocityDistribution yVelDist(-10, 11, 2);
    yVelDist.setVelocityProbability(0, 1);

    /* Initialize occupancy grid */
    vector<vector<bof::Cell> > occGrid;
    for (int y = 0; y < 100; ++y) {
        vector<bof::Cell> occRow;
        for (int x = 0; x < 100; ++x) {
            bof::Cell cell(xVelDist, yVelDist, 0, x, y);
            occRow.push_back(cell);
        }
        occGrid.push_back(occRow);
    }

    /* Update occupancy grid */
    vector<vector<bof::Cell> > prevOccGrid = occGrid;

    for (int i = 0; i < 100; ++i) {
        for (int j = 0; i < 100; ++i) {
            occGrid[i][j].updateDistributions(prevOccGrid);
        }
    }

    return 0;
}
