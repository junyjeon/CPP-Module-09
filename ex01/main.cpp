#include "RPN.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Error: Wrong number of arguments." << std::endl;
        std::cout << "Usage: " << argv[0] << " <expression>" << std::endl;
        return 1;
    }

    std::string expression = argv[1];
    RPN rpn;

    try {
        rpn.calculate(expression);
        int result = rpn.getResult();
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}