#ifndef CHARACTERMANAGER_HPP
#define CHARACTERMANAGER_HPP

#include <list>

#include "character.hpp"

class CharacterManager
{
    public:
        CharacterManager();
        ~CharacterManager();

        void generateCharacters(); // for survival mode
        void getNewCharacters(sf::Time elapsedTime);

    private:
        std::list<Character*> _characters; // waiting to be added in the game
        sf::Time _generationTime, _elapsedTime;
        bool _waveOver, _noMoreCharacters;
        unsigned int _charactersLeft;
};

#endif