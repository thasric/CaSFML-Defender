#ifndef VISIBLEGAMEOBJECT_HPP
#define VISIBLEGAMEOBJECT_HPP

#include "SFML/Graphics/Sprite.hpp"

#include "hitbox/circlehitbox.hpp"
#include "hitbox/boundingboxhitbox.hpp"

class VisibleGameObject
{
    public:
        VisibleGameObject();
        virtual ~VisibleGameObject();

        virtual void load(std::string filename);
        virtual void update(sf::Time elapsedTime);
        virtual bool collide(VisibleGameObject &object);
        virtual void draw(sf::RenderWindow& window);
        virtual void die();

        virtual void setPosition(float x, float y);
        virtual const sf::Vector2f& getPosition();
        virtual sf::FloatRect getDimension() const;
        virtual bool isLoaded() const;
        virtual bool isAlive() const;
        virtual Hitbox* getHitbox();

    protected:
        sf::Sprite& getSprite();
        Hitbox *_hitbox;
        bool _isAlive;

    private:
        sf::Sprite _sprite;
        sf::Texture *_texture;
        std::string _filename;
        bool _isLoaded;
};

#endif

