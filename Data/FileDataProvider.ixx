module;
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

export module FileDataProvider;

import IDataProvider;
import TestData;
import DefinitelyNotAVector;

export class FileDataProvider : public IDataProvider
{
private:
    std::string filePath;

public:
    FileDataProvider(std::string filePath)
    {
        this->filePath = std::move(filePath);
    }


    TestData GetRequiredData() const override
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            std::cerr << "Could not open file: " << filePath << std::endl;
            return { 0, DefinitelyNotAVector<DefinitelyNotAVector<int>>() };
        }

        int size = 0;
        if (!(file >> size))
        {
            std::cerr << "Error reading size from file: " << filePath << std::endl;
            return { 0, DefinitelyNotAVector<DefinitelyNotAVector<int>>() };
        }

        DefinitelyNotAVector<DefinitelyNotAVector<int>> data(size);
        for (int i = 0; i < size; ++i)
        {
            data[i] = DefinitelyNotAVector<int>(size);
            for (int j = 0; j < size; ++j)
            {
                int val;
                if (file >> val)
                {
                    data[i][j] = val;
                }
                else
                {
                    std::cerr << "Error reading matrix element at [" << i << "][" << j << "]" << std::endl;
                }
            }
        }

        return { size, data };
    }
};