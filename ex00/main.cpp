/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikayiban <ikayiban@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 09:36:37 by ikayiban          #+#    #+#             */
/*   Updated: 2026/01/19 12:49:46 by ikayiban         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "2 args required !" << std::endl;
        return 1;
    }

    BitcoinExchange btc;

    if (!openAndParseDB(btc))
        return 1;

    if (!mainProcess(btc, av[1]))
        return 1;

    return 0;
}

