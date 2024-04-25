#include "Complier.hpp"
#include "Runner.hpp"

using namespace ns_compiler;
using namespace ns_runner;

int main()
{
    std::string code_filename = "code";
    Compiler::Compile(code_filename);
    Runner::Run(code_filename);
    return 0;
}