#include <memory>
#include "../../MQT_DD/dd_package/include/dd/Package.hpp"
#include <chrono>
#include <time.h>
using namespace dd;
using namespace std::chrono;

mEdge createHadamardMatrix(std::unique_ptr<Package<DDPackageConfig>>& ddp, unsigned int numVars, unsigned total_vars)
{
    if (numVars == 1)
    {
        return ddp->makeGateDD(dd::Hmat, 1, total_vars - 1);
    }
    else
    {
        mEdge H_i = createHadamardMatrix(ddp, numVars/2, total_vars/2);
        return ddp->kronecker(H_i, H_i, true);
    }
}


mEdge createNOTMatrix(std::unique_ptr<Package<DDPackageConfig>>& ddp, unsigned int numVars, unsigned total_vars)
{
    if (numVars == 1)
    {
        return ddp->makeGateDD(dd::Xmat, 1, total_vars - 1);
    }
    else
    {
        mEdge H_i = createNOTMatrix(ddp, numVars/2, total_vars/2);
        return ddp->kronecker(H_i, H_i, true);
    }
}

void testSyn1(unsigned int l)
{
    std::unique_ptr<Package<DDPackageConfig>> ddp;
    unsigned int numVars = std::pow(2, l);
    auto start = high_resolution_clock::now();
    ddp = std::make_unique<dd::Package<DDPackageConfig>>(numVars);
    // mEdge H = createHadamardMatrix(ddp, numVars, numVars);
    mEdge X = createNOTMatrix(ddp, numVars, numVars);
    mEdge I = ddp->makeIdent(numVars);
    mEdge ans = ddp->add(I, X);
    // mEdge ans = ddp->add(HI_IX, XH);
    // std::cout << ddp->size(ans) << std::endl;
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    std::cout << "Duration: " << duration.count() << " Memory: " << ddp->size(ans) << std::endl;
}

void testSyn2(unsigned int l)
{
    std::unique_ptr<Package<DDPackageConfig>> ddp;
    unsigned int numVars = std::pow(2, l);
    auto start = high_resolution_clock::now();
    ddp = std::make_unique<dd::Package<DDPackageConfig>>(numVars);
    Control c; c.qubit = numVars - 1;
    unsigned int target = 0;
    mEdge C1 = ddp->makeGateDD(dd::Xmat, numVars, c, target);
    Control c2; c2.qubit = numVars - numVars/2;
    unsigned int target2 = numVars - numVars/2 - 1;
    mEdge C2 = ddp->makeGateDD(dd::Xmat, numVars, c2, target2);

    mEdge C = ddp->multiply(C1, C2);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    std::cout << "Duration: " << duration.count() << " Memory: " << ddp->size(C) << std::endl;
}

void testSyn3(unsigned int l)
{
    std::unique_ptr<Package<DDPackageConfig>> ddp;
    unsigned int numVars = std::pow(2, l);
    auto start = high_resolution_clock::now();
    ddp = std::make_unique<dd::Package<DDPackageConfig>>(numVars);
    mEdge H = createHadamardMatrix(ddp, numVars, numVars);
    mEdge ans = ddp->multiply(H, H);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    std::cout << "Duration: " << duration.count() << " Memory: " << ddp->size(ans) << std::endl;
}

void testSyn4(unsigned int l)
{
    std::unique_ptr<Package<DDPackageConfig>> ddp;
    unsigned int numVars = std::pow(2, l);
    auto start = high_resolution_clock::now();
    ddp = std::make_unique<dd::Package<DDPackageConfig>>(numVars);
    mEdge H = createHadamardMatrix(ddp, numVars, numVars);
    mEdge X = createNOTMatrix(ddp, numVars, numVars);
    mEdge I = ddp->makeIdent(numVars);
    mEdge HI = ddp->multiply(H, I);
    mEdge IX = ddp->multiply(I, X);
    mEdge ans = ddp->add(HI, IX);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    std::cout << "Duration: " << duration.count() << " Memory: " << ddp->size(ans) << std::endl;
}

void testSyn5(unsigned int l)
{
    std::unique_ptr<Package<DDPackageConfig>> ddp;
    unsigned int numVars = std::pow(2, l);
    auto start = high_resolution_clock::now();
    ddp = std::make_unique<dd::Package<DDPackageConfig>>(numVars);
    mEdge H1 = createHadamardMatrix(ddp, numVars, numVars);
    mEdge H2 = H1;
    H2.w = Complex::min_sqrt_2;
    mEdge ans = ddp->add(H1, H2);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    std::cout << "Duration: " << duration.count() << " Memory: " << ddp->size(ans) << std::endl;
}

void testSyn6(unsigned int l)
{
    std::unique_ptr<Package<DDPackageConfig>> ddp;
    unsigned int numVars = std::pow(2, l);
    auto start = high_resolution_clock::now();
    ddp = std::make_unique<dd::Package<DDPackageConfig>>(numVars);
    mEdge H1 = createHadamardMatrix(ddp, numVars, numVars);
    mEdge H2 = H1;
    H2.w = Complex::min_sqrt_2;
    mEdge ans = ddp->add(H1, H2);
    mEdge X = createNOTMatrix(ddp, numVars, numVars);
    ans = ddp->add(ans, X);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    std::cout << "Duration: " << duration.count() << " Memory: " << ddp->size(ans) << std::endl;
}

void testSyn7(unsigned int l)
{
    std::unique_ptr<Package<DDPackageConfig>> ddp;
    unsigned int numVars = std::pow(2, l);
    auto start = high_resolution_clock::now();
    ddp = std::make_unique<dd::Package<DDPackageConfig>>(numVars);
    mEdge H1 = createHadamardMatrix(ddp, numVars, numVars);
    mEdge H2 = H1;
    H2.w = Complex::min_sqrt_2;
    mEdge X = createNOTMatrix(ddp, numVars, numVars);
    mEdge ans = ddp->add(H2, X);
    ans = ddp->add(H1, X);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    std::cout << "Duration: " << duration.count() << " Memory: " << ddp->size(ans) << std::endl;
}


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: ./a.out <benchmark> <size>" << std::endl;
    }
    struct rlimit limit;

    getrlimit (RLIMIT_STACK, &limit);
    limit.rlim_cur = limit.rlim_max;
    setrlimit(RLIMIT_STACK, &limit);
    unsigned int numVars = atoi(argv[2]);
    if (strcmp(argv[1], "testSyn2") == 0)
        testSyn2(numVars); 
    if (strcmp(argv[1], "testSyn1") == 0)
        testSyn1(numVars);
    if (strcmp(argv[1], "testSyn3") == 0)
        testSyn3(numVars);
    if (strcmp(argv[1], "testSyn4") == 0)
        testSyn4(numVars);
    if (strcmp(argv[1], "testSyn5") == 0)
        testSyn5(numVars);
    if (strcmp(argv[1], "testSyn6") == 0)
        testSyn6(numVars);
    if (strcmp(argv[1], "testSyn7") == 0)
        testSyn7(numVars);
    return 0;
}
