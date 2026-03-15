cmake_minimum_required(VERSION 4.1)
project(Projekt_1)

set(CMAKE_CXX_STANDARD 26)

add_executable(Projekt_1 main.cpp)

target_sources(Projekt_1 PUBLIC
        FILE_SET CXX_MODULES FILES
        Abstractions/IAtspAlgorithm.ixx
        Algorithms/NN.ixx
        Algorithms/RNN.ixx
        Algorithms/BF.ixx
        Algorithms/BFHeap.ixx
        Algorithms/RandomAlgorithm.ixx
        Infrastructure/AlgorithmTester.ixx
        Abstractions/IDataProvider.ixx
        Data/RandomDataGenerator.ixx
        Data/FileDataProvider.ixx
        DefinitelyNotADataStructures/DefinitelyNotAVector.ixx
        Models/TestData.ixx
        Models/AlgorithmResult.ixx
        Infrastructure/Parser.ixx
        Infrastructure/CsvWriter.ixx
)

