#include <iostream>
#include <string>
#include "wrapper.h"
#include "eng.h"
#include "subj.h"

int main() 
{
    Subject ex;

    wrapper wrapper(&ex, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });
    eng eng_ex;

    eng_ex.comand(&wrapper, "command1");

    std::cout << eng_ex.execute("command1", { {"arg1", 4}, {"arg2", 5} }) << std::endl;

    return 0;
}
