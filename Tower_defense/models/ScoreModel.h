#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "include/json.hpp"

using json = nlohmann::json;

class ScoreModel {
public:
    ScoreModel();
    void saveScore(int newScore);
    std::vector<int> loadScores() const;
    void sortScores(std::vector<int>& scores) const;
}; 