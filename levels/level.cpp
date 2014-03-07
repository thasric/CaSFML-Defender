#include "../config.hpp"

#include "level.hpp"
#include "../characters/knight.hpp"

/** \brief ctor
 */
Level::Level(sf::RenderWindow& app) : _app(app),
                                      _name("<Unamed level>"),
                                      _player(NULL)
{
}

/** \brief dtor
 */
Level::~Level()
{
    std::for_each(_waves.begin(), _waves.end(), Deallocator<Wave>());
    delete _player;
}

/** \brief Load the level's objects from a XML file with a proxy class
 *
 * \param filename const std::string&
 * \return void
 *
 * TO DO : Use the abstract class with a provider to set globally the parsing language
 */
void Level::loadFromFile(const std::string& filename)
{
    try
    {
        XMLNode root; // change the way to initialize the root node
        root.loadFromFile(filename);

        // The first node has to be "level"
        if(root.getName() != "level")
        {
            std::string error = "Error " + filename + " : first node has to be \"level\"";
            throw std::runtime_error(error);
        }

        // We fill the level's attributes

        // Firstly, the name
        _name = root.firstAttributeValue("name");

        buildLevel(root);

        // The waves
        Node& wavesNode = root.firstChild("waves");
        std::vector<Node*> waveNodes = wavesNode.getChildren("wave");
        Factory<Enemy>& enemyFactory = Factory<Enemy>::GetFactory(); // Factory to know what type to build

        // Creation of each wave
        for(std::vector<Node*>::iterator waveItr = waveNodes.begin() ; waveItr != waveNodes.end() ; ++waveItr)
        {
            // New enemies' wave !
            Wave* wave = new Wave();

            std::vector<Node*> enemyNodes = (*waveItr)->getChildren("enemy");
            // if empty, there is no wave
            if(!enemyNodes.empty())
            {
                // Fills with enemies !
                for(std::vector<Node*>::iterator enemyItr = enemyNodes.begin() ; enemyItr != enemyNodes.end() ; ++enemyItr)
                {
                    // Asks the factory for an instance of the asking class in the XML
                    sf::Texture& texture = *(Locator::getImageManager()->getTexture(IMAGES_PATH + (*enemyItr)->firstAttributeValue("file")));
                    //Enemy* enemy = enemyFactory.build((*enemyItr)->firstAttributeValue("class"), texture);
                    Enemy* enemy = new Knight(texture); ///@todo: find a way to get Factory back again

                    enemy->load((*enemyItr)->firstAttributeValue("file"));

                    ///@todo: automatic creation with the file
                    enemy->createAnimation(Enemy::BLINK, sf::IntRect(0, 0, 100, 100), sf::IntRect(0, 0, 50, 100), sf::seconds(1));
                    enemy->setAnimation(Enemy::BLINK);

                    ///@todo: dynamic hitbox
                    enemy->setHitbox(new BoundingBoxHitbox(static_cast<sf::FloatRect>(enemy->getRect())));
                    Log::write(Log::LOG_INFO, "Enemy's hitbox creation : " + toString(enemy->getRect().width) + ";"
                                                                      + toString(enemy->getRect().height));

                    // how to deal with positions ? In the file ?
                    enemy->setPosition(- enemy->getRect().height,
                                       VIEW_HEIGHT - enemy->getRect().height - _ground.getRect().height / 2);

                    wave->addEnemy(enemy);
                }

                _waves.push_back(wave);
            }
            else
                delete wave;
        }
    }
    catch(std::exception const& e)
    {
        Log::write(Log::LOG_ERROR, "Error : " + toString(e.what()));
    }
}

void Level::buildLevel(Node& root)
{
    // The background
    _background.load(root.firstChild("background").firstAttributeValue("file"));
    _background.setPosition(0, 0);

    // The ground
    Node& groundNode = root.firstChild("ground");
    _ground.load(groundNode.firstAttributeValue("file"));

    int x = 0, y = 0;
    /*_ground.setPosition(x, y);*/

    // The player
    Node& playerNode = root.firstChild("player");

    sf::Texture* playerTexture = Locator::getImageManager()->getTexture(IMAGES_PATH + playerNode.firstAttributeValue("file"));
    if(!playerTexture)
        throw std::runtime_error("Player's texture not found");
    _player = new Player(*playerTexture);

    Node& weaponNode = playerNode.firstChild("weapon"); ///@todo: multiple weapons please
    _player->load(playerNode.firstAttributeValue("file"), weaponNode.firstAttributeValue("file"));

    x = y = 0;
    x = atoi(playerNode.firstAttributeValue("x").c_str());
    y = atoi(playerNode.firstAttributeValue("y").c_str());
    _player->setPosition(x, y);

    // The castle
    Node& castleNode = root.firstChild("castle");
    _castle.load(castleNode.firstAttributeValue("file"));

    int offsetX = 0, offsetY = 0;
    try
    {
        std::string offsetXStr = castleNode.firstAttributeValue("offsetx");
        offsetX = offsetXStr != "" ? atoi(offsetXStr.c_str()) : 0;
    }
    catch(std::runtime_error& e)
    { /* if offsetX are not there, it's not a problem */ }

    try
    {
        std::string offsetYStr = castleNode.firstAttributeValue("offsety");
        offsetY = offsetYStr != "" ? atoi(offsetYStr.c_str()) : 0;
    }
    catch(std::runtime_error& e)
    { /* if offsetY are not there, it's not a problem */ }

    std::string position = castleNode.firstAttributeValue("position");
    if(position == "left")
        x = std::abs(offsetX);
    else if(position == "middle")
        x = VIEW_WIDTH / 2 - _castle.getRect().width / 2 + offsetX;
    else // by default it's right
        x = VIEW_WIDTH - _castle.getRect().width - std::abs(offsetX);

    y = VIEW_HEIGHT - _castle.getRect().height - std::abs(offsetY);
    _castle.setPosition(x, y);
    _castle.getHitbox()->setPosition(x, y);
    Log::write(Log::LOG_INFO, std::string("Loading castle to the " + position + " : offsets : " + toString(offsetX) + ";" + toString(offsetX)
                                          + " - position " + toString(_castle.getPosition().x) + "x" + toString(_castle.getPosition().y) + " - size "
                                          + toString(_castle.getRect().width) + ";" + toString(_castle.getRect().height)));
    Log::write(Log::LOG_INFO, std::string("Castle sprite hitbox : position " + toString(x) + ";" + toString(y) + " - size "
                                          + toString(_castle.getRect().width) + ";" + toString(_castle.getRect().height)));
}

Wave* Level::getNextWave()
{
    if(_waves.empty())
        return NULL;

    Wave* response = _waves.front();
    _waves.erase(_waves.begin());

    return response;
}
