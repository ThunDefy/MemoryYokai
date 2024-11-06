#include "player.h"

Player::Player()
{

}

void Player::setScore(int newScore)
{
    score =  newScore;
}

void Player::setTurns(int newTurns)
{
    turns = newTurns;
}

int Player::getScore()
{
    return score;
}

int Player::getTurns()
{
    return turns;
}

void Player::gets1Point()
{
    score += 1;
}

void Player::makeTurn()
{
    turns+=1;
}
