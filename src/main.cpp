#include "pch.h"

#include "engine/Application.h"

int main(int argc, char** argv)
{
    Log::Initialize();

    const auto application = MakeUnique<Application>(argc, argv);

    return application->Run();
}