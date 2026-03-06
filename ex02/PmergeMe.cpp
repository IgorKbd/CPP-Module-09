/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikayiban <ikayiban@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:58:06 by ikayiban          #+#    #+#             */
/*   Updated: 2026/03/06 10:00:51 by ikayiban         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

void print_before(char **argv)
{
    int i = 1;

    std::cout << "Before: ";

    while (argv[i])
    {
        std::string s = argv[i];
        std::cout << s << " ";
        ++i;
    }

    std::cout << std::endl;
}

bool validate_and_build_input(char **argv, std::vector<int> &out)
{
    int i = 1;

    while (argv[i])
    {
        std::string s = argv[i];
        if (s.size() == 0)
            return false;

        for (size_t j = 0; j < s.size(); ++j)
        {
            if (!std::isdigit(s[j]))
                return false;
        }

        std::stringstream ss(s);
        long val;
        ss >> val;        

        if (ss.fail() || val < 0 || val > INT_MAX)
            return false;

        for (size_t j = 0; j < out.size(); ++j)
        {
            if (out[j] == (int)val)
                return false;
        }
        out.push_back((int)val);
        ++i;
    }

    return true;
}

static std::vector<size_t> generateJacobsthalOrder(size_t n)
{
    std::vector<size_t> order;
    if (n == 0)
        return order;
        
    const size_t jacobsthal[] = {0, 1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101};

    size_t k = 1;
    size_t prev_j = 0;

    while (k < sizeof(jacobsthal)/sizeof(size_t) && jacobsthal[k] <= n)
    {
        size_t curr_j = jacobsthal[k];
        
        for (size_t i = curr_j; i > prev_j; --i)
        {
            if (i - 1 < n)
                order.push_back(i - 1);
        }

        prev_j = curr_j;
        k++;
    }
    
    for (size_t i = n; i > prev_j; --i)
        order.push_back(i - 1);

    return order;
}

static std::vector< std::pair<int, int> > createPairsVector(const std::vector<int> &elems, int &straggler)
{
    std::vector< std::pair<int, int> > pairs;

    straggler = -1;
    size_t i = 0;

    for (; i + 1 < elems.size(); i += 2)
    {
        int a = elems[i];
        int b = elems[i + 1];

        if (a < b)
            pairs.push_back(std::make_pair(a, b));
        else
            pairs.push_back(std::make_pair(b, a));
    }
    
    if (i < elems.size())
        straggler = elems[i];

    return pairs;
}

std::vector<int> fordJohnsonVector(const std::vector<int> &input)
{
    size_t n = input.size();

    if (n <= 1)
        return input;

    int straggler;
    std::vector< std::pair<int, int> > pairs = createPairsVector(input, straggler);

    std::vector<int> larger;
    larger.reserve(pairs.size());

    for (size_t i = 0; i < pairs.size(); ++i)
        larger.push_back(pairs[i].second);

    std::vector<int> sortedLarger = fordJohnsonVector(larger);
    std::vector<int> mainChain;

    mainChain.reserve(sortedLarger.size() + (pairs.size() > 0 ? 1 : 0));
    mainChain.insert(mainChain.begin(), sortedLarger.begin(), sortedLarger.end());

    if (!pairs.empty())
    {
        int firstSmall = pairs[0].first;
        std::vector<int>::iterator lb = std::lower_bound(mainChain.begin(), mainChain.end(), firstSmall);
        mainChain.insert(lb, firstSmall);
    }

    std::vector<int> pend;
    std::vector<int> pendPartners;

    for (size_t i = 1; i < pairs.size(); ++i)
    {
        pend.push_back(pairs[i].first);
        pendPartners.push_back(pairs[i].second);
    }

    std::vector<size_t> order = generateJacobsthalOrder(pend.size());

    for (size_t oi = 0; oi < order.size(); ++oi)
    {
        size_t pend_idx = order[oi];
        
        if (pend_idx >= pend.size())
            continue;

        int valueToInsert = pend[pend_idx];
        int partner = pendPartners[pend_idx];

        std::vector<int>::iterator bound_it = std::find(mainChain.begin(), mainChain.end(), partner);

        if (bound_it == mainChain.end())
            bound_it = mainChain.end();

        std::vector<int>::iterator pos_it = std::lower_bound(mainChain.begin(), bound_it, valueToInsert);
        mainChain.insert(pos_it, valueToInsert);
    }

    if (straggler != -1)
    {
        std::vector<int>::iterator pos_it = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
        mainChain.insert(pos_it, straggler);
    }

    return mainChain;
}

std::deque<int> fordJohnsonDeque(const std::vector<int> &input)
{
    size_t n = input.size();
    if (n <= 1)
    {
        std::deque<int> out;

        if (n == 1)
            out.push_back(input[0]);
        return out;
    }

    int straggler;
    std::vector< std::pair<int, int> > pairs = createPairsVector(input, straggler);

    if (pairs.empty())
    {
        std::deque<int> out;
        if (straggler != -1) out.push_back(straggler);
        return out;
    }

    std::vector<int> larger;
    larger.reserve(pairs.size());
    
    for (size_t i = 0; i < pairs.size(); ++i)
        larger.push_back(pairs[i].second);    

    std::vector<int> sortedLargerVec = fordJohnsonVector(larger);
    std::deque<int> mainChainDeque;

    mainChainDeque.insert(mainChainDeque.begin(), sortedLargerVec.begin(), sortedLargerVec.end());
    
    if (!pairs.empty())
    {
        int firstSmall = pairs[0].first;
        std::deque<int>::iterator lb = std::lower_bound(mainChainDeque.begin(), mainChainDeque.end(), firstSmall);
        mainChainDeque.insert(lb, firstSmall);
    }

    std::vector<int> pend;
    std::vector<int> pendPartners;

    for (size_t i = 1; i < pairs.size(); ++i)
    {
        pend.push_back(pairs[i].first);
        pendPartners.push_back(pairs[i].second);
    }

    std::vector<size_t> order = generateJacobsthalOrder(pend.size());

    for (size_t oi = 0; oi < order.size(); ++oi)
    {
        size_t pend_idx = order[oi];
        if (pend_idx >= pend.size())
            continue;

        int valueToInsert = pend[pend_idx];
        int partner = pendPartners[pend_idx];

        std::deque<int>::iterator bound_it = std::find(mainChainDeque.begin(), mainChainDeque.end(), partner);

        if (bound_it == mainChainDeque.end())
            bound_it = mainChainDeque.end();

        std::deque<int>::iterator pos_it = std::lower_bound(mainChainDeque.begin(), bound_it, valueToInsert);
        mainChainDeque.insert(pos_it, valueToInsert);
    }

    if (straggler != -1)
    {
        std::deque<int>::iterator pos_it = std::lower_bound(mainChainDeque.begin(), mainChainDeque.end(), straggler);
        mainChainDeque.insert(pos_it, straggler);
    }

    return mainChainDeque;
}

std::vector<int> deque_to_vector(const std::deque<int> &d)
{
    std::vector<int> v;
    v.reserve(d.size());

    for (std::deque<int>::const_iterator it = d.begin(); it != d.end(); ++it)
        v.push_back(*it);
    return v;
}

long get_time_us()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long)tv.tv_sec * 1000000L + (long)tv.tv_usec;
}

bool isSorted(const std::vector<int> &v)
{
    for (size_t i = 0; i + 1 < v.size(); ++i)
        if (v[i] > v[i+1])
            return false;
    return true;
}