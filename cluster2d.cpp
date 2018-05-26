#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <thread>

// Constants
std::string VersionNumber = "0.2";

// Initialize starting parameters
// Rows: Number of rows of the 2D data table
// Columns: Number of columns of the 2D data table
// MaxElement: Maximum value of elements of the 2D data table
std::tuple<int, int, double> Initialization()
{
    int Rows;
    int Columns;
    double MaxElement;

    std::cout << ">> Please enter the number of Rows of the 2D data table!" << std::endl;
    std::cout << ">> Number of Rows: ";
    std::cin >> Rows;
    std::cout << "\n\n";

    std::cout << ">> Please enter the number of Columns of the 2D data table!" << std::endl;
    std::cout << ">> Number of Columns: ";
    std::cin >> Columns;
    std::cout << "\n\n";

    std::cout << ">> Please enter the Maximum Value of elements of the 2D data table!" << std::endl;
    std::cout << ">> Maximum Value: ";
    std::cin >> MaxElement;
    std::cout << "\n\n";

    return(std::make_tuple(Rows, Columns, MaxElement));

}


std::vector<double> Generate2dArray(int Rows, int Columns, double MaxElement)
{
    std::vector<double> ArrayTable;

    //Randomize
    std::random_device Randomized;
    std::mt19937 Generate(Randomized());
    std::uniform_real_distribution<> Distribution(0, MaxElement);

    for(int i = 0; i < Rows * Columns; i++)
    {
        double RandomElement = Distribution(Generate);
		ArrayTable.push_back(RandomElement);
    }

    return(ArrayTable);
}

// Test
void PrintElements(std::vector<double> ArrayTable, int Rows, int Columns)
{
    for(int i = 0; i < Rows; i++)
    {
        for(int j = 0; j < Columns; j++)
        {
            std::cout << ArrayTable[i * Columns + j] << " ";
        }
        std::cout << "\n";
    }
}


int main()
{
    std::cout << ">>>    CLUSTERING WITH C++ PARALLEL PROGRAMMING    <<<" << std::endl;
	std::cout << ">>>                  VERSION " << VersionNumber << "                   <<<\n\n" << std::endl;

    auto InitialParameters = Initialization();

    int Rows = std::get<0>(InitialParameters);
    int Columns = std::get<1>(InitialParameters);
    double MaxElement = std::get<2>(InitialParameters);

    // Generate a 2D table with random floats, valued between 0 and a choosen number
    std::vector<double> ArrayTable = Generate2dArray(Rows, Columns, MaxElement);

    // Test
    //PrintElements(ArrayTable, Rows, Columns);

}