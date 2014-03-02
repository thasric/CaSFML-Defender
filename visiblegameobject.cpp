#include "config.hpp"
#include "game.hpp"
#include "visiblegameobject.hpp"

VisibleGameObject::VisibleGameObject() : _isLoaded(false), _isAlive(true), _hitbox(NULL)
{
}

VisibleGameObject::~VisibleGameObject()
{
}

void VisibleGameObject::load(std::string filename)
{
    ImageManager *im = Locator::getImageManager();
    _texture = im->getTexture(IMAGES_PATH + filename);
    if(_texture == NULL)
    {
        _filename = "";
        _isLoaded = false;
    }
    else
    {
        _filename = filename;
        _sprite.setTexture(*_texture);
        _isLoaded = true;
    }
}

void VisibleGameObject::draw(sf::RenderWindow& window)
{
    if(_isLoaded)
    {
        window.draw(_sprite);
        if(_hitbox && Game::getContext().getDebug())
            _hitbox->drawDebug(window);
    }
}

void VisibleGameObject::update(sf::Time elapsedTime)
{
}

bool VisibleGameObject::collide(VisibleGameObject &object)
{
    return _hitbox->collide(object.getHitbox());
}

bool VisibleGameObject::collide(Collidable &object)
{
    return _hitbox->collide(object.getHitbox());
}

void VisibleGameObject::die()
{
}

void VisibleGameObject::setPosition(float x, float y)
{
    if(_isLoaded)
    {
        _sprite.setPosition(x, y);
        if(_hitbox)
            _hitbox->setPosition(x, y);
    }
}

sf::FloatRect VisibleGameObject::getDimension() const
{
    return _sprite.getGlobalBounds();
}

const sf::Vector2f& VisibleGameObject::getPosition()
{
    if(_isLoaded)
        return _sprite.getPosition();
    return sf::Vector2f();
}

bool VisibleGameObject::isLoaded() const
{
    return _isLoaded;
}

bool VisibleGameObject::isAlive() const
{
    return _isAlive;
}

sf::Sprite& VisibleGameObject::getSprite()
{
    return _sprite;
}

Hitbox* VisibleGameObject::getHitbox()
{
    return _hitbox;
}
