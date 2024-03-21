#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(RPN const &other) {
    *this = other;
}

RPN &RPN::operator=(RPN const &other) {
    if (this != &other) {
        this->stack = other.stack;
    }
    return *this;
}

RPN::~RPN() {}

void RPN::calculate(const std::string& expression) {
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token) {
        if (isdigit(token[0])) {
            // 토큰이 숫자인 경우 스택에 push
            int num = std::atoi(token.c_str());
            stack.push(num);
        } else {
            // 토큰이 연산자인 경우 스택에서 두 개의 피연산자를 pop하여 계산 후 결과를 다시 스택에 push
            if (stack.size() < 2)
                throw std::runtime_error("Invalid expression");

            int operand2 = stack.top();
            stack.pop();
            int operand1 = stack.top();
            stack.pop();

            switch (token[0]) {
                case '+':
                    stack.push(operand1 + operand2);
                    break;
                case '-':
                    stack.push(operand1 - operand2);
                    break;
                case '*':
                    stack.push(operand1 * operand2);
                    break;
                case '/':
                    stack.push(operand1 / operand2);
                    break;
                default:
                    throw std::runtime_error("Invalid operator");
            }
        }
    }

    // 계산 후 스택에 결과값 하나만 남아있어야 함
    if (stack.size() != 1)
        throw std::runtime_error("Invalid expression");
}

int RPN::getResult() const {
    if (stack.empty())
        throw std::runtime_error("No result available");
    return stack.top();
}
