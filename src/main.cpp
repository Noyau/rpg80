#include "pch.h"

#include "engine/Application.h"

int main(int argc, char** argv)
{
    auto application = MakeUnique<Application>(argc, argv);

    return application->Run();
}