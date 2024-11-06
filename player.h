#ifndef PLAYER_H
#define PLAYER_H


class Player
{
public:
    Player();
    void setScore(int newScore);
    void setTurns(int newTurns);
    int getScore();
    int getTurns();
    void gets1Point();
    void makeTurn();



private:
    int score=0;
    int turns=0;

};

#endif // PLAYER_H
