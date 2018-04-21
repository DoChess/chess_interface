#include "../include/common.hpp"
#include "../include/player.hpp"

using namespace std;

Player::Player()
{
    faults = NUMBER_OF_FAULTS;
}

void Player::setFault()
{
    faults -= 1;
}

short int Player::getFaults()
{
    return faults;
}

bool Player::lostGamePerFault()
{
    if(faults >= 0)
    {
        return false;
    } else {
        return true;
    }
}