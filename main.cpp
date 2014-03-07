#include "Application.h"

int main(int argc, char* argv[])
{
    Application application(argc, argv);

    application.CreateWindow("Engine", 700, 700);
    application.BeginLoop();

    return 0;
}
