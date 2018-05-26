#include "../include/common.hpp"
#include "../include/player.hpp"

using namespace std;

Player::Player()
{
    faults = NUMBER_OF_FAULTS;
    cout << "Player created successfuly" << endl;
}
Player::~Player()
{
    faults = NUMBER_OF_FAULTS;
    cout << "Player destroyed successfuly" << endl;
}

void Player::setFault()
{
    faults -= 1;
}

string Player::getFaults()
{
    stringstream faults_format;
    faults_format << faults;
    return faults_format.str();
}

bool Player::lostGamePerFault()
{
    return (faults > 0) ? false : true;
}

bool Player::lostGamePerTime()
{
    return (this->timer.showCurrentTime() != "00:00:00") ? false : true;
}
