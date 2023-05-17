#include "pch.h"

#include "engine/Application.h"

#include "gameplay/GameLayer.h"

int main(int argc, char** argv)
{
    Log::Initialize();

    const auto application = MakeUnique<Application>(argc, argv);

    application->AddLayer<GameLayer>();

    return application->Run();
}
