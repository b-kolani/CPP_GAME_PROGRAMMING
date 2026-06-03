#include "iostream"
#include "vector"

typedef enum class zombieTypes {
    RUNNER, CRAWLER
} zombieType;


// Declare the prototype of doSomething()
// function to allow the program to be aware 
// of the exact signature of the function
// and to ckeck if it matches the definition 
// signature 
void    doSomething();

int main()
{
    zombieType  z1 = zombieTypes::CRAWLER;
    zombieType  z2 = zombieTypes::RUNNER;

    const std::vector<int> V({1, 2, 3});
    float myFloatingPointArray[3]{3.14f, 1.63f, 99.0f};
    std::cout << std::vector<float>(myFloatingPointArray, myFloatingPointArray + 3).size() << std::endl;
    std::cout << std::vector<float>(std::begin(myFloatingPointArray), std::end(myFloatingPointArray)).size() << std::endl;
    std::cout << "V size: " << V.size() << std::endl;
    srand((unsigned int)time(NULL));
    std::cout << RAND_MAX << std::endl;
    std::cout << rand() % 100 << std::endl;
    std::cout << "z1 type:" << static_cast<int>(z1) << std::endl;
    std::cout << "z2 type:" << static_cast<int>(z2) << std::endl;
    doSomething();
    return 0;
}

void doSomething() {
    std::cout << "Do something" << std::endl;
}