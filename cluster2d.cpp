#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <random>
#include <thread>
#include <ctime>

// A global string for indicating current version of program
std::string VersionNumber = "0.2";


// ####### 1. INITIALIZING FUNCTIONS #######

// Initialize starting parameters
// Rows: Number of rows of the 2D data table
// Columns: Number of columns of the 2D data table
// MaxElement: Maximum value of elements of the 2D data table
std::tuple<int, int, double, double> Initialization()
{
    int Rows;
    int Columns;
    double MaxElement;
    double dx;

    std::string Choosedx;

    while(1)
    {
        std::cout << ">> Please enter the number of Rows of the 2D data table!" << std::endl;
        std::cout << ">> Number of Rows: ";
        std::cin >> Rows;
        if(int(Rows) != Rows)
        {
            std::cout << "ERROR: Number of Rows should be an integer!\n" << std::endl; 
        }
        else
        {
            std::cout << "\n\n";
            break;
        }
    }

    while(1)
    {
        std::cout << ">> Please enter the number of Columns of the 2D data table!" << std::endl;
        std::cout << ">> Number of Columns: ";
        std::cin >> Columns;
        if(int(Columns) != Columns)
        {
            std::cout << "ERROR: Number of Columns should be an integer!\n" << std::endl; 
        }
        else
        {
            std::cout << "\n\n";
            break;
        }
    }

    while(1)
    {
        std::cout << ">> Please enter the Maximum Value of elements of the 2D data table! (Should be a positive real number)" << std::endl;
        std::cout << ">> Maximum Value: ";
        std::cin >> MaxElement;
        if(MaxElement <= 0)
        {
            std::cout << "ERROR: The maximum element should be a positive real number!!\n" << std::endl; 
        }
        else if(MaxElement > 0)
        {
            std::cout << "\n\n";
            break;
        }
        else
        {
            std::cout << "ERROR: Not permitted input!\n       The maximum element should be a positive real number! Please try again!" << std::endl;
        }
    }

    while(1)
    {
        std::cout << ">> Now please define the maximum distance between two neighbouring element!\n>> This shoud indicate, that what maximal difference between two elements means\n>> That they're belong to the same cluster. (Should be a positive real number)" << std::endl;
        std::cout << ">> Maximal difference (dx): ";
        std::cin >> dx;

        if(MaxElement <= 0)
        {
            std::cout << "ERROR: The maximum element should be a positive real number!!\n" << std::endl; 
        }

        else if(dx > MaxElement)
        {
            std::cout << "WARNING: \'dx\' is greater than the maximal value in your table. This means, that\n          Every element in the table will be classified into one cluster!\n          Would you like to continue or give an other value for dx?" << std::endl;
            std::cout << "WARNING: Write \'Y\' to continue, or \'N\' to give \'dx\' an other value: ";
            std::cin >> Choosedx;
            
            if(Choosedx.compare("Y") == 0 || Choosedx.compare("y") == 0 || Choosedx.compare("Yes") == 0 || Choosedx.compare("yes") == 0 || Choosedx.compare("YEs") == 0 || Choosedx.compare("yEs") == 0 || Choosedx.compare("yeS") == 0 || Choosedx.compare("YeS") == 0 || Choosedx.compare("yES") == 0)
            {
                std::cout << "\n\n";
                break;
            }
            else if(Choosedx.compare("N") == 0 || Choosedx.compare("n") == 0 || Choosedx.compare("No") == 0 || Choosedx.compare("no") == 0 || Choosedx.compare("nO") == 0)
            {
                std::cout << "\n\n" << std::endl;
            } // Do nothing; go to the start of the loop
        }

        else if(dx < MaxElement)
        {
            std::cout << "\n\n";
            break;
        }

        else
        {
            std::cout << "ERROR: Not permitted input!\n       The value of \'dx\' should be a positive real number! Please try again!" << std::endl;
        }
    }

    return(std::make_tuple(Rows, Columns, MaxElement, dx));

}


// Generate a 2D data table, as an elements of an std::vector
// This will look something like this (If MaxElement == 1):
//
//         1-1     1-2     1-3     1-4     1-5    ...
//
//  2-1   0.156   0.247   0.639   0.826   0.026   ...
//  2-2   0.852   0.765   0.285   0.095   0.706   ...
//  2-3   0.87    0.257   0.821   0.940   0.983   ...
//  2-4   0.278   0.198   0.189   0.209   0.960   ...
//  .      .      .      .      .      .
//  .      .      .      .      .      .
//
// Number indicate the following:
// #ofRows - #ofColumns
std::vector<std::pair<std::string,double>> Generate2dArray(int Rows, int Columns, double MaxElement)
{
    std::vector<std::pair<std::string,double>> ArrayTable;
    std::stringstream CurrentElement;

    //Randomize
    std::random_device Randomized;
    std::mt19937 Generate(Randomized());
    std::uniform_real_distribution<> Distribution(0, MaxElement);

    for(int i = 1; i <= Rows; i++)
    {
        for(int j = 1; j <= Columns; j++)
        {
            // Clear stringstream
            CurrentElement.str(std::string());

            // Make a string, which indicates the actual element with its position in the table
            // Like this: "#ofRows - #ofColumns"
            CurrentElement << i << "-" << j;
            std::string CurrentElementString = CurrentElement.str();

            //Generate a random double
            double RandomElement = Distribution(Generate);

            // Create a pair for the current element in the table:
            // 1. First element is the string above
            // 2. Second is the actual number in that particular location
            std::pair<std::string, double> CurrentElementPair;

            CurrentElementPair = std::make_pair(CurrentElementString, RandomElement);
            ArrayTable.push_back(CurrentElementPair);
        }
    }

    return(ArrayTable);
}


// Test
// Print out the generated 2D data table's elements
void PrintElements(std::vector<std::pair<std::string,double>> ArrayTable, int Rows, int Columns)
{
    for(int i = 0; i < Rows; i++)
    {
        for(int j = 0; j < Columns; j++)
        {
            std::cout << ArrayTable[i * Columns + j].second << " ";
        }
        std::cout << "\n";
    }
}


// ####### 2. CLUSTERING #######

// Auxiliary function
std::map<int, std::vector<std::string>> SimpleClusterStep(std::map<int, std::vector<std::string>> ClusteredGroups, std::vector<std::pair<std::string,double>> ArrayTable, int NumberOfClusterGroups, int Rows, int Columns, double dx)
{
    int i;
    int j;

    if((ArrayTable[(i - 1) * Columns + j].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + j].second > ArrayTable[i * Columns + j].second - dx))
    {
        
    }

    if((ArrayTable[i * Columns + (j - 1)].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[i * Columns + (j - 1)].second > ArrayTable[i * Columns + j].second - dx))
    {
        
    }

    if((ArrayTable[(i + 1) * Columns + j].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i + 1) * Columns + j].second > ArrayTable[i * Columns + j].second - dx))
    {
        
    }

    if((ArrayTable[i * Columns + (j + 1)].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[i * Columns + (j + 1)].second > ArrayTable[i * Columns + j].second - dx))
    {

    }

    return(ClusteredGroups);
}


// Brute force solution
std::map<int, std::vector<std::string>> ClusteringBruteForce(std::vector<std::pair<std::string,double>> ArrayTable, int Rows, int Columns, double dx)
{
    std::map<int, std::vector<std::string>> ClusteredGroups;
    std::map<int, std::vector<std::string>>::iterator MapIterator;

    std::vector<std::string>::iterator StringVectorIterator;

    int NumberOfClusterGroups = 0;

    // We on run through on the elements inside the outermost frame of our 2D table
    // This means we start from the 2-2 element, and  
    for(int i = 1; i < Rows - 1; i++)
    {
        for(int j = 1; j < Columns - 1; j++)
        {
            // Upper left corner
            if(i == 1 && j == 1)
            {
                SimpleClusterStep(ClusteredGroups, ArrayTable, NumberOfClusterGroups, Rows, Columns, dx);
                
                if((ArrayTable[(i - 1) * Columns + j].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + j].second > ArrayTable[i * Columns + j].second - dx))
                {
                    for(MapIterator = ClusteredGroups.begin(); MapIterator != ClusteredGroups.end(); ++MapIterator)
                    {
                        if(std::find(MapIterator->second.begin(), MapIterator->second.end(), ArrayTable[i * Columns + j].first) != MapIterator->second.end())
                        {
                            if(std::find(MapIterator->second.begin(), MapIterator->second.end(), ArrayTable[(i - 1) * Columns + j].first) != MapIterator->second.end())
                            {}

                            else
                            {
                                MapIterator->second.push_back(ArrayTable[(i - 1) * Columns + j].first);
                                break;
                            }
                        }

                        else
                        {
                            if(std::find(MapIterator->second.begin(), MapIterator->second.end(), ArrayTable[(i - 1) * Columns + j].first) != MapIterator->second.end())
                            {
                                MapIterator->second.push_back(ArrayTable[i * Columns + j].first);
                                break;
                            }
                        }
                    }
                }
            }

            // Upper right corner
            else if(i == 1 && j == Columns - 2)
            {
                SimpleClusterStep(ClusteredGroups, ArrayTable, NumberOfClusterGroups, Rows, Columns, dx);
                
                if((ArrayTable[(i - 1) * Columns + j].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + j].second > ArrayTable[i * Columns + j].second - dx))
                {
                    
                }
            }

            // Bottom left corner
            else if(i == Rows - 2 && j == 1)
            {
                SimpleClusterStep(ClusteredGroups, ArrayTable, NumberOfClusterGroups, Rows, Columns, dx);
                
                if((ArrayTable[(i - 1) * Columns + j].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + j].second > ArrayTable[i * Columns + j].second - dx))
                {
                    
                }
            }

            // Botton right corner
            else if(i == Rows - 2 && j == Columns - 2)
            {
                SimpleClusterStep(ClusteredGroups, ArrayTable, NumberOfClusterGroups, Rows, Columns, dx);
                
                if((ArrayTable[(i - 1) * Columns + j].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + j].second > ArrayTable[i * Columns + j].second - dx))
                {
                    
                }
            }

            // Other tiles
            else
            {
                SimpleClusterStep(ClusteredGroups, ArrayTable, NumberOfClusterGroups, Rows, Columns, dx);
            }
        }
    }

    return ClusteredGroups;
}

int main()
{
    std::cout << ">>>    CLUSTERING WITH C++ PARALLEL PROGRAMMING    <<<" << std::endl;
	std::cout << ">>>                  VERSION " << VersionNumber << "                   <<<\n\n" << std::endl;

    auto InitialParameters = Initialization();

    int Rows = std::get<0>(InitialParameters);
    int Columns = std::get<1>(InitialParameters);
    double MaxElement = std::get<2>(InitialParameters);
    double dx = std::get<3>(InitialParameters);

    // Generate a 2D table with random floats, valued between 0 and a choosen number
    std::vector<std::pair<std::string,double>> ArrayTable = Generate2dArray(Rows, Columns, MaxElement);
    std::cout << "INFORMATIVE MESSAGE: The data table has been created!\n" << std::endl;

    // Run test
    //PrintElements(ArrayTable, Rows, Columns);

}