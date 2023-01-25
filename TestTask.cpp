#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cctype>
#include <string>
using namespace std;

vector<string> getExpression(string value)
{
    string firstArgument = "";
    string secondArgument = "";
    string sign = "";
    vector<string> expression;
    int position;
    try
    {
        if (value.find('+') != -1)
        {
            position = value.find('+');
            sign = "+";
        }
        if (value.find('-') != -1) {
            position = value.find('-');
            sign = "-";
        }
        if (value.find('*') != -1) {
            position = value.find('*');
            sign = "*";
        }
        if (value.find('/') != -1) {
            position = value.find('/');
            sign = "/";
        }
    }
    catch (invalid_argument e)
    {
        throw "Invalid sign";
    }

    for (int i = 0; i < position; ++i)
    {
        firstArgument += value[i];
    }
    for (int i = position + 1; i < value.size(); ++i)
    {
        secondArgument += value[i];
    }

    expression.push_back(firstArgument);
    expression.push_back(sign);
    expression.push_back(secondArgument);

    return expression;
}

vector<string> getCoordinates(string value)
{
    string word = "";
    string number = "";
    for (int i = 0; i < value.length(); ++i)
    {
        if (isalpha(value[i]))
        {
            word += value[i];
        }
        if (isdigit(value[i]))
        {
            number += value[i];
        }

    }
    vector<string> expression;
    expression.push_back(word);
    expression.push_back(number);
    return expression;
}

int getIndex(string coordinate, vector <string>& array)
{
    for (int i = 0; i < array.size(); ++i)
    {
        if (coordinate == array[i])
        {
            return i;
        }
    }
    return -1;
}

int getNumber(string expression, vector <string>& header, vector <string>& numbers, vector <vector <string>>& cells)
{
    vector<string> coordinates = getCoordinates(expression);
    int result = 0;
    int column = getIndex(coordinates[0], header);
    int row = getIndex(coordinates[1], numbers);
    try
    {
        if (row == -1 || column == -1)
        {
            throw "Invalid expression";
        }
        result = stoi(cells[row][column]);
    }
    catch (invalid_argument e)
    {
        throw "Invalid argument";
    }
    return result;
}

string getCellExpression(string value, vector <string>& header, vector <string>& numbers, vector <vector <string>>& cells)
{
    vector<string> expression = getExpression(value);
    int firstArgument = 0, secondArgument = 0;

    if (isalpha(expression[0][0]))
    {
        try
        {
            firstArgument = getNumber(expression[0], header, numbers, cells);
        }
        catch (char const* str)
        {
            return (str);
        }
    }
    else
    {
        try
        {
            firstArgument = stoi(expression[0]);
        }
        catch (invalid_argument e)
        {
            return ("Invalid Argument");
        }
    }

    if (isalpha(expression[2][0]))
    {
        try
        {
            secondArgument = getNumber(expression[2], header, numbers, cells);
        }
        catch (char const* str)
        {
            return (str);
        }
    }
    else
    {
        try
        {
            secondArgument = stoi(expression[2]);
        }
        catch (invalid_argument e)
        {
            return ("Invalid Argument");
        }
    }

    int result = 0;
    if (expression[1] == "+")
    {
        result = firstArgument + secondArgument;
    }
    else if (expression[1] == "-")
    {
        result = firstArgument - secondArgument;
    }
    else if (expression[1] == "/")
    {
        if (secondArgument == 0) return ("Dividing by zero");
        else result = int(firstArgument / secondArgument);
    }
    else if (expression[1] == "*")
    {
        result = firstArgument * secondArgument;
    }
    return to_string(result);
}

void calculateCell(vector <string>& header, vector <string>& numbers, vector <vector <string>>& cells)
{
    vector <string> expression;
    string  value = "";
    for (int i = 0; i < cells.size(); ++i)
    {
        for (int j = 0; j < cells[i].size(); ++j)
        {
            value = cells[i][j];
            if (value[0] == '=')
            {
                value.erase(0, 1);
                cells[i][j] = getCellExpression(value, header, numbers, cells);
            }
        }
    }
}


int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        cout << "Please specify csv file";
        return 0;
    }
    ifstream file(argv[1]);
    string line;
    string value;
    char delimiter = ',';
    vector <string> header;
    vector <string> numbers;
    vector < vector <string> > cells;
    int i = 0;
    while (getline(file, line))
    {
        int j = 0;
        vector <string> row;
        istringstream s(line);
        string value;
        while (getline(s, value, delimiter))
        {
            if (j == 0)
            {
                ++j;
                if (value == "" && i == 0)
                {
                    if (i == 0) {}
                    else
                    {
                        cout << "Blank cell [" << i << "][" << j << "]" << endl;
                        return 0;
                    }
                }
                else
                {
                    numbers.push_back(value);
                }
            }
            else
            {
                if (value == "")
                {
                    cout << "Blank cell [" << i << "][" << j << "]" << endl;
                    return 0;
                }
                row.push_back(value);
            }
        }
        ++i;
        cells.push_back(row);
    }
    header = cells[0];
    auto iter = cells.cbegin();
    cells.erase(iter);

    calculateCell(header, numbers, cells);

    cout << ",";
    for (size_t j = 0; j < header.size(); j++)
    {
        if (j == 0) cout << header[j];
        else cout << "," << header[j];
    }
    cout << endl;
    for (size_t i = 0; i < cells.size(); i++)
    {
        cout << numbers[i] << ",";
        for (size_t j = 0; j < cells[i].size(); j++)
        {
            cout << cells[i][j];
            if (j != cells[i].size() - 1) cout << ",";
        }
        cout << endl;
    }
    file.close();
    return 0;
}