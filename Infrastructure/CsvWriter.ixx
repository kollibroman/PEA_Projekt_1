module;

#include <fstream>
#include <string>
#include <iomanip>

export module CsvWriter;

import DefinitelyNotAVector;

// Klasa do zapisu danych w formacie CSV
export class CsvWriter
{
private:
    std::string filename;
    std::ofstream file;
    bool header_written;

public:
    CsvWriter(const std::string& filepath)
        : filename(filepath), header_written(false)
    {
        file.open(filename);
    }

    ~CsvWriter()
    {
        if (file.is_open())
            file.close();
    }

    bool IsOpen() const
    {
        return file.is_open();
    }

    // Zapis nagłówka CSV
    void WriteHeader(const DefinitelyNotAVector<std::string>& headers)
    {
        if (!file.is_open() || header_written)
            return;

        for (size_t i = 0; i < headers.size(); ++i)
        {
            file << headers[i];
            if (i < headers.size() - 1)
                file << ",";
        }
        file << "\n";
        header_written = true;
    }

    // Zapis wiersza z danymi (template dla różnych typów)
    template<typename... Args>
    void WriteRow(Args... args)
    {
        if (!file.is_open())
            return;

        WriteRowHelper(args...);
        file << "\n";
    }

    // Wymuszenie zapisu do pliku
    void Flush()
    {
        if (file.is_open())
            file.flush();
    }

private:
    // Pomocnicza funkcja do rekurencyjnego zapisu argumentów
    template<typename T>
    void WriteRowHelper(T value)
    {
        file << value;
    }

    template<typename T, typename... Rest>
    void WriteRowHelper(T first, Rest... rest)
    {
        file << first << ",";
        WriteRowHelper(rest...);
    }
};

// Dedykowany writer dla wyników czasowych
export class TimeResultsWriter
{
private:
    CsvWriter writer;

public:
    TimeResultsWriter(const std::string& filename = "times.csv")
        : writer(filename)
    {
        DefinitelyNotAVector<std::string> headers;
        headers.push_back("Algorithm");
        headers.push_back("N");
        headers.push_back("Time_ms");

        writer.WriteHeader(headers);
    }

    void WriteTimeResult(const std::string& algorithm_name, int n, double time_ms)
    {
        writer.WriteRow(algorithm_name, n, std::fixed, std::setprecision(3), time_ms);
    }

    void Flush()
    {
        writer.Flush();
    }

    bool IsOpen() const
    {
        return writer.IsOpen();
    }
};

// Dedykowany writer dla błędów względnych
export class ErrorResultsWriter
{
private:
    CsvWriter writer;

public:
    ErrorResultsWriter(const std::string& filename = "errors.csv")
        : writer(filename)
    {
        DefinitelyNotAVector<std::string> headers;
        headers.push_back("Algorithm");
        headers.push_back("N");
        headers.push_back("Error_pct");

        writer.WriteHeader(headers);
    }

    void WriteErrorResult(const std::string& algorithm_name, int n, double error_pct)
    {
        writer.WriteRow(algorithm_name, n, std::fixed, std::setprecision(2), error_pct);
    }

    void Flush()
    {
        writer.Flush();
    }

    bool IsOpen() const
    {
        return writer.IsOpen();
    }
};
