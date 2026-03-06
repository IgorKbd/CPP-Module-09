/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikayiban <ikayiban@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:58:10 by ikayiban          #+#    #+#             */
/*   Updated: 2026/01/19 10:28:14 by ikayiban         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include <sys/time.h>
#include <climits>
#include <string>
#include <sstream>

long get_time_us();
void print_before(char **argv);
bool isSorted(const std::vector<int> &v);
std::vector<int> deque_to_vector(const std::deque<int> &d);
bool validate_and_build_input(char **argv, std::vector<int> &out);
std::deque<int> fordJohnsonDeque(const std::vector<int> &input);
std::vector<int> fordJohnsonVector(const std::vector<int> &input);

#endif