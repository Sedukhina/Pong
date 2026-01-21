#include "Pong.h"
#include "Log.h"

int main()
{
    try 
    {
        Pong PongInst{};
        PongInst.RunPong();
    }
    catch (const std::exception& e) 
    {
        return EXIT_FAILURE;
    }
    return 0;
}