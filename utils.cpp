#include <iostream>

#include "utils.h"

void printMap(const std::map<int, float>& intFloatMap) {
    for (std::map<int, float>::const_iterator it = intFloatMap.begin(); it != intFloatMap.end(); ++it) {
        std::cout << "key: " << it->first << ", value: " << it->second << std::endl;
    }
}
