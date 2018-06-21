#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <random>
#include <thread>
#include <ctime>

// ####### 0. GLOBAL PARAMETERS #######

// A global string for indicating current version of program
std::string VersionNumber = "1.0";

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
        std::cout << ">> Number of Rows (SHOULD BE AT LEAST 5): ";
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
        std::cout << ">> Number of Columns (SHOULD BE AT LEAST 5): ";
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
std::map<int, std::vector<std::string>> ClusterStep(std::map<int, std::vector<std::string>> ClusteredGroups, std::pair<std::string,double> ArrayTableElement1, std::pair<std::string,double> ArrayTableElement2)
{
    // Measure time
    std::clock_t StartTime;
    StartTime = std::clock();

    // Iterator for the map structure
    std::map<int, std::vector<std::string>>::iterator MapIteratorMain;
    std::map<int, std::vector<std::string>>::iterator MapIteratorSub1;
    std::map<int, std::vector<std::string>>::iterator MapIteratorSub2;

    // Iterator for the vector structure
    std::vector<std::string>::iterator StringVectorIterator;

    // Temporary storages for specific indeces while iterating through the entries of a map
    int TempStrorageForErase = -1;
    int TempStrorageForSearch;

    // Bool indicator for various purpose
    int IndicatorMain = 0;
    int Indicator = 0;

    std::cout << "INFORMATIVE MESSAGE: Check is now executed between tile " << ArrayTableElement1.first << " and " << ArrayTableElement2.first << std::endl;

    if(ClusteredGroups.size() == 0)
    {
        // Increment the counter for the number of clusters to 1
        NumberOfClusterGroups = 1;

        // Make a temporary storage for the first two cluster element
        std::vector<std::string> TempStorage = {ArrayTableElement1.first, ArrayTableElement2.first};

        // Put them insode the std::map structure as first entries
        ClusteredGroups[NumberOfClusterGroups] = TempStorage;

        // Clear the temporary storage
        TempStorage.clear();
    }

    else
    {
        // Run through the vectors respect to them keys in the std::map structure
        for(MapIteratorMain = ClusteredGroups.begin(); MapIteratorMain != ClusteredGroups.end(); ++MapIteratorMain)
        {
            // Activates if the first element is already inside one of the vectors in the std::map structure
            // Particulary in the MapIterator# group
            if(std::find(MapIteratorMain->second.begin(), MapIteratorMain->second.end(), ArrayTableElement1.first) != MapIteratorMain->second.end())
            {
                // Indicates if first element is found
                IndicatorMain = 1;

                for(MapIteratorSub1 = ClusteredGroups.begin(); MapIteratorSub1 != ClusteredGroups.end(); ++MapIteratorSub1)
                {
                    // Check if the second element is inside any of the std::map's entries
                    // Indicator indicates if second element is found
                    if(std::find(MapIteratorSub1->second.begin(), MapIteratorSub1->second.end(), ArrayTableElement2.first) != MapIteratorSub1->second.end())
                    {
                        // If the second element is already inside the SAME group, then everything is all right, do nothing
                        if(MapIteratorSub1->first == MapIteratorMain->first)
                        {
                            Indicator = 1;
                            break;
                        }

                        // If they aren't, then merge the two group, and refresh the 'ClusteredGroups' map's keys
                        else
                        {
                            if(MapIteratorSub1->first > MapIteratorMain->first)
                            {
                                ClusteredGroups[MapIteratorMain->first].insert(ClusteredGroups[MapIteratorMain->first].end(), ClusteredGroups[MapIteratorSub1->first].begin(), ClusteredGroups[MapIteratorSub1->first].end());
                                TempStrorageForErase = MapIteratorSub1->first;
                                ClusteredGroups.erase(MapIteratorSub1->first);
                                Indicator = 1;
                                break;
                            }

                            else if(MapIteratorSub1->first < MapIteratorMain->first)
                            {
                                ClusteredGroups[MapIteratorSub1->first].insert(ClusteredGroups[MapIteratorSub1->first].end(), ClusteredGroups[MapIteratorMain->first].begin(), ClusteredGroups[MapIteratorMain->first].end());
                                TempStrorageForErase = MapIteratorMain->first;
                                ClusteredGroups.erase(MapIteratorMain->first);
                                Indicator = 1;
                                break;
                            }
                        }
                    }
                }

                // If the second element isn't in any group, then put it into there, where the first one is
                if(Indicator == 0)
                {
                    ClusteredGroups[MapIteratorMain->first].push_back(ArrayTableElement2.first);
                    break;
                }

                else
                {
                    Indicator = 0;
                    break;
                }
            }
        }

        // What if the first element isn't inside a group?
        if(IndicatorMain == 0)
        {
            // Check if the second element is already contained by a group
            for(MapIteratorSub2 = ClusteredGroups.begin(); MapIteratorSub2 != ClusteredGroups.end(); ++MapIteratorSub2)
            {
                // If it is, then insert the first element into that too
                if(std::find(MapIteratorSub2->second.begin(), MapIteratorSub2->second.end(), ArrayTableElement2.first) != MapIteratorSub2->second.end())
                {
                    ClusteredGroups[MapIteratorSub2->first].push_back(ArrayTableElement1.first);
                    Indicator = 1;
                    break;
                }
            }

            // If none of them is inside a group, then create a new group
            if(Indicator == 0)
            {
                // Increment the counter for the number of the cluster groups
                int *TempForNumberOfCG;
                TempForNumberOfCG = &NumberOfClusterGroups;
                ++*TempForNumberOfCG;

                std::vector<std::string> TempStorage = {ArrayTableElement1.first, ArrayTableElement2.first};

                ClusteredGroups[NumberOfClusterGroups] = TempStorage;

                TempStorage.clear();
            }
        }
    }

    if(TempStrorageForErase != -1)
    {
        for(MapIteratorMain = ClusteredGroups.begin(); MapIteratorMain != ClusteredGroups.end(); ++MapIteratorMain)
        {
            TempStrorageForSearch = TempStrorageForErase + 1;
            if(MapIteratorMain->first == TempStrorageForSearch)
            {
                ClusteredGroups[TempStrorageForErase] = ClusteredGroups[TempStrorageForSearch];
                ClusteredGroups.erase(TempStrorageForSearch);
                ++TempStrorageForErase;
            }
        }
        TempStrorageForErase = -1;
    }

    std::cout << "Elapsed time: " << (std::clock() - StartTime) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n" << std::endl;

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
        ArrayTableElement1 = ArrayTable[i * Columns + j];
        ArrayTableElement2 = ArrayTable[(i + 1) * Columns + j];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    //  O.O.O
    //  O.X.X
    //  O.O.O
    if((ArrayTable[i * Columns + (j + 1)].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[i * Columns + (j + 1)].second > ArrayTable[i * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + j];
        ArrayTableElement2 = ArrayTable[i * Columns + (j + 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
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
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // Activates if the element in the cell (i-1)(j+1)# (here 1-3) is in the radii criterium compared to the (i-1)j# (here 1-2) element
    //  O.X.X
    //  O.O.O
    //  O.O.O
    if((ArrayTable[(i - 1) * Columns + (j + 1)].second < ArrayTable[(i - 1) * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + (j + 1)].second > ArrayTable[(i - 1) * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[(i - 1) * Columns + j];
        ArrayTableElement2 = ArrayTable[(i - 1) * Columns + (j + 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
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
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // Activates if the element in the cell (i+1)(j-1)# (here 3-1) is in the radii criterium compared to the i(j-1)# (here 2-1) element
    //  O.O.O
    //  X.O.O
    //  X.O.O
    if((ArrayTable[(i + 1) * Columns + (j - 1)].second < ArrayTable[i * Columns + (j - 1)].second + dx) && (ArrayTable[(i + 1) * Columns + (j - 1)].second > ArrayTable[i * Columns + (j - 1)].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + (j - 1)];
        ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j - 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
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
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
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
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
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

    // Element indeces
    int i;
    int j;


    // We only on run through the tiles, which are situated inside the outermost frame of our 2D table
    // This means we start from the 2-2 element
    // The X-s indicates, which elements are compared around the middle one

    // STEP 1.
    // We check the corner elements, they all have their own unique moves
    // O.O.O.O.O.O.O
    // O.X.O.O.O.X.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.X.O.O.O.X.O
    // O.O.O.O.O.O.O
    
    // 1./a
    // UPPER LEFT corner
    i = 1;
    j = 1;

    ClusteredGroups = BasicClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
    ClusteredGroups = UppermostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
    ClusteredGroups = LeftmostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);

    // Activates if the element in the UPPER LEFT corner (here 0-0) is in the radii criterium compared to the (i-1)j# (here 0-1) element
    //  X.X.O
    //  O.O.O
    //  O.O.O
    if((ArrayTable[(i - 1) * Columns + (j - 1)].second < ArrayTable[(i - 1) * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + (j - 1)].second > ArrayTable[(i - 1) * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[(i - 1) * Columns + j];
        ArrayTableElement2 = ArrayTable[(i - 1) * Columns + (j - 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // Activates if the element in the UPPER LEFT corner (here 0-0) is in the radii criterium compared to the i(j-1)# (here 1-0) element
    //  X.O.O
    //  X.O.O
    //  O.O.O
    if((ArrayTable[(i - 1) * Columns + (j - 1)].second < ArrayTable[i * Columns + (j - 1)].second + dx) && (ArrayTable[(i - 1) * Columns + (j - 1)].second > ArrayTable[i * Columns + (j - 1)].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + (j - 1)];
        ArrayTableElement2 = ArrayTable[(i - 1) * Columns + (j - 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // 1./b
    // UPPER RIGHT corner
    i = 1;
    j = Columns - 2;
    ClusteredGroups = BasicClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
    ClusteredGroups = RightmostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);

    // Activates if the element in the cell (i-1)j# (here 0-(Columns - 2)) is in the radii criterium compared to the ij# (here 1-(Cols - 2)) element
    //  O.X.O
    //  O.X.O
    //  O.O.O
    if((ArrayTable[(i - 1) * Columns + j].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + j].second > ArrayTable[i * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + j];
        ArrayTableElement2 = ArrayTable[(i - 1) * Columns + j];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // Activates if the element in the UPPER RIGHT corner (here 0-(Cols - 1)) is in the radii criterium compared to the i(j+1)# (here 1-(Cols - 1)) element
    //  O.O.X
    //  O.O.X
    //  O.O.O
    if((ArrayTable[(i - 1) * Columns + (j + 1)].second < ArrayTable[i * Columns + (j + 1)].second + dx) && (ArrayTable[(i - 1) * Columns + (j + 1)].second > ArrayTable[i * Columns + (j + 1)].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + (j + 1)];
        ArrayTableElement2 = ArrayTable[(i - 1) * Columns + (j + 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // Activates if the element in the UPPER RIGHT corner (here 0-(Cols - 1)) is in the radii criterium compared to the (i-1)j# (here 0-(Cols - 2)) element
    //  O.X.X
    //  O.O.O
    //  O.O.O
    if((ArrayTable[(i - 1) * Columns + (j + 1)].second < ArrayTable[(i - 1) * Columns + j].second + dx) && (ArrayTable[(i - 1) * Columns + (j + 1)].second > ArrayTable[(i - 1) * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[(i - 1) * Columns + j];
        ArrayTableElement2 = ArrayTable[(i - 1) * Columns + (j + 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // 1./c
    // BOTTOM LEFT corner
    i = Rows - 2;
    j = 1;

    ClusteredGroups = BasicClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
    ClusteredGroups = BottommostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
    
    // Activates if the element in the cell (here (Rows - 2)-0)# is in the radii criterium compared to the ij# (here (Rows - 2)-1) element
    //  O.O.O
    //  X.X.O
    //  O.O.O
    if((ArrayTable[i * Columns + (j - 1)].second < ArrayTable[i * Columns + j].second + dx) && (ArrayTable[(i + 1) * Columns + (j - 1)].second > ArrayTable[i * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + j];
        ArrayTableElement2 = ArrayTable[i * Columns + (j - 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // Activates if the element in the BOTTOM LEFT corner (here (Rows - 1)-0) is in the radii criterium compared to the (i+1)j# (here (Rows - 1)-1) element
    //  O.O.O
    //  O.O.O
    //  X.X.O
    if((ArrayTable[(i + 1) * Columns + (j - 1)].second < ArrayTable[(i + 1) * Columns + j].second + dx) && (ArrayTable[(i + 1) * Columns + (j - 1)].second > ArrayTable[(i + 1) * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[(i + 1) * Columns + j];
        ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j - 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // Activates if the element in the BOTTOM LEFT corner (here (Rows - 1)-0) is in the radii criterium compared to the i(j-1)# (here (Rows - 2)-0) element
    //  O.O.O
    //  X.O.O
    //  X.O.O
    if((ArrayTable[(i + 1) * Columns + (j - 1)].second < ArrayTable[i * Columns + (j - 1)].second + dx) && (ArrayTable[(i + 1) * Columns + (j - 1)].second > ArrayTable[i * Columns + (j - 1)].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + (j - 1)];
        ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j - 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // 1./d
    // BOTTOM RIGHT corner
    i = Rows - 2;
    j = Columns - 2;

    ClusteredGroups = BasicClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
                
    // Activates if the element in the BOTTOM RIGHT corner (here Rows-Columns) is in the radii criterium compared to the (i+1)j# (here (Rows - 1)-(Columns - 2)) element
    //  O.O.O
    //  O.O.O
    //  O.X.X
    if((ArrayTable[(i + 1) * Columns + (j + 1)].second < ArrayTable[(i + 1) * Columns + j].second + dx) && (ArrayTable[(i + 1) * Columns + (j + 1)].second > ArrayTable[(i + 1) * Columns + j].second - dx))
    {
        ArrayTableElement1 = ArrayTable[(i + 1) * Columns + j];
        ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j + 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // Activates if the element in the BOTTOM RIGHT corner (here Rows-Columns) is in the radii criterium compared to the i(j+1)# (here (Rows - 2)-(Columns - 1)) element
    //  O.O.O
    //  O.O.X
    //  O.O.X
    if((ArrayTable[(i + 1) * Columns + (j + 1)].second < ArrayTable[i * Columns + (j + 1)].second + dx) && (ArrayTable[(i + 1) * Columns + (j + 1)].second > ArrayTable[i * Columns + (j + 1)].second - dx))
    {
        ArrayTableElement1 = ArrayTable[i * Columns + (j + 1)];
        ArrayTableElement2 = ArrayTable[(i + 1) * Columns + (j + 1)];
        ClusteredGroups = ClusterStep(ClusteredGroups, ArrayTableElement1, ArrayTableElement2);
    }

    // STEP 2.
    // We run through the outer frame of our iteration area, but except corner tiles
    // O.O.O.O.O.O.O
    // O.O.X.X.X.O.O
    // O.X.O.O.O.X.O
    // O.X.O.O.O.X.O
    // O.O.X.X.X.O.O
    // O.O.O.O.O.O.O

    // 2./a
    // Run through the uppermost bar of tiles
    // O.O.O.O.O.O.O
    // O.O.X.X.X.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    i = 1;
    for(int j = 2; j <= Columns - 3; j++)
    {
        ClusteredGroups = BasicClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
        ClusteredGroups = UppermostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
    }

    // 2./b
    // Run throught the bottommost bar of tiles
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.X.X.X.O.O
    // O.O.O.O.O.O.O
    i = Rows - 2;
    for(int j = 2; j <= Columns - 3; j++)
    {
        ClusteredGroups = BasicClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
        ClusteredGroups = BottommostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
    }

    // 2./c
    // Run throught the leftmost bar of tiles
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.X.O.O.O.O.O
    // O.X.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    j = 1;
    for(i = 2; i <= Rows - 3; i++)
    {
        ClusteredGroups = BasicClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
        ClusteredGroups = LeftmostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
    }

    // 2./d
    // Run throught the rightmost bar of tiles
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.X.O
    // O.O.O.O.O.X.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    j = Columns - 2;
    for(i = 2; i <= Rows - 3; i++)
    {
        ClusteredGroups = BasicClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
        ClusteredGroups = RightmostBarClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
    }

    // STEP 3.
    // We run through on the inside of our table two frame deep
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.X.X.X.O.O
    // O.O.X.X.X.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    for(int i = 2; i <= Rows - 3; i++)
    {
        for(int j = 2; j <= Columns - 3; j++)
        {
            // Basic step in the clustering, needed for every step
            ClusteredGroups = BasicClusterFrame(ClusteredGroups, ArrayTable, Rows, Columns, i, j, dx);
        }
    }

    return ClusteredGroups;
}


// ####### 3. WRITE DATA INTO FILE #######

void WriteToFile(std::map<int, std::vector<std::string>> ClusteredGroups)
{
    // Iterator for the map structure
    std::map<int, std::vector<std::string>>::iterator MapIterator;

    // Iterator for the vector structure
    std::vector<std::string>::iterator StringVectorIterator;

    // Make a textfile to store output data
    std::ofstream OutputFile;
    OutputFile.open("ClusteredData.txt");

    // Write a header for the output file
    OutputFile << "Indeces of  |  Elements of\nclusters    |  clusters" << std::endl;
    // Write the clusters' data into the file
    for(MapIterator = ClusteredGroups.begin(); MapIterator != ClusteredGroups.end(); ++MapIterator)
    {
        OutputFile << "Cluster #" << MapIterator->first << ":     {";
        for(StringVectorIterator = MapIterator->second.begin(); StringVectorIterator != MapIterator->second.end(); ++StringVectorIterator)
        {
            OutputFile << *StringVectorIterator << "; ";
        }
        OutputFile << "END}" << std::endl;
    }

    OutputFile.close();

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
    PrintElements(ArrayTable, Rows, Columns);

    // Cluster with bruteforce method
    auto ClusteredGroups = ClusteringBruteForce(ArrayTable, Rows, Columns, dx);
    std::cout << "INFORMATIVE MESSAGE: Clustering steps finished!\n" << std::endl;

    // Write data to output file
    WriteToFile(ClusteredGroups);
    std::cout << "INFORMATIVE MESSAGE: Output file created!\n" << std::endl;

    std::cout << "INFORMATIVE MESSAGE: Clustering is completed!\n" << std::endl;
}