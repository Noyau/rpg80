#include "pch.h"
#include "GameLayer.h"

#ifdef OLD_QUEST
#include "gameplay/dialogue/Edge.h"
#include "gameplay/dialogue/DEPRECATED_Node.h"
#include "gameplay/dialogue/Tree.h"
#endif

#include "gameplay/scene/Entity.h"
#include "gameplay/scene/Scene.h"

#include "gameplay/questsystem/Node.h"
#include "gameplay/questsystem/Outcome.h"
#include "gameplay/questsystem/Quest.h"

#include "gameplay/components/CombatComponent.h"
#include "gameplay/components/DialogueComponent.h"
#include "gameplay/components/TextComponent.h"

#ifdef OLD_QUEST
// Game Node Visitor
class GameNodeVisitor : public NodeVisitor
{
public:
    void Display(const CombatNode& node) const override
    {
        ImGui::TextWrapped(Format("[{}]", node.m_EnemyName).c_str());
        ImGui::TextWrapped(Format("   {}", node.m_EnemyDescription).c_str());
        ImGui::TextWrapped(Format("-> {}", node.m_EnemyText).c_str());
    }

    void Display(const DialogueNode& node) const override
    {
        ImGui::TextWrapped(Format("[{}] ({})", node.m_CharacterName, node.m_CharacterDescription).c_str());
        ImGui::TextWrapped(Format("[{}] {}", node.m_CharacterName, node.m_CharacterText).c_str());
    }

    void Display(const NarrativeNode& node) const override
    {
        ImGui::TextWrapped(node.m_Text.c_str());
    }
};
#endif

// Game Layer
void GameLayer::Initialize()
{
    Layer::Initialize();

#ifndef OLD_QUEST
    m_Quest = MakeShared<Quest>();
    {
        std::array nodes
        {
            m_Quest->CreateNode(),
            m_Quest->CreateNode(),
            m_Quest->CreateNode(),
            m_Quest->CreateNode(),
            m_Quest->CreateNode(),
            m_Quest->CreateNode(),
            m_Quest->CreateNode(),
            m_Quest->CreateNode(),
            m_Quest->CreateNode(),
            m_Quest->CreateNode()
        };

        nodes[0].AddComponent<TextComponent>("Bonjour aventurier !");
        nodes[1].AddComponent<TextComponent>("Droit devant vous se tient un vieil homme, parlez lui.");
        nodes[2].AddComponent<DialogueComponent>("Pedro Bear", "Porte un grand manteau mais pas de... pantalon ?", "Tu as déjà vu un monsieur tout nu ?");
        nodes[3].AddComponent<TextComponent>("Soudain le vieil homme ouvre son grand manteau d'un geste si vif que vous parvenez à peine à le distinguer...");
        nodes[4].AddComponent<CombatComponent>("Pedro « Maître Biffleur » Bear", "Ah le vicieux ! Il est vif !", "L'ennemie a l'initiative et vous inflige une « Méga-biffle des Bois », tout devient noir...");
        nodes[5].AddComponent<TextComponent>("La légende raconte que des fantômes hantent les lieux et dévorent l'âme des imprudents.");
        nodes[6].AddComponent<TextComponent>("Mais vous vous connaissez la vérité.");
        nodes[7].AddComponent<TextComponent>("Malheureusement vous êtes morts.");
        nodes[8].AddComponent<TextComponent>("Merci d'avoir joué.");
        nodes[9].AddComponent<TextComponent>("Bisous");

        for (uint32_t i = 1; i < nodes.size(); ++i)
        {
            nodes[i - 1].AddOutcome<SimpleOutcome>(nodes[i]);
        }

        m_InitialNode = m_CurrentNode = MakeShared<Node>(nodes[0]);
    }
#else
    // Just a simple test... ahem...
    // So many things to do...
    m_Tree = MakeUnique<Tree>();
    {
        m_Tree->m_Nodes.emplace_back(MakeShared<NarrativeNode>("Bonjour aventurier !"));
        m_Tree->m_Nodes.emplace_back(MakeShared<NarrativeNode>("Droit devant vous se tient un vieil homme, parlez lui."));
        m_Tree->m_Nodes.emplace_back(MakeShared<DialogueNode>("Pedro Bear", "Porte un grand manteau mais pas de... pantalon ?", "Tu as déjà vu un monsieur tout nu ?"));
        m_Tree->m_Nodes.emplace_back(MakeShared<NarrativeNode>("Soudain le vieil homme ouvre son grand manteau d'un geste si vif que vous parvenez à peine à le distinguer..."));
        m_Tree->m_Nodes.emplace_back(MakeShared<CombatNode>("Pedro « Maître Biffleur » Bear", "Ah le vicieux ! Il est vif !", "L'ennemie a l'initiative et vous inflige une « Méga-biffle des Bois », tout devient noir..."));
        m_Tree->m_Nodes.emplace_back(MakeShared<NarrativeNode>("La légende raconte que des fantômes hantent les lieux et dévorent l'âme des imprudents."));
        m_Tree->m_Nodes.emplace_back(MakeShared<NarrativeNode>("Mais vous vous connaissez la vérité."));
        m_Tree->m_Nodes.emplace_back(MakeShared<NarrativeNode>("Malheureusement vous êtes morts."));
        m_Tree->m_Nodes.emplace_back(MakeShared<NarrativeNode>("Merci d'avoir joué."));
        m_Tree->m_Nodes.emplace_back(MakeShared<NarrativeNode>("Bisous"));
    }
    {
        m_Tree->m_Edges.emplace_back(0, 1);
        m_Tree->m_Edges.emplace_back(1, 2);
        m_Tree->m_Edges.emplace_back(2, 3);
        m_Tree->m_Edges.emplace_back(3, 4);
        m_Tree->m_Edges.emplace_back(4, 5);
        m_Tree->m_Edges.emplace_back(5, 6);
        m_Tree->m_Edges.emplace_back(6, 7);
        m_Tree->m_Edges.emplace_back(7, 8);
        m_Tree->m_Edges.emplace_back(8, 9);
    }
#endif

    LOG_INFO("RPG-80");
    LOG_INFO("Vous pouvez quitter le programme à tout moment en appuyant sur la touche (plan) 'Q'.");
    LOG_INFO("(Mais en vrai, faites ALT+F4, on se sens plus puissant quand on a droit de vie ou de mort sur les processus.)");
    LOG_INFO("Préparez vous à une aventure sans pareil !");
    LOG_INFO("------------------------------------------");
}

void GameLayer::Shutdown()
{
    LOG_INFO("------------------------------------------");
    LOG_INFO("À bientôt.");

    Layer::Shutdown();
}

void GameLayer::Update()
{
    Layer::Update();
}

void GameLayer::RenderImGui(const ImGuiIO& io)
{
    Layer::RenderImGui(io);

    IMGUI_WINDOW("Game")
    {
#ifndef OLD_QUEST
        if (m_CurrentNode != nullptr)
        {
            if (m_CurrentNode->HasComponent<TextComponent>())
            {
                const auto text = m_CurrentNode->GetComponent<TextComponent>();

                ImGui::TextWrapped(text.m_Text.c_str());
            }
            else if (m_CurrentNode->HasComponent<DialogueComponent>())
            {
                const auto dialogue = m_CurrentNode->GetComponent<DialogueComponent>();

                ImGui::TextWrapped(Format("[{}] ({})", dialogue.m_ActorName, dialogue.m_ActorDescription).c_str());
                ImGui::TextWrapped(Format("[{}] {}", dialogue.m_ActorName, dialogue.m_Text).c_str());
            }
            else if (m_CurrentNode->HasComponent<CombatComponent>())
            {
                const auto combat = m_CurrentNode->GetComponent<CombatComponent>();

                ImGui::TextWrapped(Format("[{}]", combat.m_ActorName).c_str());
                ImGui::TextWrapped(Format("   {}", combat.m_ActorDescription).c_str());
                ImGui::TextWrapped(Format("-> {}", combat.m_Attack).c_str());
            }

            if (m_CurrentNode->HasOutcome<SimpleOutcome>())
            {
                if (ImGui::Button("Suite"))
                {
                    const auto outcome = m_CurrentNode->GetOutcome<SimpleOutcome>();

                    m_CurrentNode = MakeShared<Node>(outcome.m_Node);
                }
            }
            else if (m_CurrentNode->HasOutcome<TextOutcome>())
            {
                const auto outcome = m_CurrentNode->GetOutcome<TextOutcome>();

                if (ImGui::Button(outcome.m_Text.c_str()))
                {
                    m_CurrentNode = MakeShared<Node>(outcome.m_Node);
                }
            }
            else if (ImGui::Button("Recommencer"))
            {
                m_CurrentNode = m_InitialNode;
            }
        }
#else
        const auto& node = m_Tree->m_Nodes.at(m_NodeId);

        node->Display(GameNodeVisitor{});

        const auto it = std::ranges::find(m_Tree->m_Edges, m_NodeId, &Edge::m_SourceId);

        if (it != m_Tree->m_Edges.end())
        {
            if (ImGui::Button("Suite"))
            {
                m_NodeId = it->m_TargetId;
            }
        }
        else if (ImGui::Button("Recommencer"))
        {
            m_NodeId = 0;
        }
#endif
    }
}
