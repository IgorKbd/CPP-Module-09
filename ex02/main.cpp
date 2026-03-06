/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikayiban <ikayiban@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:58:02 by ikayiban          #+#    #+#             */
/*   Updated: 2026/01/19 12:55:20 by ikayiban         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " \"positive integers\"" << std::endl;
        return 1;
    }

    std::vector<int> input;
    
    if (!validate_and_build_input(argv, input))
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    
    print_before(argv);

    unsigned long size = input.size();
    
    long t0 = get_time_us();
    std::vector<int> resultVec = fordJohnsonVector(input);
    long t1 = get_time_us();

    long t2 = get_time_us();
    std::deque<int> resultDeq = fordJohnsonDeque(input);
    long t3 = get_time_us();

    std::cout << "After (vector): ";
    for (size_t i = 0; i < resultVec.size(); ++i)
    {
        std::cout << resultVec[i];
        if (i + 1 < resultVec.size()) std::cout << " ";
    }
    std::cout << std::endl;

    std::vector<int> resultDeqVec = deque_to_vector(resultDeq);
    std::cout << "After (deque) : ";
    for (size_t i = 0; i < resultDeqVec.size(); ++i)
    {
        std::cout << resultDeqVec[i];
        if (i + 1 < resultDeqVec.size()) std::cout << " ";
    }
    std::cout << std::endl;

    bool okVec = isSorted(resultVec) && size == resultDeq.size();
    bool okDeq = isSorted(resultDeqVec) && size == resultDeqVec.size();

    std::cout << "Time to process a range of " << input.size() << " elements with std::vector : " << (t1 - t0) << " us" << std::endl;

    std::cout << "Time to process a range of " << input.size() << " elements with std::deque  : " << (t3 - t2) << " us" << std::endl;
    
    if (okVec && okDeq)
        std::cout << "Correctly sorted!!!" << std::endl;
    else
        std::cout << "Not sorted!!!" << std::endl;

    return (okVec && okDeq) ? 0 : 2;
}

