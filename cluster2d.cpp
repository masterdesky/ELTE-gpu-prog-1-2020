#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <random>
#include <thread>
#include <ctime>

// ####### 0. GLOBAL PARAMETERS #######

// A global string for indicating current version of program
std::string VersionNumber = "0.3";

// Counter for the number of cluster group = 0;
int NumberOfClusterGroups = 0;


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
            // A string will look like this: "#ofRows - #ofColumns"
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
// ###    2/1. BRUTEFORCE    ###

// --- Auxiliary function ---
// - Does the following:
// 1. Activates if both of the neighbouring tiles are inside the same radii criterium
// 2. The function then classify them to the proper cluster (group)
//
// - Legends:
// ArrayTableElement1: The first element, to which we compare the other element (CALLED FIRST ELEMENT)
// ArrayTableElement2: The second element, which is compared to the first one (CALLED SECOND ELEMENT)
std::map<int, std::vector<std::string>> ClusterStep(std::map<int, std::vector<std::string>> ClusteredGroups, std::pair<std::string,double> ArrayTableElement1, std::pair<std::string,double> ArrayTableElement2, double dx)
{
    // Iterator for the map structure
    std::map<int, std::vector<std::string>>::iterator MapIterator;
    std::map<int, std::vector<std::string>>::iterator MapIteratorTemp;

    // Iterator for the vector structure
    std::vector<std::string>::iterator StringVectorIterator;

    // Temporary storage for a specific index while iterating through the entries of a map
    int TempStrorageForIndeces = -1;

    // Run through the vectors respect to them keys in the std::map structure
    for(MapIterator = ClusteredGroups.begin(); MapIterator != ClusteredGroups.end(); ++MapIterator)
    {
        // Activates if the first element is already inside one of the vectors in the std::map structure
        // Particulary in the MapIterator# group
        if(std::find(MapIterator->second.begin(), MapIterator->second.end(), ArrayTableElement1.first) != MapIterator->second.end())
        {
            for(MapIteratorTemp = ClusteredGroups.begin(); MapIteratorTemp != ClusteredGroups.end(); ++MapIteratorTemp)
            {
                // Check if the second element is inside any of the map's pairs
                if(std::find(MapIteratorTemp->second.begin(), MapIteratorTemp->second.end(), ArrayTableElement2.first) != MapIteratorTemp->second.end())
                {
                    // If the second element is already inside the SAME group, then everything is all right, do nothing
                    if(MapIteratorTemp->first == MapIterator->first)
                    {
                        break;
                    }

                    // If they aren't, then merge the two group, and refresh the 'ClusteredGroups' map's keys
                    else
                    {
                        if(MapIteratorTemp->first > MapIterator->first)
                        {
                            MapIterator->second.insert(MapIterator->second.end(), MapIteratorTemp->second.begin(), MapIteratorTemp->second.end());
                            TempStrorageForIndeces = MapIteratorTemp->first;
                            ClusteredGroups.erase(MapIteratorTemp->first);
                            break;
                        }

                        else if(MapIteratorTemp->first < MapIterator->first)
                        {
                            MapIteratorTemp->second.insert(MapIteratorTemp->second.end(), MapIterator->second.begin(), MapIterator->second.end());
                            TempStrorageForIndeces = MapIterator->first;
                            ClusteredGroups.erase(MapIterator->first);
                            break;
                        }
                    }
                }

                // If the second element isn't in that group, then put it into there
                else
                {
                    MapIterator->second.push_back(ArrayTableElement2.first);
                    break;
                }
            }

            if(TempStrorageForIndeces != -1)
            {
                break;
            }
        }

        // What if the first element isn't inside a group?
        else
        {
            // Check if the second element is already contained by a group
            // If it is, then insert the first element into that too
            if(std::find(MapIterator->second.begin(), MapIterator->second.end(), ArrayTableElement2.first) != MapIterator->second.end())
            {
                MapIterator->second.push_back(ArrayTableElement1.first);
                break;
            }

            // If none of them is inside a group, then create a new group
            else
            {
                // Increment the counter for the number of the cluster groups
                int *TempForNumberOfCG;
                TempForNumberOfCG = &NumberOfClusterGroups;
                ++*TempForNumberOfCG;

                std::vector<std::string> TempStorage = {ArrayTableElement1.first, ArrayTableElement2.first};

                ClusteredGroups.insert(std::pair<int, std::vector<std::string>>(NumberOfClusterGroups, TempStorage));

                TempStorage.clear();
            }
        }
    }

    if(TempStrorageForIndeces != -1)
    {
        for(MapIterator = ClusteredGroups.begin(); MapIterator != ClusteredGroups.end(); ++MapIterator)
        {
            int TempIndexFind = TempStrorageForIndeces + 1;
            if(MapIterator->first == TempIndexFind)
            {
                auto NodeHandler = ClusteredGroups.extract(TempIndexFind);
                NodeHandler.key() = TempStrorageForIndeces;
                ClusteredGroups.insert(std::move(NodeHandler));
                ++TempStrorageForIndeces;
            }
        }
        TempStrorageForIndeces = -1;
    }

    return(ClusteredGroups);
}

// Frame for the basic check in an individual step of the clustering
// For every tile, we need to do (at least) the following two step
// The X-s indicates, which elements are compared around the middle one
std::map<int, std::vector<std::string>> BasicClusterFrame(std::map<int, std::vector<std::string>> ClusteredGroups, std::vector<std::pair<std::string,double>> ArrayTable, int Rows, int Columns, int i, int j, double dx)
{
    std::pair<std::string,double> ArrayTableElement1;
    std::pair<std::string,double> ArrayTableElement2;

    //  O.O.O
    //  O.X.O
    //  O.X.O
    if((ArrayTable[(i + 1) * Columns + j].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i + 1) * Columns + j].second > ArrayTable[i * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[(i + 1) * Columns + j];
        ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j - 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
    }

    //  O.O.O
    //  O.X.X
    //  O.O.O
    if((ArrayTable[i * Columns + (j + 1)].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[i * Columns + (j + 1)].second > ArrayTable[i * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[(i + 1) * Columns + j];
        ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j - 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
    }

    return(ClusteredGroups);

}


// Function for the uppermost bar of tiles
// O.X.X.X.X.X.O
// O.O.O.O.O.O.O
// O.O.O.O.O.O.O
// O.O.O.O.O.O.O
std::map<int, std::vector<std::string>> UppermostBarClusterFrame(std::map<int, std::vector<std::string>> ClusteredGroups, std::vector<std::pair<std::string,double>> ArrayTable, int Rows, int Columns, int i, int j, double dx)
{
    std::pair<std::string,double> ArrayTableElement1;
    std::pair<std::string,double> ArrayTableElement2;

    // Activates if the element in the cell (i-1)j# (here 1-2) is in the radii criterium compared to the ij# (here 2-2) element
    //  O.X.O
    //  O.X.O
    //  O.O.O
    if((ArrayTable[(i - 1) * Columns + j].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + j].second > ArrayTable[i * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + j];
        ArrayTableElement2 = ArrayTable[(i - 1) * Columns + j];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
    }

    // Activates if the element in the cell (i-1)(j+1)# (here 1-3) is in the radii criterium compared to the (i-1)j# (here 1-2) element
    //  O.X.X
    //  O.O.O
    //  O.O.O
    if((ArrayTable[(i - 1) * Columns + (j + 1)].second < ArrayTable[(i - 1) * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + (j + 1)].second > ArrayTable[(i - 1) * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[(i - 1) * Columns + j];
        ArrayTableElement2 = ArrayTable[(i - 1) * Columns + (j + 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
    }

    return(ClusteredGroups);
}

// Function for the leftmost bar of tiles
// O.O.O.O
// X.O.O.O
// X.O.O.O
// X.O.O.O
// X.O.O.O
// O.O.O.O
std::map<int, std::vector<std::string>> LeftmostBarClusterFrame(std::map<int, std::vector<std::string>> ClusteredGroups, std::vector<std::pair<std::string,double>> ArrayTable, int Rows, int Columns, int i, int j, double dx)
{
    std::pair<std::string,double> ArrayTableElement1;
    std::pair<std::string,double> ArrayTableElement2;

    // Activates if the element in the cell i(j-1)# (here 2-1) is in the radii criterium compared to the ij# (here 2-2) element
    //  O.O.O
    //  X.X.O
    //  O.O.O
    if((ArrayTable[i * Columns + (j - 1)].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[i * Columns + (j - 1)].second > ArrayTable[i * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + (j - 1)];
        ArrayTableElement2 = ArrayTable[i * Columns + j];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
    }

    // Activates if the element in the cell (i+1)(j-1)# (here 3-1) is in the radii criterium compared to the i(j-1)# (here 2-1) element
    //  O.O.O
    //  X.O.O
    //  X.O.O
    if((ArrayTable[(i + 1) * Columns + (j - 1)].second < ArrayTable[i * Columns + (j - 1)].second + dx) && (ArrayTable[(i + 1) * Columns + (j - 1)].second > ArrayTable[i * Columns + (j - 1)].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + (j - 1)];
        ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j - 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
    }

    return(ClusteredGroups);
}

// Function for the rightmost bar of tiles
// O.O.O.O
// O.O.O.X
// O.O.O.X
// O.O.O.X
// O.O.O.X
// O.O.O.O
std::map<int, std::vector<std::string>> RightmostBarClusterFrame(std::map<int, std::vector<std::string>> ClusteredGroups, std::vector<std::pair<std::string,double>> ArrayTable, int Rows, int Columns, int i, int j, double dx)
{
    std::pair<std::string,double> ArrayTableElement1;
    std::pair<std::string,double> ArrayTableElement2;

    // Activates if the element in the cell (i+1)(j+1)# is in the radii criterium compared to the i(j+1)# (here 3-(Cols)) element
    //  O.O.O
    //  O.O.X
    //  O.O.X
    if((ArrayTable[(i + 1) * Columns + (j + 1)].second < ArrayTable[i * Columns + (j + 1)].second + dx) && (ArrayTable[(i + 1) * Columns + (j + 1)].second > ArrayTable[i * Columns + (j + 1)].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + (j + 1)];
        ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j + 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
    }

    return(ClusteredGroups);
}

// Function for the bottommost bar of tiles
// O.O.O.O.O.O.O
// O.O.O.O.O.O.O
// O.O.O.O.O.O.O
// O.X.X.X.X.X.O
std::map<int, std::vector<std::string>> BottommostBarClusterFrame(std::map<int, std::vector<std::string>> ClusteredGroups, std::vector<std::pair<std::string,double>> ArrayTable, int Rows, int Columns, int i, int j, double dx)
{
    std::pair<std::string,double> ArrayTableElement1;
    std::pair<std::string,double> ArrayTableElement2;

    // Activates if the element in the cell (i+1)(j+1)# is in the radii criterium compared to the (i+1)j# element
    //  O.O.O
    //  O.O.O
    //  O.X.X
    if((ArrayTable[(i + 1) * Columns + (j + 1)].second < ArrayTable[(i + 1) * Columns + j].second + dx) && (ArrayTable[(i + 1) * Columns + (j + 1)].second > ArrayTable[(i + 1) * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[(i + 1) * Columns + j];
        ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j + 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
    }

    return(ClusteredGroups);
}

// Brute force solution frame
std::map<int, std::vector<std::string>> ClusteringBruteForce(std::vector<std::pair<std::string,double>> ArrayTable, int Rows, int Columns, double dx)
{
    // Container for the actual clusters
    std::map<int, std::vector<std::string>> ClusteredGroups;

    // Tomporary containers for the std::pairs of the ArrayTable 
    std::pair<std::string,double> ArrayTableElement1;
    std::pair<std::string,double> ArrayTableElement2;

    // We on run through on the tiles, which are inside the outermost frame of our 2D table
    // This means we start from the 2-2 element
    // The X-s indicates, which elements are compared around the middle one
    for(int i = 1; i < Rows - 1; i++)
    {
        for(int j = 1; j < Columns - 1; j++)
        {
            // Basic step in the clustering, needed for every step
            ClusteredGroups = BasicClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);

            // UPPER LEFT corner
            if(i == 1 && j == 1)
            {
                ClusteredGroups = UppermostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
                ClusteredGroups = LeftmostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);

                // Activates if the element in the UPPER LEFT corner (here 1-1) is in the radii criterium compared to the (i-1)j# (here 1-2) element
                //  X.X.O
                //  O.O.O
                //  O.O.O
                if((ArrayTable[(i - 1) * Columns + (j - 1)].second < ArrayTable[(i - 1) * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + (j - 1)].second > ArrayTable[(i - 1) * Columns + j].second - dx))
                {
                    ArrayTableElement1 = ArrayTable[(i - 1) * Columns + j];
                    ArrayTableElement2 = ArrayTable[(i - 1) * Columns + (j - 1)];
                    ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
                }

                // Activates if the element in the UPPER LEFT corner (here 1-1) is in the radii criterium compared to the i(j-1)# (here 2-1) element
                //  X.O.O
                //  X.O.O
                //  O.O.O
                if((ArrayTable[(i - 1) * Columns + (j - 1)].second < ArrayTable[i * Columns + (j - 1)].second + dx) && (ArrayTable[(i - 1) * Columns + (j - 1)].second > ArrayTable[i * Columns + (j - 1)].second - dx))
                {
                    ArrayTableElement1 = ArrayTable[i * Columns + (j - 1)];
                    ArrayTableElement2 = ArrayTable[(i - 1) * Columns + (j - 1)];
                    ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
                }
            }

            // UPPER RIGHT corner
            else if(i == 1 && j == Columns - 2)
            {
                ClusteredGroups = RightmostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
                
                // Activates if the element in the UPPER RIGHT corner (here 1-(Cols)) is in the radii criterium compared to the (i-1)j# (here 1-(Cols - 1)) element
                //  O.X.X
                //  O.O.O
                //  O.O.O
                if((ArrayTable[(i - 1) * Columns + (j + 1)].second < ArrayTable[(i - 1) * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + (j + 1)].second > ArrayTable[(i - 1) * Columns + j].second - dx))
                {
                    ArrayTableElement1 = ArrayTable[(i - 1) * Columns + j];
                    ArrayTableElement2 = ArrayTable[(i - 1) * Columns + (j + 1)];
                    ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
                }

                // Activates if the element in the UPPER RIGHT corner (here 1-(Cols)) is in the radii criterium compared to the i(j+1)# (here 2-(Cols)) element
                //  O.O.X
                //  O.O.X
                //  O.O.O
                if((ArrayTable[(i - 1) * Columns + (j + 1)].second < ArrayTable[i * Columns + (j + 1)].second + dx) && (ArrayTable[(i - 1) * Columns + (j + 1)].second > ArrayTable[i * Columns + (j + 1)].second - dx))
                {
                    ArrayTableElement1 = ArrayTable[i * Columns + (j + 1)];
                    ArrayTableElement2 = ArrayTable[(i - 1) * Columns + (j + 1)];
                    ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
                }

                // Activates if the element in the cell (i-1)j# (here 1-(Columns - 1)) is in the radii criterium compared to the ij# (here 2-(Cols - 1)) element
                //  O.X.O
                //  O.X.O
                //  O.O.O
                if((ArrayTable[(i - 1) * Columns + j].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + j].second > ArrayTable[i * Columns + j].second - dx))
                {
                    ArrayTableElement1 = ArrayTable[i * Columns + j];
                    ArrayTableElement2 = ArrayTable[(i - 1) * Columns + j];
                    ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
                }
            }

            // BOTTOM LEFT corner
            else if(i == Rows - 2 && j == 1)
            {
                ClusteredGroups = BottommostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
                
                // Activates if the element in the BOTTOM LEFT corner (here (Rows)-1) is in the radii criterium compared to the (i+1)j# (here (Rows)-2) element
                //  O.O.O
                //  O.O.O
                //  X.X.O
                if((ArrayTable[(i + 1) * Columns + (j - 1)].second < ArrayTable[(i + 1) * Columns + j].second + dx) && (ArrayTable[(i + 1) * Columns + (j - 1)].second > ArrayTable[(i + 1) * Columns + j].second - dx))
                {
                    ArrayTableElement1 = ArrayTable[(i + 1) * Columns + j];
                    ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j - 1)];
                    ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
                }

                // Activates if the element in the BOTTOM LEFT corner (here (Rows)-1) is in the radii criterium compared to the i(j-1)# (here (Rows - 1)-1) element
                //  O.O.O
                //  X.O.O
                //  X.O.O
                if((ArrayTable[(i + 1) * Columns + (j - 1)].second < ArrayTable[i * Columns + (j - 1)].second + dx) && (ArrayTable[(i + 1) * Columns + (j - 1)].second > ArrayTable[i * Columns + (j - 1)].second - dx))
                {
                    ArrayTableElement1 = ArrayTable[i * Columns + (j - 1)];
                    ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j - 1)];
                    ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
                }

                // Activates if the element in the cell (here (Rows - 1)-1)# is in the radii criterium compared to the ij# (here (Rows - 1)-2) element
                //  O.O.O
                //  X.X.O
                //  O.O.O
                if((ArrayTable[i * Columns + (j - 1)].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i + 1) * Columns + (j - 1)].second > ArrayTable[i * Columns + j].second - dx))
                {
                    ArrayTableElement1 = ArrayTable[i * Columns + j];
                    ArrayTableElement2 = ArrayTable[i * Columns + (j - 1)];
                    ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
                }
            }

            // BOTTOM RIGHT corner
            else if(i == Rows - 2 && j == Columns - 2)
            {
                
                // Activates if the element in the BOTTOM RIGHT corner (here Rows-Columns) is in the radii criterium compared to the (i+1)j# (here (Rows - 1)-(Columns - 2)) element
                //  O.O.O
                //  O.O.O
                //  O.X.X
                if((ArrayTable[(i + 1) * Columns + (j + 1)].second < ArrayTable[(i + 1) * Columns + j].second + dx) && (ArrayTable[(i + 1) * Columns + (j + 1)].second > ArrayTable[(i + 1) * Columns + j].second - dx))
                {
                    ArrayTableElement1 = ArrayTable[(i + 1) * Columns + j];
                    ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j + 1)];
                    ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
                }

                // Activates if the element in the BOTTOM RIGHT corner (here Rows-Columns) is in the radii criterium compared to the i(j+1)# (here (Rows - 2)-(Columns - 1)) element
                //  O.O.O
                //  O.O.X
                //  O.O.X
                if((ArrayTable[(i + 1) * Columns + (j + 1)].second < ArrayTable[i * Columns + (j + 1)].second + dx) && (ArrayTable[(i + 1) * Columns + (j + 1)].second > ArrayTable[i * Columns + (j + 1)].second - dx))
                {
                    ArrayTableElement1 = ArrayTable[i * Columns + (j + 1)];
                    ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j + 1)];
                    ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2, dx);
                }
            }

            // Uppermost bar of tiles
            else if(i == 1 && j != 1 && j != Columns - 2)
            {
                ClusteredGroups = UppermostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
            }

            // Bottommost bar of tiles
            else if(i == Rows - 2 && j != 1 && j != Columns - 2)
            {
                ClusteredGroups = BottommostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
            }

            // Leftmost bar of tiles
            else if(i != 1 && i != Rows - 2 && j == 1)
            {
                ClusteredGroups = LeftmostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
            }

            // Rightmost bar of tiles
            else if(i != 1 && i != Rows - 2 && j == Columns - 2)
            {
                ClusteredGroups = RightmostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
            }

            // Other tiles
            else
            {} // Only the basic move needed
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

    // Cluster with bruteforce method
    auto ClusteredGroups = ClusteringBruteForce(ArrayTable, Rows, Columns, dx);

}