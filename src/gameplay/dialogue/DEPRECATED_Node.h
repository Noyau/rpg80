#pragma once

struct CombatNode;
struct DialogueNode;
struct NarrativeNode;

// Node Visitor
class NodeVisitor
{
public:
    virtual void Display(const CombatNode&) const = 0;
    virtual void Display(const DialogueNode&) const = 0;
    virtual void Display(const NarrativeNode&) const = 0;
};

#define NODE_VISIT_IMPLEMENTATION() \
    void Display(const NodeVisitor& visitor) const override { visitor.Display(*this); }

// DEPRECATED_Node
struct DEPRECATED_Node
{
    virtual void Display(const NodeVisitor& visitor) const = 0;
};

// Narrative Node
struct NarrativeNode : DEPRECATED_Node
{
    std::string m_Text{};

    NarrativeNode() = default;
    NarrativeNode(std::string&& text)
        : m_Text(std::move(text))
    {}

    NODE_VISIT_IMPLEMENTATION()
};

// Dialogue Node
struct DialogueNode : DEPRECATED_Node
{
    std::string m_CharacterName{};
    std::string m_CharacterDescription{};
    std::string m_CharacterText{};

    DialogueNode() = default;
    DialogueNode(std::string&& charName, std::string&& charDesc, std::string&& charText)
        : m_CharacterName(std::move(charName))
        , m_CharacterDescription(std::move(charDesc))
        , m_CharacterText(std::move(charText))
    {}

    NODE_VISIT_IMPLEMENTATION()
};

// Combat Node
struct CombatNode : DEPRECATED_Node
{
    std::string m_EnemyName{};
    std::string m_EnemyDescription{};
    std::string m_EnemyText{};

    CombatNode() = default;
    CombatNode(std::string&& nmeName, std::string&& nmeDesc, std::string&& nemText)
        : m_EnemyName(std::move(nmeName))
        , m_EnemyDescription(std::move(nmeDesc))
        , m_EnemyText(std::move(nemText))
    {}

    NODE_VISIT_IMPLEMENTATION()
};
