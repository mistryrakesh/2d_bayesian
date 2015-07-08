#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "bayesian.h"

#define GRIDROWS 100
#define GRIDCOLS 100

using namespace std;
using namespace cv;

void displayOccupancyGrid(vector<vector<bof::Cell> >& occGrid) {

    Mat mapImg = Mat::zeros(GRIDROWS, GRIDCOLS, CV_8UC3);

    for (int i = 0; i < GRIDROWS; ++i) {
        for (int j = 0; j < GRIDCOLS; ++j) {
            mapImg.at<Vec3b>(i, j)[0] = occGrid[i][j].getOccupiedProbability() * 255;
            mapImg.at<Vec3b>(i, j)[1] = occGrid[i][j].getOccupiedProbability() * 255;
            mapImg.at<Vec3b>(i, j)[2] = occGrid[i][j].getOccupiedProbability() * 255;
        }
    }

    while (waitKey(30) != 27) { // wait for ESC key press
        imshow("occupancyGrid", mapImg);
    }
}

int main(int argc, char **argv) {

    namedWindow("occupancyGrid", CV_WINDOW_AUTOSIZE);

    bof::VelocityDistribution xVelDist(-6, 7, 2, 0);
    bof::VelocityDistribution yVelDist(-6, 7, 2, -2);

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
            // cout << "[" << i << "][" << j << "]: ";
            // occGrid[i][j].toString();
        }
    }

    displayOccupancyGrid(occGrid);

    return 0;
}
