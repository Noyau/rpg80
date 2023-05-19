#pragma once

#include "gameplay/questsystem/Node.h"

// Text Outcome
struct TextOutcome
{
    Node m_Node;
    std::string m_Text{};
};

// Simple Outcome
struct SimpleOutcome
{
    Node m_Node;
};
