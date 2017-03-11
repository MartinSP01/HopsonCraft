#include "Block_Editor.h"

#include <SFML/Window/Mouse.hpp>


#include "Chunk/CMap.h"
#include "../HUD/Crosshair.h"
#include "../Maths/General_Maths.h"
#include "../Physics/Ray.h"
#include "../Player/Player.h"


float delay = 0.2f;

Block_Editor::Block_Editor(Chunk::Map& chunkMap)
:   m_p_chunkMap    (&chunkMap)
{ }

void Block_Editor::input(Player& player, Crosshair& crosshair)
{
    Ray raycast (player.rotation.y + 90,
                 player.rotation.x,
                 player.position);

    for (uint32_t i = 0; i < 5 / 0.04; i++)
    {
        raycast.step(0.1);

        if (raycast.getEndPoint().x < 0 ||
            raycast.getEndPoint().z < 0 ||
            raycast.getEndPoint().y < 1 ) return;

        if(isHitBlock(raycast))
        {
            crosshair.setMode(Crosshair::Mode::Hit);
            if (editBlock(raycast, player))
                break;
        }
        else
        {
            crosshair.setMode(Crosshair::Mode::Reg);
        }
        m_lastRayPosition = raycast.getEndPoint();
    }
}

bool Block_Editor::isHitBlock(const Ray& ray) const
{
    CBlock block = m_p_chunkMap->getBlockAt(ray.getEndPoint());
    return !(block == Block::ID::Air);
}

bool Block_Editor::editBlock(const Ray& ray, Player& player)
{
    if(m_editorTimer.getElapsedTime().asSeconds() >= delay)
    {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            m_editorTimer.restart();
            m_p_chunkMap->breakBlock(ray.getEndPoint());
            return true;
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            m_editorTimer.restart();

            auto playerBlockPos = Maths::worldToBlockPos(player.position);
            auto editPos        = Maths::worldToBlockPos(m_lastRayPosition);


            if(playerBlockPos != editPos)
            {
                m_p_chunkMap->addBlock(m_lastRayPosition, Block::ID::Grass);
            }
            return true;
        }
    }
    return false;
}










