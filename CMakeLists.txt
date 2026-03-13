cmake_minimum_required(VERSION 4.1)
project(Projekt_1)

set(CMAKE_CXX_STANDARD 26)

add_executable(Projekt_1 main.cpp
        Data/RandomDataGenerator.ixx
        Data/FileDataProvider.ixx
)

target_sources(Projekt_1 PUBLIC
        FILE_SET CXX_MODULES FILES
        Abstractions/IAtspAlgorithm.ixx
        Algorithms/NN.ixx
        Algorithms/RNN.ixx
        Algorithms/BF.ixx
        Infrastructure/CsvParser.ixx
        Infrastructure/AlgorithmTester.ixx
        Abstractions/IDataProvider.ixx
)

