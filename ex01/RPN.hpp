#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <stdexcept>
#include <sstream>

class RPN
{
public:
    RPN();
    RPN(RPN const &other);
    RPN &operator=(RPN const &other);
    ~RPN();

    void calculate(const std::string& expression);
    int getResult() const;

private:
    std::stack<int> stack;
};

#endif // RPN_HPP

/*
토큰을 받아서 스택에 넣고,
연산자를 만나면 스택에서 피연산자 두개를 꺼내서 연산을 하고
결과를 다시 스택에 넣는다.

1. to
*/