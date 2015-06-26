////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "glcommon.h"
#include "Application.h"

/**
* Main entry point
*/
int main()
{
    bool pauseConsole = true;

    Random::Initialise();

    auto application = std::make_unique<Application>();
    if (application->Initialise())
    {
        pauseConsole = false;
        application->Run();
    }

    application->Release();

    if (pauseConsole)
    {
        std::cin.get();
    }

    return 0;
}