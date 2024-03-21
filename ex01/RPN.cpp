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

//문자, 연산기호 잘못, 두자리 수
//연산자 들어왔을 때 스택에 숫자 두개 
//끝났을때 하나 남았는지.
void RPN::calculate(const std::string& expression) {
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token) {
        if (token.length() == 1 && isdigit(token[0])) {
			// 토큰이 한 자리 숫자인 경우 스택에 push
            int num = token[0] - '0';
            stack.push(num);
        } else if (token.length() == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/')) {
            // 토큰이 연산자인 경우 스택에서 두 개의 피연산자를 pop하여 계산 후 결과를 다시 스택에 push
            
			// 피연산자가 2개 미만인 경우
			if (stack.size() < 2)
                throw std::runtime_error("Invalid expression: insufficient operands");

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
            }
        } else {
            // 토큰이 한 자리 숫자도 아니고 연산자도 아닌 경우 예외 처리
            throw std::runtime_error("Invalid token");
        }
    }

    // 계산 후 스택에 결과값 하나만 남아있어야 함
    if (stack.size() != 1)
        throw std::runtime_error("Invalid expression: too many operands");
}

int RPN::getResult() const {
    if (stack.empty())
        throw std::runtime_error("No result available");
    return stack.top();
}
