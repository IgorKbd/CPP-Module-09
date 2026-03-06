/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikayiban <ikayiban@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 11:04:31 by ikayiban          #+#    #+#             */
/*   Updated: 2026/01/19 12:45:51 by ikayiban         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>

class BitcoinExchange
{
    private:
        std::map<std::string, float> dataBase;

    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &copy);
        BitcoinExchange &operator=(const BitcoinExchange &obj);
        ~BitcoinExchange();

        float getValue(const std::string &date);

        std::map<std::string, float> &getDataBase();
        void setDataBase(const std::map<std::string, float>& Db);
};

bool isDate(const std::string &toCheck);
bool isValue(const std::string &toCheck, const std::string &line);

bool openAndParseDB(BitcoinExchange &btc);
void readProcess(std::ifstream &input, BitcoinExchange &btc);
bool mainProcess(BitcoinExchange &btc, char *file);

#endif

