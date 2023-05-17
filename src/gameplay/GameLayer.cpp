#include "pch.h"
#include "GameLayer.h"

#include "gameplay/dialogue/Edge.h"
#include "gameplay/dialogue/Node.h"
#include "gameplay/dialogue/Tree.h"

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

// Game Layer
void GameLayer::Initialize()
{
    Layer::Initialize();

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
    }
}
