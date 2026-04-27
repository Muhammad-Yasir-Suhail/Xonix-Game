#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H
class ScoreSystem {
private:
    int score;
    int rewardCount;
    int bonusThreshold;
public:
    ScoreSystem();
    void reset();
    void addCapturedTiles(int tiles);
    int getScore() const;
};

#endif
