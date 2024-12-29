#include "Application.h"

#include <fstream>
#include <iostream>
#include <string>
#include <streambuf>
#include <unordered_map>

int main()
{
    Application App;
    App.Init();
    App.Run();
    App.DeInit();
    return 0;
}

