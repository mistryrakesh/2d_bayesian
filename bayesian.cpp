#include <iostream>
#include <cassert>
#include <cmath>

#include "bayesian.h"
#include "utils.h"

namespace bof {

    /* VelocityDistribution - function definitions */
    VelocityDistribution::VelocityDistribution() {
    }

    VelocityDistribution::VelocityDistribution(std::map<int, float> velocityDist) {
        this->velocityDist = velocityDist;
    }

    VelocityDistribution::VelocityDistribution(const int beginVelocity, const int numElems, const int stride) {
        velocityDist.clear();
        for (int i = 0, velocity = beginVelocity; i < numElems; ++i) {
            velocityDist[velocity] = 0;
            velocity += stride;
        }

        assert(velocityDist.size() == numElems);
    }

    void VelocityDistribution::setVelocityProbability(const int velocity, const float probability) {
        assert(velocityDist.find(velocity) != velocityDist.end());
        velocityDist[velocity] = probability;
    }

    float VelocityDistribution::getVelocityProbability(const int velocity) {
        assert(velocityDist.find(velocity) != velocityDist.end());
        return velocityDist[velocity];
    }

    void VelocityDistribution::setVelocityDist(std::map<int, float> velocityDist) {
        this->velocityDist = velocityDist;
    }

    std::map<int, float> VelocityDistribution::getVelocityDist() const {
        return velocityDist;
    }

    std::map<int, float>::const_iterator VelocityDistribution::begin() {
        return velocityDist.begin();
    }

    std::map<int, float>::const_iterator VelocityDistribution::end() {
        return velocityDist.end();
    }

    void VelocityDistribution::toString() {
        printMap(velocityDist);
    }

    /* Cell - function definitions */
    void Cell::getAntecedents(std::set<Cell *>& antecedents, std::vector<std::vector<Cell> >& prevOccGrid) {
        assert(prevOccGrid.size() != 0 && prevOccGrid[0].size() != 0);

        for (std::map<int, float>::const_iterator xit = xVelocityDistribution.begin(); xit != xVelocityDistribution.end(); ++xit) {
            for (std::map<int, float>::const_iterator yit = yVelocityDistribution.begin(); yit != yVelocityDistribution.end(); ++yit) {
                int x = round(xpos + xit->first * dt);
                int y = round(ypos + yit->first * dt);

                if (x >= 0 && x < prevOccGrid.size()) {
                    if (y >= 0 && y < prevOccGrid[x].size()) {
                        antecedents.insert(&prevOccGrid[x][y]);
                    }
                }
            }
        }
    }

    Cell::Cell(VelocityDistribution xVelocityDistribution, VelocityDistribution yVelocityDistribution, float occupiedProbability, const int xpos, const int ypos) {
        this->xpos = xpos;
        this->ypos = ypos;
        this->xVelocityDistribution = xVelocityDistribution;
        this->yVelocityDistribution = yVelocityDistribution;
        this->occupiedProbability = occupiedProbability;
    }

    void Cell::updateDistributions(std::vector<std::vector<Cell> >& prevOccGrid) {
        std::set<Cell *> antecedents;
        getAntecedents(antecedents, prevOccGrid);
    }

    void Cell::toString() {
        xVelocityDistribution.toString();
        yVelocityDistribution.toString();
        std::cout << "Occupied Probability: " << occupiedProbability << std::endl;
    }
}
