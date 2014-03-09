#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../SceneNode.hpp"
#include "character.hpp"

class Player : public Character
{
    public:
        Player(const sf::Texture& texture);
        ~Player();

        void load(std::string filename, std::string fWeapon);

        void attack(sf::Time elapsedTime, sf::Vector2f location);

        sf::FloatRect getGlobalBounds() const;
        sf::FloatRect getLocalBounds() const;

    private:

        void updateCurrent(sf::Time elapsedTime);

        void drawCurrent(sf::RenderTarget& target,sf::RenderStates states) const;

    private:
        DrawableEntity _head,
                       _body,
                       _arm,
                       _foot;
};

#endif

