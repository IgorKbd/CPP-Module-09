/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikayiban <ikayiban@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 11:41:32 by ikayiban          #+#    #+#             */
/*   Updated: 2026/01/19 12:48:44 by ikayiban         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() 
{}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &copy) 
{
    *this = copy;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &obj)
{
    if (this != &obj)
        this->dataBase = obj.dataBase;
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{}

float BitcoinExchange::getValue(const std::string &date)
{
    std::map<std::string, float>::const_iterator it = dataBase.lower_bound(date);

    if (it != dataBase.end() && it->first == date)
        return it->second;

    if (it == dataBase.begin())
        throw std::runtime_error("Error: no earlier date available in database.");

    --it;
    return it->second;
}

std::map<std::string, float> &BitcoinExchange::getDataBase() 
{
    return dataBase;
}

void BitcoinExchange::setDataBase(const std::map<std::string, float>& Db)
{
    dataBase = Db;
}

bool isDate(const std::string &date)
{
    if (date.size() != 10)
        return false;

    for (size_t i = 0; i < date.size(); ++i)
    {
        if (i == 4 || i == 7)
        {
            if (date[i] != '-')
                return false;
        }
        else if (!isdigit(date[i]))
            return false;
    }

    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());

    if (month < 1 || month > 12)
        return false;

    static const int daysInMonth[12] =
    {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    int maxDay = daysInMonth[month - 1];

    if (month == 2 &&
        ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
    {
        maxDay = 29;
    }

    return (day >= 1 && day <= maxDay);
}

bool isValid(const std::string &s)
{
    bool hasDot = false;

    if (s.empty())
        return false;

    for (size_t i = 0; i < s.size(); ++i)
    {
        if (s[i] == '.')
        {
            if (hasDot || i == 0)
                return false;
            hasDot = true;
        }
        else if (s[i] == '-')
        {
            if (i != 0)
                return false;
        }
        else if (!isdigit(s[i]))
            return false;
    }
    return true;
}

bool isValue(const std::string &toCheck, const std::string &line)
{
    if (!isValid(toCheck))
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return false;
    }

    float value;
    std::istringstream iss(toCheck);
    if (!(iss >> value))
        return false;

    if (value < 0)
    {
        std::cerr << "Error: not a positive number." << std::endl;
        return false;
    }
    
    if (value > 1000)
    {
        std::cerr << "Error: too large a number." << std::endl;
        return false;
    }
    return true;
}

bool isDbValue(const std::string &s)
{
    bool hasDot = false;

    if (s.empty())
        return false;

    for (size_t i = 0; i < s.size(); ++i)
    {
        if (s[i] == '.')
        {
            if (hasDot)
                return false;
            hasDot = true;
        }
        else if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

bool parsing(std::ifstream &file, std::map<std::string, float> &dataBase)
{
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        std::string date, val;

        std::getline(iss, date, ',');
        std::getline(iss, val);

        if (!isDbValue(val))
            continue;
        
        if (!isDate(date))
            continue;

        float value;
        std::istringstream(val) >> value;
        dataBase[date] = value;
    }
    return true;
}

bool openAndParseDB(BitcoinExchange &btc)
{
    std::map<std::string, float> dataBase;
    std::ifstream file("data.csv");

    if (!file.is_open())
    {
        std::cerr << "Error: could not open data.csv" << std::endl;
        return false;
    }

    if (!parsing(file, dataBase))
        return false;
    btc.setDataBase(dataBase);
    return true;
}

static std::string trim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    if (start == std::string::npos)
        return "";
    return s.substr(start, end - start + 1);
}

void readProcess(std::ifstream &input, BitcoinExchange &btc)
{
    std::string line;
    bool first = true;
    
    while (std::getline(input, line))
    {
        if (first)
        {
            first = false;
            if (trim(line) == "date | value")
                continue;
        }

        if (trim(line).empty())
            continue;
        
        std::istringstream iss(line);
        std::string date, amount;

        std::getline(iss, date, '|');
        std::getline(iss, amount, '|');

        date = trim(date);
        amount = trim(amount);

        if (!isDate(date))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (!isValue(amount, line))
            continue;

        float btcAmount;
        std::istringstream(amount) >> btcAmount;

        try
        {
            float value = btc.getValue(date);
            std::cout << date << " => " << btcAmount << " = " << value * btcAmount << std::endl;
        }
        catch(const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

bool mainProcess(BitcoinExchange &btc, char *file)
{
    std::ifstream input(file);
    if (!input.is_open())
    {
        std::cerr << "Error: could not open file" << std::endl;
        return false;
    }
    readProcess(input, btc);
    return true;
}
