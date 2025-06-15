#include "ScoreModel.h"
#include <algorithm>
#include <fstream>
#include <iostream>

ScoreModel::ScoreModel() {
    // Ensure the scores file exists
    std::ifstream inFile("menu/score.json");
    if (!inFile.is_open()) {
        // Create a new file with empty scores array
        std::ofstream outFile("menu/score.json");
        json data;
        data["scores"] = json::array();
        outFile << data.dump(4);
    }
}

void ScoreModel::saveScore(int newScore) {
    json data;
    std::ifstream inFile("menu/score.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }

    // Add new score
    data["scores"].push_back(newScore);

    // Save back to file
    std::ofstream outFile("menu/score.json");
    outFile << data.dump(4);
}

std::vector<int> ScoreModel::loadScores() const {
    std::vector<int> scores;
    json data;
    std::ifstream inFile("menu/score.json");
    
    if (inFile.is_open()) {
        inFile >> data;
        if (data.contains("scores") && data["scores"].is_array()) {
            for (const auto& score : data["scores"]) {
                scores.push_back(score.get<int>());
            }
        }
    }
    
    return scores;
}

void ScoreModel::sortScores(std::vector<int>& scores) const {
    std::sort(scores.begin(), scores.end(), std::greater<>());
} 