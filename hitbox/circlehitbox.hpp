#ifndef CIRCLEHITBOX_HPP
#define CIRCLEHITBOX_HPP

#include "hitbox.hpp"

class CircleHitbox : public Hitbox
{
    public:
        CircleHitbox(int x, int y, float radius);
        CircleHitbox(sf::Vector2f pos, float radius);
        CircleHitbox(Circle circ);
        ~CircleHitbox();

        bool collide(Hitbox* hitbox);

        bool collide(int x, int y);
        bool collide(sf::Rect<float> rect);
        bool collide(Circle circ);

        void setPosition(int x, int y);
        sf::Vector2f getPosition() const;

        Circle getCircle();

        void drawDebug(sf::RenderWindow& window) const;
        void drawDebug(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        Circle _circ;
};

#endif


