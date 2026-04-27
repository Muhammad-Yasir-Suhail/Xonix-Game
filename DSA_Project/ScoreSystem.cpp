#include "ScoreSystem.h"
ScoreSystem::ScoreSystem() {
    reset();
}
void ScoreSystem::reset() {
    score = 0;
    rewardCount = 0;
    bonusThreshold = 10;
}
void ScoreSystem::addCapturedTiles(int tiles) {
    if (tiles <= 0) {
        return;
    }
    int multiplier = 1;
    if (tiles > bonusThreshold) {
        rewardCount++;
        if (rewardCount >= 5)
            multiplier = 4;
        else {
            multiplier = 2;
        }
        if (rewardCount == 3)
            bonusThreshold = 5;
    }
    score += tiles * multiplier;
}
int ScoreSystem::getScore() const {
    return score;
}
