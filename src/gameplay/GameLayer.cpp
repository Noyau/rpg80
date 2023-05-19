#include "pch.h"
#include "GameLayer.h"

#include "gameplay/scene/Entity.h"
#include "gameplay/scene/Scene.h"

#include "gameplay/questsystem/Node.h"
#include "gameplay/questsystem/Outcome.h"
#include "gameplay/questsystem/Quest.h"

#include "gameplay/components/CombatComponent.h"
#include "gameplay/components/DialogueComponent.h"
#include "gameplay/components/TextComponent.h"

// Game Layer
void GameLayer::Initialize()
{
    Layer::Initialize();

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
    }
}
