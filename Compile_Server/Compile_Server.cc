#include "Complier.hpp"

using namespace ns_compiler;

int main()
{
    std::string code_filename = "code";
    Compiler::Compile(code_filename);
    return 0;
}