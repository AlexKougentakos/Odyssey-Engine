﻿#include "ColliderComponent.h"
#include <Box2D/b2_fixture.h>

ody::ColliderComponent::ColliderComponent(const glm::vec2& dimensions, ColliderSettings settings):
m_Dimensions(dimensions), m_Settings(settings)
{
    
}

void ody::ColliderComponent::InitializeFilter()
{
    b2Filter filter{};
    filter.categoryBits = static_cast<uint16>(m_Settings.collisionGroup); //Set the collision group of the player
    filter.maskBits = 0xFFFF;  // collide with everything
    filter.groupIndex = 0;  // no specific group

    m_pRuntimeFixture->SetFilterData(filter);
}

void ody::ColliderComponent::AddIgnoreGroup(constants::CollisionGroups category) 
{
    b2Filter filter = m_pRuntimeFixture->GetFilterData();

    // Use bitwise AND with the inverse of the category to clear that bit
    filter.maskBits &= ~static_cast<uint16>(category);
    m_pRuntimeFixture->SetFilterData(filter);
}

void ody::ColliderComponent::RemoveIgnoreGroup(constants::CollisionGroups category) 
{
    b2Filter filter = m_pRuntimeFixture->GetFilterData();

    // Use bitwise OR to set the category bit
    filter.maskBits |= static_cast<uint16>(category);
    m_pRuntimeFixture->SetFilterData(filter);
}