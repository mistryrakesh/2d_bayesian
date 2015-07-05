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

    void Cell::getPrediction(float& alphaO, float& alphaE, const int xVelocity, const int yVelocity, const std::set<Cell *>& antecedents, const std::vector<std::vector<Cell> >& prevOccGrid) {
        alphaO = 0;
        alphaE = 0;

        for (std::set<Cell *>::const_iterator it = antecedents.begin(); it != antecedents.end(); ++it) {
            Cell *cell = *it;
            alphaO += (1.0 / antecedents.size()) * // P(A)
                    cell->getProbabilityOfXVelocity(xVelocity) * cell->getProbabilityOfYVelocity(yVelocity) * // P(V|A)
                    isReachable(xVelocity, yVelocity, cell) * // P(C|V,A)
                    cell->getOccupiedProbability(); // P(occ^-1|A)

            alphaE += (1.0 / antecedents.size()) * // P(A)
                    cell->getProbabilityOfXVelocity(xVelocity) * cell->getProbabilityOfYVelocity(yVelocity) * // P(V|A)
                    isReachable(xVelocity, yVelocity, cell) * // P(C|V,A)
                    (1.0 - cell->getOccupiedProbability()); // P(emp^-1|A)
        }
    }

    int Cell::isReachable(const int xVelocity, const int yVelocity, const Cell* cell) {
        int reachableXPos = round(cell->getXPos() + xVelocity * dt);
        int reachableYPos = round(cell->getYPos() + yVelocity * dt);

        if (xpos == reachableXPos && ypos == reachableYPos)
            return 1;

        return 0;
    }

    Cell::Cell(VelocityDistribution xVelocityDistribution, VelocityDistribution yVelocityDistribution, float occupiedProbability, const int xpos, const int ypos) {
        this->xpos = xpos;
        this->ypos = ypos;
        this->xVelocityDistribution = xVelocityDistribution;
        this->yVelocityDistribution = yVelocityDistribution;
        this->occupiedProbability = occupiedProbability;
    }

    int Cell::getXPos() const {
        return xpos;
    }

    int Cell::getYPos() const {
        return ypos;
    }

    int Cell::getOccupiedProbability() const {
        return occupiedProbability;
    }

    float Cell::getProbabilityOfXVelocity(const int xVelocity) {
        return xVelocityDistribution.getVelocityProbability(xVelocity);
    }

    float Cell::getProbabilityOfYVelocity(const int yVelocity) {
        return yVelocityDistribution.getVelocityProbability(yVelocity);
    }

    void Cell::updateDistributions(std::vector<std::vector<Cell> >& prevOccGrid) {
        std::set<Cell *> antecedents;
        getAntecedents(antecedents, prevOccGrid);

        std::vector<int> xVelocityKeys;
        std::vector<int> yVelocityKeys;
        bool yVelocityKeysUpdated = false;

        std::vector<std::vector<float> > alphaOccMatrix;
        std::vector<std::vector<float> > alphaEmpMatrix;

        float lvkSum = 0;
        for (std::map<int, float>::const_iterator xit = xVelocityDistribution.begin(); xit != xVelocityDistribution.end(); ++xit) {
            xVelocityKeys.push_back(xit->first);
            std::vector<float> alphaOccRow;
            std::vector<float> alphaEmpRow;

            for (std::map<int, float>::const_iterator yit = yVelocityDistribution.begin(); yit != yVelocityDistribution.end(); ++yit) {
                if (!yVelocityKeysUpdated)
                    yVelocityKeys.push_back(yit->first);

                float alphaO = 0;
                float alphaE = 0;
                getPrediction(alphaO, alphaE, xit->first, yit->first, antecedents, prevOccGrid);
                lvkSum += alphaO + alphaE;

                alphaOccRow.push_back(alphaO);
                alphaEmpRow.push_back(alphaE);
            }
            alphaOccMatrix.push_back(alphaOccRow);
            alphaEmpMatrix.push_back(alphaEmpRow);
            yVelocityKeysUpdated = true;
        }
    }

    void Cell::toString() {
        xVelocityDistribution.toString();
        yVelocityDistribution.toString();
        std::cout << "Occupied Probability: " << occupiedProbability << std::endl;
    }
}
