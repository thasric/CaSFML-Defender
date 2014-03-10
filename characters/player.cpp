#include "../config.hpp"

#include "../game.hpp"
#include "player.hpp"

Player::Player(const sf::Texture& texture) : Character(texture),
                                             _head(texture),
                                             _body(texture),
                                             _arm(texture),
                                             _foot(texture)
{
}

Player::~Player()
{
}

void Player::load(std::string filename, std::string fWeapon)
{
    Character::load(filename, fWeapon);

    detachChild(*_weapon);
    _arm.attachChild(_weapon);
}

void Player::loadFromNode(Node& root)
{
    ///@todo: this can be in the parent class
    // The first node has to be "entity"
    if(root.getName() != "entity")
    {
        std::string error = "Error : first node has to be \"entity\"";
        throw std::runtime_error(error);
    }

    // We fill the entity's attributes
    //_life = root.firstAttributeValue("life");

    ///////////// MEMBERS PART /////////////

    Node& membersNode = root.firstChild("members");

    Node& footNode = membersNode.firstChild("foot");
    fillPartsFromNode(footNode, _foot);
    attachChild(&_foot);
    Log::write(Log::LOG_INFO, "Member part : Foot texture region : " + toString(_foot.getGlobalBounds().left) + ";" + toString(_foot.getGlobalBounds().top)
               + " - size : " + toString(_foot.getGlobalBounds().width) + ";" + toString(_foot.getGlobalBounds().height));

    Node& bodyNode = membersNode.firstChild("body");
    fillPartsFromNode(bodyNode, _body);
    attachChild(&_body);
    Log::write(Log::LOG_INFO, "Member part : Body texture region : " + toString(_body.getGlobalBounds().left) + ";" + toString(_body.getGlobalBounds().top)
               + " - size : " + toString(_body.getGlobalBounds().width) + ";" + toString(_body.getGlobalBounds().height));

    Node& headNode = membersNode.firstChild("head");
    fillPartsFromNode(headNode, _head);
    _body.attachChild(&_head);
    Log::write(Log::LOG_INFO, "Member part : Head texture region : " + toString(_head.getGlobalBounds().left) + ";" + toString(_head.getGlobalBounds().top)
               + " - size : " + toString(_head.getGlobalBounds().width) + ";" + toString(_head.getGlobalBounds().height));

    Node& armNode = membersNode.firstChild("arm");
    fillPartsFromNode(armNode, _arm);
    _body.attachChild(&_arm);
    Log::write(Log::LOG_INFO, "Member part : Arm texture region : " + toString(_arm.getGlobalBounds().left) + ";" + toString(_arm.getGlobalBounds().top)
               + " - size : " + toString(_arm.getGlobalBounds().width) + ";" + toString(_arm.getGlobalBounds().height));

}

void Player::fillPartsFromNode(Node& node, DrawableEntity& part)
{
    sf::IntRect area = loadAreaFromNode(node);
    ///@todo: currently no animations, but maybe in the future
    part.createAnimation(0, area, area, sf::seconds(1)); // no animation, so 0 is perfect and 1 second

    int oX = atoi(node.firstAttributeValue("originx").c_str());
    int oY = atoi(node.firstAttributeValue("originy").c_str());
    part.setOrigin(oX, oY);

    int x = atoi(node.firstAttributeValue("x").c_str());
    int y = atoi(node.firstAttributeValue("y").c_str());
    part.setPosition(x, y);
}


void Player::updateCurrent(sf::Time elapsedTime)
{
    /////////////////////////////////////
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        // if mouse button left is pressed, the character attacks
        attack(elapsedTime, Game::getContext().getApp().mapPixelToCoords(sf::Mouse::getPosition(Game::getContext().getApp())));
    /////////////////////////////////////

    // We get the mouse position relative to the window
    sf::Vector2f clicPos = Game::getContext().getApp().mapPixelToCoords(sf::Mouse::getPosition(Game::getContext().getApp()));

    // The points to track
    sf::Vector2f armOrigin = (_arm.getWorldTransform() * _arm.getOrigin());
    sf::Vector2f bodyOrigin = (_body.getWorldTransform() * _body.getOrigin());

    sf::Vector2f deltaArm = clicPos - armOrigin;
    sf::Vector2f deltaBody = clicPos - bodyOrigin;

    if(deltaArm.x == 0)
        deltaArm.x = 1; // avoid division by 0
    if(deltaBody.x == 0)
        deltaBody.x = 1; // avoid division by 0

    float bodyAngle = (std::atan(deltaBody.y / deltaBody.x) / (M_PI * 2)) * 360 / 4;
    // we substract the body angle because the body rotates too
    float armAngle = (std::atan(deltaArm.y / deltaArm.x) / (M_PI * 2)) * 360 - bodyAngle;

    ///@todo: fix here
    if(deltaArm.x > 0 || deltaBody.x > 0)
    {
        setScale(- std::abs(getScale().x), getScale().y);
        bodyAngle *= -1;
        armAngle *= -1;
    }
    else
        setScale(std::abs(getScale().x), getScale().y);

    _body.setRotation(bodyAngle);
    _arm.setRotation(armAngle);
}

void Player::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    // draw nothing because we don't want a player background
}

void Player::attack(sf::Time elapsedTime, sf::Vector2f location)
{
    getWeapon()->shoot(elapsedTime, location);
}

sf::FloatRect Player::getGlobalBounds() const
{
    ///@todo: if there's no animation currently, return the addition of every bounds
    return Character::getGlobalBounds();
}

sf::FloatRect Player::getLocalBounds() const
{
    ///@todo: how to deal with localbounds if there's no animation ?
    return Character::getLocalBounds();
}
