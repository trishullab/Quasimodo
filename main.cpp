#include <iostream>
#include "quantum_language_parser.h"
#include <chrono>
using namespace std::chrono;

int main(int argc, char* argv[])
{
    QuantumLanguageParser qlp = QuantumLanguageParser("inp_file.txt");
    auto start = high_resolution_clock::now();
    qlp.Execute();
    auto end = high_resolution_clock::now();
    std::cout << "Duration: " << duration_cast<milliseconds>(end - start).count() << std::endl;
    return 0;
}
