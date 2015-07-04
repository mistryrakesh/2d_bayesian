#ifndef __bayesian_h__
#define __bayesian_h__

#include <map>
#include <vector>
#include <set>

#define dt 0.5

namespace bof {

    class VelocityDistribution {
    private:
        std::map<int, float> velocityDist;

    public:
        VelocityDistribution();
        VelocityDistribution(std::map<int, float> velocityDist);
        VelocityDistribution(const int beginVelocity, const int numElems, const int stride);

        void setVelocityProbability(const int velocity, const float probability);
        float getVelocityProbability(const int velocity);
        
        std::map<int, float>::const_iterator begin();
        std::map<int, float>::const_iterator end();
        void toString();

        void setVelocityDist(std::map<int, float> velocityDist);

        std::map<int, float> getVelocityDist() const;
    };

    class Cell {
    private:
        int xpos;
        int ypos;
        VelocityDistribution xVelocityDistribution;
        VelocityDistribution yVelocityDistribution;
        float occupiedProbability;
        
    private:
        void getAntecedents(std::set<Cell *>& antecedents, std::vector<std::vector<Cell> >& prevOccGrid);

    public:
        Cell(VelocityDistribution xVelocityDistribution, VelocityDistribution yVelocityDistribution, const float occupiedProbability, const int xpos, const int ypos);

        void updateDistributions(std::vector<std::vector<Cell> >& prevOccGrid);
        void toString();
    };
}

#endif // __bayesian_h__