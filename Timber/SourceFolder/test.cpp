#include "iostream"
#include "vector"

int main()
{
    const std::vector<int> V({1, 2, 3});
    float myFloatingPointArray[3]{3.14f, 1.63f, 99.0f};
    std::cout << std::vector<float>(myFloatingPointArray, myFloatingPointArray + 3).size() << std::endl;
    std::cout << std::vector<float>(std::begin(myFloatingPointArray), std::end(myFloatingPointArray)).size() << std::endl;
    std::cout << "V size: " << V.size() << std::endl;
    srand((unsigned int)time(NULL));
    std::cout << RAND_MAX << std::endl;
    std::cout << rand() % 100 << std::endl;
    return 0;
}