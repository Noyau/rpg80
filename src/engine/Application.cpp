#include "pch.h"
#include "Application.h"

#include "engine/Layer.h"
#include "engine/Window.h"

#include "engine/imgui/ImGuiLayer.h"

#include "gameplay/dialogue/Edge.h"
#include "gameplay/dialogue/Node.h"
#include "gameplay/dialogue/Tree.h"

// Application
Application::Application(int argc, char** argv)
{
    s_Instance = this;
}

Application::~Application()
{
    s_Instance = nullptr;
}

class AppNodeVisitor : public NodeVisitor
{
public:
    void Display(const CombatNode& node) const override
    {
        std::cout << Format("[{}]\n", node.m_EnemyName);
        std::cout << Format("   {}", node.m_EnemyDescription);
        std::cout << Format("-> {}", node.m_EnemyText);
        std::cout << std::endl;
    }

    void Display(const DialogueNode& node) const override
    {
        std::cout << Format("[{}] ({})\n", node.m_CharacterName, node.m_CharacterDescription);
        std::cout << Format("[{}] {}", node.m_CharacterName, node.m_CharacterText);
        std::cout << std::endl;
    }

    void Display(const NarrativeNode& node) const override
    {
        std::cout << node.m_Text;
        std::cout << std::endl;
    }
};

int Application::Run()
{
    if (m_Window = Window::Create(WindowArgs{ 800, 600, "RPG-80" }); m_Window != nullptr)
    {
        m_ImGuiLayer = AddLayer<ImGuiLayer>();

        while (!m_Window->ShouldClose())
        {
            std::ranges::for_each(m_Layers, &Layer::Update);

            m_Window->Update();
        }

        std::ranges::for_each(m_Layers, &Layer::Shutdown);

        m_Layers.clear();

        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;

#if false
    // Set console code page to UTF-8 so console known how to interpret string data
    //SetConsoleOutputCP(CP_UTF8);

    // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
    //setvbuf(stdout, nullptr, _IOFBF, 1000);

    // Just a simple test... ahem...
    // So many things to do...
    Tree tree;
    {
        tree.m_Nodes.emplace_back(MakeShared<NarrativeNode>("Bonjour aventurier !"));
        tree.m_Nodes.emplace_back(MakeShared<NarrativeNode>("Droit devant vous se tient un vieil homme, parlez lui."));
        tree.m_Nodes.emplace_back(MakeShared<DialogueNode>("Pedro Bear", "Porte un grand manteau mais pas de... pantalon ?", "Tu as déjà vu un monsieur tout nu ?"));
        tree.m_Nodes.emplace_back(MakeShared<NarrativeNode>("Soudain le vieil homme ouvre son grand manteau d'un geste si vif que vous parvenez à peine à le distinguer..."));
        tree.m_Nodes.emplace_back(MakeShared<CombatNode>("Pedro « Maître Biffleur » Bear", "Ah le vicieux ! Il est vif !", "L'ennemie a l'initiative et vous inflige une « Méga-biffle des Bois », tout devient noir..."));
        tree.m_Nodes.emplace_back(MakeShared<NarrativeNode>("La légende raconte que des fantômes hantent les lieux et dévorent l'âme des imprudents."));
        tree.m_Nodes.emplace_back(MakeShared<NarrativeNode>("Mais vous vous connaissez la vérité."));
        tree.m_Nodes.emplace_back(MakeShared<NarrativeNode>("Malheureusement vous êtes morts."));
        tree.m_Nodes.emplace_back(MakeShared<NarrativeNode>("Merci d'avoir joué."));
        tree.m_Nodes.emplace_back(MakeShared<NarrativeNode>("Bisous"));
    }
    {
        tree.m_Edges.emplace_back(0, 1);
        tree.m_Edges.emplace_back(1, 2);
        tree.m_Edges.emplace_back(2, 3);
        tree.m_Edges.emplace_back(3, 4);
        tree.m_Edges.emplace_back(4, 5);
        tree.m_Edges.emplace_back(5, 6);
        tree.m_Edges.emplace_back(6, 7);
        tree.m_Edges.emplace_back(7, 8);
        tree.m_Edges.emplace_back(8, 9);
    }

    LOG_INFO("RPG-80");
    LOG_INFO("Vous pouvez quitter le programme à tout moment en appuyant sur la touche (plan) 'Q'.");
    LOG_INFO("(Mais en vrai, faites ALT+F4, on se sens plus puissant quand on a droit de vie ou de mort sur les processus.)");
    LOG_INFO("Préparez vous à une aventure sans pareil !");
    LOG_INFO("------------------------------------------");

    bool shouldQuit = false;

    uint32_t nodeId = 0;

    do
    {
        const auto& node = tree.m_Nodes.at(nodeId);

        node->Display(AppNodeVisitor{});

        char key = std::tolower(getchar());

        shouldQuit = key == 'q';

        if (!shouldQuit)
        {
            auto it = std::ranges::find_if(tree.m_Edges, [nodeId](const Edge& edge)
                {
                    return edge.m_SourceId == nodeId;
                }
            );

            if (it != tree.m_Edges.end())
            {
                nodeId = it->m_TargetId;
                continue;
            }

            std::cout << "------------------------------------------\n";
            std::cout << "Souhaitez-vous continuer ? [O]ui/[n]on";
            std::cout << std::endl;

            do
            {
                key = std::tolower(getchar());
            } while (key != 'o' && key != 'n' && key != 'q');

            switch (key)
            {
            case 'o':
                break;
            case 'n':
            case 'q':
                shouldQuit = true;
                break;
            }

            std::cout << "Ok\n";

            if (!shouldQuit)
            {
                nodeId = 0;
                std::cout << "------------------------------------------\n";
            }

        }
    } while (!shouldQuit);

    LOG_INFO("------------------------------------------");
    LOG_INFO("À bientôt.");

    return EXIT_SUCCESS;
#endif
}

const SharedPtr<Layer>& Application::AddLayer(SharedPtr<Layer>&& layer)
{
    const auto& addedLayer = m_Layers.emplace_back(std::move(layer));

    addedLayer->Initialize();

    return addedLayer;
}

void Application::RemoveLayer(const SharedPtr<Layer>& layer)
{
    if (const auto it = std::ranges::remove(m_Layers, layer).begin(); it != m_Layers.end())
    {
        (*it)->Shutdown();

        m_Layers.erase(it);
    }
}
