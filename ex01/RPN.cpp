/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikayiban <ikayiban@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 09:38:26 by ikayiban          #+#    #+#             */
/*   Updated: 2026/01/19 10:16:23 by ikayiban         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

bool isOperator(const std::string &s)
{
    return (s == "+" || s == "-" || s == "*" || s == "/");
}

bool isInt(const std::string &s)
{
    if (s.empty())
        return false;
    if (s.size() == 1 && (s[0] == '+' || s[0] == '-'))
        return false;

    size_t i = 0;
    if (s[0] == '+' || s[0] == '-')
        i++;
    if (i == s.size())
        return false;
    while (i < s.size())
    {
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return false;
        i++;
    }
    return true;
}

bool    do_op(std::stack<int>& stack, std::string token)
{
    int b = stack.top();
    stack.pop();
    int a = stack.top();
    stack.pop();
    int res = 0;
    if (token == "+")
        res = a + b;
    else if (token == "-")
        res = a - b;
    else if (token == "*")
        res = a * b;
    else if (token == "/")
    {
        if (b == 0)
        {
            std::cerr << "Error" << std::endl;
            return false;
        }
        res = a / b;
    }
    stack.push(res);
    return true;
}

bool parsing(char *s)
{
    std::string input(s);
    std::istringstream iss(input);
    std::string token;
    std::stack<int> st;

    while (iss >> token)
    {
        if (isInt(token))
        {
            std::istringstream conv(token);
            int value;
            conv >> value;
            if (value < 0 || value > 9)
            {
                std::cerr << "Error" << std::endl;
                return false;
            }
            st.push(value);
        }
        else if (isOperator(token))
        {
            if (st.size() < 2)
            {
                std::cerr << "Error" << std::endl;
                return false;
            }
            if (!do_op(st, token))
                return false;
        }
        else
        {
            std::cerr << "Error" << std::endl;
            return false;
        }
    }

    if (st.size() != 1)
    {
        std::cerr << "Error" << std::endl;
        return false;
    }

    std::cout << st.top() << std::endl;
    return true;
}

