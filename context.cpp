#include "config.hpp"

#include "context.hpp"

Context::Context() : _mainWindow(),
                     _shots(),
                     _characters(),
                     _level(_mainWindow),
                     _isDebug(false),
                     _isGameOver(false),
                     _isSuccess(false)
{
}

Context::~Context()
{
    std::for_each(_shots.begin(), _shots.end(), Deallocator<Shot>());
    std::for_each(_characters.begin(), _characters.end(), Deallocator<Character>());
}

void Context::reset()
{
    std::for_each(_shots.begin(), _shots.end(), Deallocator<Shot>());
    std::for_each(_characters.begin(), _characters.end(), Deallocator<Character>());
    _isGameOver = false;
    _isSuccess = false;

    _shots.clear();
    _characters.clear();
}

std::list<Shot*>& Context::getShots()
{
    return _shots;
}

void Context::addShot(Shot* shot)
{
    _shots.push_back(shot);
}

std::list<Character*>& Context::getCharacters()
{
    return _characters;
}

void Context::addCharacter(Character* character)
{
    _characters.push_back(character);
}
