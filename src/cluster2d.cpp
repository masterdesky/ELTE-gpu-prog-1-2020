//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//      _____              _____ _           _            _                     //
//     /  __ \ _     _    /  __ \ |         | |          (_)                    //
//     | /  \/| |_ _| |_  | /  \/ |_   _ ___| |_ ___ _ __ _ _ __   __ _         //
//     | |  |_   _|_   _| | |   | | | | / __| __/ _ \ '__| | '_ \ / _` |        //
//     | \__/\|_|   |_|   | \__/\ | |_| \__ \ ||  __/ |  | | | | | (_| |        //
//      \____/             \____/_|\__,_|___/\__\___|_|  |_|_| |_|\__, |        //
//                                                                 __/ |        //
//                                                                |___/         //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include <random>
#include <chrono>
#include <ctime>

#include <future>
#include <thread>

// ####### GLOBAL PARAMETERS #######

// A global string for indicating current version of program
std::string version_number = "1.0";

// Counter for the number of cluster group = 0;
int n_clusters = 0;

// ####### 0. AUXILIARY FUNCTIONS #######
// None yet

// ####### 1. INITIALIZING FUNCTIONS #######

// Initialize starting parameters
// rows: Number of rows of the 2D data table
// columns: Number of columns of the 2D data table
// max_element: Maximum value of elements of the 2D data table
// dx: Largest allowed distance between two neighbouring tile
std::tuple<int, int, double, double> Initialization()
{
    int rows;
    int columns;
    double max_element;
    double dx;

    std::string Choosedx;

    while(1)
    {
        std::cout << ">> Please enter the number of rows of the 2D data table!" << std::endl;
        std::cout << ">> Number of rows (SHOULD BE AT LEAST 5): ";
        std::cin >> rows;
        if(int(rows) != rows)
        {
            std::cout << "ERROR: Number of rows should be an integer!\n" << std::endl; 
        }
        else
        {
            std::cout << "\n\n";
            break;
        }
    }

    while(1)
    {
        std::cout << ">> Please enter the number of columns of the 2D data table!" << std::endl;
        std::cout << ">> Number of columns (SHOULD BE AT LEAST 5): ";
        std::cin >> columns;
        if(int(columns) != columns)
        {
            std::cout << "ERROR: Number of columns should be an integer!\n" << std::endl; 
        }
        else if(int(columns) < 5)
        {
            std::cout << "ERROR: Number of columns should be at least 5!\n" << std::endl;
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
        std::cin >> max_element;
        if(max_element <= 0)
        {
            std::cout << "ERROR: The maximum element should be a positive real number!!\n" << std::endl; 
        }
        else if(max_element > 0)
        {
            std::cout << "\n\n";
            break;
        }
        else
        {
            std::cout << "ERROR: Not permitted input!\n";
            std::cout << "       The maximum element should be a positive real number! Please try again!" << std::endl;
        }
    }

    while(1)
    {
        std::cout << ">> Please define the maximum distance between two neighbouring elements!\n";
        std::cout << ">> If the difference between two elements is greater than this value, then \n";
        std::cout << ">> they won't placed into the same cluster (Should be a positive real number)" << std::endl;
        std::cout << ">> Maximal difference (dx): ";
        std::cin >> dx;

        if(max_element <= 0)
        {
            std::cout << "ERROR: The maximum element should be a positive real number!!\n" << std::endl; 
        }

        else if(dx > max_element)
        {
            std::cout << "WARNING: \'dx\' is greater than the maximal value in your table. This means, that\n";
            std::cout << "         every element in the table will be classified into one cluster!\n";
            std::cout << "         Would you like to continue or give an other value for dx?" << std::endl;
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

        else if(dx < max_element)
        {
            std::cout << "\n\n";
            break;
        }

        else
        {
            std::cout << "ERROR: Not permitted input!\n";
            std::cout << "       The value of \'dx\' should be a positive real number! Please try again!" << std::endl;
        }
    }

    return(std::make_tuple(rows, columns, max_element, dx));

}


// Generate a 2D data table
// This will look something like this (if max_element == 1):
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
// #ofrows - #ofcolumns
std::vector<std::pair<std::string,double>> gen_2d_table(int rows, int columns, double max_element)
{
    std::vector<std::pair<std::string,double>> table;
    std::stringstream pos_idx;

    //Randomize
    std::random_device Randomized;
    std::mt19937 Generate(Randomized());
    std::uniform_real_distribution<> Distribution(0, max_element);

    for(int i = 1; i <= rows; i++)
    {
        for(int j = 1; j <= columns; j++)
        {
            // Clear stringstream
            pos_idx.str(std::string());

            // Create a string which indicates the position of the element in the 2D matrix
            pos_idx << i << "," << j;
            std::string pos_idx_string = pos_idx.str();

            //Generate a random double
            double random_element = Distribution(Generate);

            // Create a pair for the current element in the table:
            // 1. First element is the string above
            // 2. Second is the actual number in that particular location
            std::pair<std::string, double> pos_idx_pair;

            pos_idx_pair = std::make_pair(pos_idx_string, random_element);
            table.push_back(pos_idx_pair);
        }
    }

    return(table);
}


// Test
// Print out the generated 2D data table's elements
void PrintElements(std::vector<std::pair<std::string,double>> table, int rows, int columns)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            std::cout << table[i * columns + j].second << " ";
        }
        std::cout << "\n";
    }
}


// ####### 2. CLUSTERING #######

// --- Auxiliary function ---
// - Does the following:
// 1. Activates if both of the neighbouring tiles are inside the same radii criterium
// 2. The function then classify them to the proper cluster (group)
//
// - Legends:
// table_element_1: The first element, to which we compare the other element (CALLED AS FIRST ELEMENT)
// table_element_2: The second element, which is compared to the first one (CALLED AS SECOND ELEMENT)
std::map<int, std::vector<std::string>> ClusterStep(std::map<int, std::vector<std::string>> clustered_groups,
                                                    std::pair<std::string,double> table_element_1,
                                                    std::pair<std::string,double> table_element_2)
{

    // Iterator for the map structure
    std::map<int, std::vector<std::string>>::iterator map_iterator_main;
    std::map<int, std::vector<std::string>>::iterator map_iterator_sub_1;
    std::map<int, std::vector<std::string>>::iterator map_iterator_sub_2;

    // Iterator for the vector structure
    std::vector<std::string>::iterator string_vector_iterator;

    // Temporary storages for specific indeces while iterating through the entries of a map
    int temp_storage_erase = -1;
    int temp_strorage_search;

    // Bool indicator for various purpose
    int indicator_main = 0;
    int indicator = 0;

    //std::cout << "INFORMATIVE MESSAGE: Check is now executed between tile " << table_element_1.first << " and " << table_element_2.first << std::endl;

    if(clustered_groups.size() == 0)
    {
        // Increment the counter for the number of clusters to 1
        n_clusters = 1;

        // Make a temporary storage for the first two cluster element
        std::vector<std::string> temp_storage = {table_element_1.first, table_element_2.first};

        // Put them insode the std::map structure as first entries
        clustered_groups[n_clusters] = temp_storage;

        // Clear the temporary storage
        temp_storage.clear();
    }

    else
    {
        // Run through the vectors respect to them keys in the std::map structure
        for(map_iterator_main = clustered_groups.begin(); map_iterator_main != clustered_groups.end(); ++map_iterator_main)
        {
            // Activates if the first element is already inside one of the vectors in the std::map structure
            // Particularly in the map_iterator# group
            if(std::find(map_iterator_main->second.begin(), map_iterator_main->second.end(), table_element_1.first) != map_iterator_main->second.end())
            {
                // Indicates that the first element is found
                indicator_main = 1;

                for(map_iterator_sub_1 = clustered_groups.begin(); map_iterator_sub_1 != clustered_groups.end(); ++map_iterator_sub_1)
                {
                    // Check if the second element is inside any of the std::map's entries
                    // indicator indicates if second element is found
                    if(std::find(map_iterator_sub_1->second.begin(), map_iterator_sub_1->second.end(), table_element_2.first) != map_iterator_sub_1->second.end())
                    {
                        // If the second element is already inside the SAME group, then everything is all right, do nothing
                        if(map_iterator_sub_1->first == map_iterator_main->first)
                        {
                            indicator = 1;
                            break;
                        }

                        // If they aren't, then merge the two group, and refresh the 'clustered_groups' map's keys
                        else
                        {
                            if(map_iterator_sub_1->first > map_iterator_main->first)
                            {
                                clustered_groups[map_iterator_main->first].insert(clustered_groups[map_iterator_main->first].end(),
                                                                                  clustered_groups[map_iterator_sub_1->first].begin(),
                                                                                  clustered_groups[map_iterator_sub_1->first].end());
                                temp_storage_erase = map_iterator_sub_1->first;
                                clustered_groups.erase(map_iterator_sub_1->first);
                                indicator = 1;
                                break;
                            }

                            else if(map_iterator_sub_1->first < map_iterator_main->first)
                            {
                                clustered_groups[map_iterator_sub_1->first].insert(clustered_groups[map_iterator_sub_1->first].end(),
                                                                                   clustered_groups[map_iterator_main->first].begin(),
                                                                                   clustered_groups[map_iterator_main->first].end());
                                temp_storage_erase = map_iterator_main->first;
                                clustered_groups.erase(map_iterator_main->first);
                                indicator = 1;
                                break;
                            }
                        }
                    }
                }

                // If the second element isn't in any group, then put it into there, where the first one is
                if(indicator == 0)
                {
                    clustered_groups[map_iterator_main->first].push_back(table_element_2.first);
                    break;
                }

                else
                {
                    indicator = 0;
                    break;
                }
            }
        }

        // What if the first element isn't inside a group?
        if(indicator_main == 0)
        {
            // Check if the second element is already contained by a group
            for(map_iterator_sub_2 = clustered_groups.begin(); map_iterator_sub_2 != clustered_groups.end(); ++map_iterator_sub_2)
            {
                // If it is, then insert the first element into that too
                if(std::find(map_iterator_sub_2->second.begin(), map_iterator_sub_2->second.end(), table_element_2.first) != map_iterator_sub_2->second.end())
                {
                    clustered_groups[map_iterator_sub_2->first].push_back(table_element_1.first);
                    indicator = 1;
                    break;
                }
            }

            // If none of them is inside a group, then create a new group
            if(indicator == 0)
            {
                // Increment the counter for the number of the cluster groups
                int *temp_n_clusers;
                temp_n_clusers = &n_clusters;
                ++*temp_n_clusers;

                std::vector<std::string> temp_storage = {table_element_1.first, table_element_2.first};

                clustered_groups[n_clusters] = temp_storage;

                temp_storage.clear();
            }
        }
    }

    if(temp_storage_erase != -1)
    {
        for(map_iterator_main = clustered_groups.begin(); map_iterator_main != clustered_groups.end(); ++map_iterator_main)
        {
            temp_strorage_search = temp_storage_erase + 1;
            if(map_iterator_main->first == temp_strorage_search)
            {
                clustered_groups[temp_storage_erase] = clustered_groups[temp_strorage_search];
                clustered_groups.erase(temp_strorage_search);
                ++temp_storage_erase;
            }
        }
        temp_storage_erase = -1;
    }

    //std::cout << "Elapsed time: " << (std::clock() - StartTime) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n" << std::endl;

    return(clustered_groups);
}

// Frame for the basic check in an individual step of the clustering
// For every tile, we need to do (at least) the following two step
// The X-s indicates, which elements are compared around the middle one
std::map<int, std::vector<std::string>> BasicClusterFrame(std::map<int, std::vector<std::string>> clustered_groups, std::vector<std::pair<std::string,double>> table, int rows, int columns, int i, int j, double dx)
{
    std::pair<std::string,double> table_element_1;
    std::pair<std::string,double> table_element_2;

    //  O.O.O
    //  O.X.O
    //  O.X.O
    if((table[(i + 1) * columns + j].second < table[i * columns + j].second + dx) && (table[(i + 1) * columns + j].second > table[i * columns + j].second - dx))
    {
        table_element_1 = table[i * columns + j];
        table_element_2 = table[(i + 1) * columns + j];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    //  O.O.O
    //  O.X.X
    //  O.O.O
    if((table[i * columns + (j + 1)].second < table[i * columns + j].second + dx) && (table[i * columns + (j + 1)].second > table[i * columns + j].second - dx))
    {
        table_element_1 = table[i * columns + j];
        table_element_2 = table[i * columns + (j + 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    return(clustered_groups);

}


// Function for the uppermost bar of tiles
// O.X.X.X.X.X.O
// O.O.O.O.O.O.O
// O.O.O.O.O.O.O
// O.O.O.O.O.O.O
std::map<int, std::vector<std::string>> UppermostBarClusterFrame(std::map<int, std::vector<std::string>> clustered_groups, std::vector<std::pair<std::string,double>> table, int rows, int columns, int i, int j, double dx)
{
    std::pair<std::string,double> table_element_1;
    std::pair<std::string,double> table_element_2;

    // Activates if the element in the cell (i-1)j# (here 1-2) is in the radii criterium compared to the ij# (here 2-2) element
    //  O.X.O
    //  O.X.O
    //  O.O.O
    if((table[(i - 1) * columns + j].second < table[i * columns + j].second + dx) && (table[(i - 1) * columns + j].second > table[i * columns + j].second - dx))
    {
        table_element_1 = table[i * columns + j];
        table_element_2 = table[(i - 1) * columns + j];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    // Activates if the element in the cell (i-1)(j+1)# (here 1-3) is in the radii criterium compared to the (i-1)j# (here 1-2) element
    //  O.X.X
    //  O.O.O
    //  O.O.O
    if((table[(i - 1) * columns + (j + 1)].second < table[(i - 1) * columns + j].second + dx) && (table[(i - 1) * columns + (j + 1)].second > table[(i - 1) * columns + j].second - dx))
    {
        table_element_1 = table[(i - 1) * columns + j];
        table_element_2 = table[(i - 1) * columns + (j + 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    return(clustered_groups);
}

// Function for the leftmost bar of tiles
// O.O.O.O
// X.O.O.O
// X.O.O.O
// X.O.O.O
// X.O.O.O
// O.O.O.O
std::map<int, std::vector<std::string>> LeftmostBarClusterFrame(std::map<int, std::vector<std::string>> clustered_groups, std::vector<std::pair<std::string,double>> table, int rows, int columns, int i, int j, double dx)
{
    std::pair<std::string,double> table_element_1;
    std::pair<std::string,double> table_element_2;

    // Activates if the element in the cell i(j-1)# (here 2-1) is in the radii criterium compared to the ij# (here 2-2) element
    //  O.O.O
    //  X.X.O
    //  O.O.O
    if((table[i * columns + (j - 1)].second < table[i * columns + j].second + dx) && (table[i * columns + (j - 1)].second > table[i * columns + j].second - dx))
    {
        table_element_1 = table[i * columns + (j - 1)];
        table_element_2 = table[i * columns + j];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    // Activates if the element in the cell (i+1)(j-1)# (here 3-1) is in the radii criterium compared to the i(j-1)# (here 2-1) element
    //  O.O.O
    //  X.O.O
    //  X.O.O
    if((table[(i + 1) * columns + (j - 1)].second < table[i * columns + (j - 1)].second + dx) && (table[(i + 1) * columns + (j - 1)].second > table[i * columns + (j - 1)].second - dx))
    {
        table_element_1 = table[i * columns + (j - 1)];
        table_element_2 = table[(i + 1) * columns + (j - 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    return(clustered_groups);
}

// Function for the rightmost bar of tiles
// O.O.O.O
// O.O.O.X
// O.O.O.X
// O.O.O.X
// O.O.O.X
// O.O.O.O
std::map<int, std::vector<std::string>> RightmostBarClusterFrame(std::map<int, std::vector<std::string>> clustered_groups, std::vector<std::pair<std::string,double>> table, int rows, int columns, int i, int j, double dx)
{
    std::pair<std::string,double> table_element_1;
    std::pair<std::string,double> table_element_2;

    // Activates if the element in the cell (i+1)(j+1)# is in the radii criterium compared to the i(j+1)# (here 3-(Cols)) element
    //  O.O.O
    //  O.O.X
    //  O.O.X
    if((table[(i + 1) * columns + (j + 1)].second < table[i * columns + (j + 1)].second + dx) && (table[(i + 1) * columns + (j + 1)].second > table[i * columns + (j + 1)].second - dx))
    {
        table_element_1 = table[i * columns + (j + 1)];
        table_element_2 = table[(i + 1) * columns + (j + 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    return(clustered_groups);
}

// Function for the bottommost bar of tiles
// O.O.O.O.O.O.O
// O.O.O.O.O.O.O
// O.O.O.O.O.O.O
// O.X.X.X.X.X.O
std::map<int, std::vector<std::string>> BottommostBarClusterFrame(std::map<int, std::vector<std::string>> clustered_groups, std::vector<std::pair<std::string,double>> table, int rows, int columns, int i, int j, double dx)
{
    std::pair<std::string,double> table_element_1;
    std::pair<std::string,double> table_element_2;

    // Activates if the element in the cell (i+1)(j+1)# is in the radii criterium compared to the (i+1)j# element
    //  O.O.O
    //  O.O.O
    //  O.X.X
    if((table[(i + 1) * columns + (j + 1)].second < table[(i + 1) * columns + j].second + dx) && (table[(i + 1) * columns + (j + 1)].second > table[(i + 1) * columns + j].second - dx))
    {
        table_element_1 = table[(i + 1) * columns + j];
        table_element_2 = table[(i + 1) * columns + (j + 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    return(clustered_groups);
}

// Brute force solution frame
std::map<int, std::vector<std::string>> ClusteringBruteForce(std::vector<std::pair<std::string,double>> table, int rows, int columns, double dx)
{
    // Container for the actual clusters
    std::map<int, std::vector<std::string>> clustered_groups;

    // Tomporary containers for the std::pairs of the table 
    std::pair<std::string,double> table_element_1;
    std::pair<std::string,double> table_element_2;

    // Element indeces
    int i;
    int j;


    // We only run through the tiles, which are situated inside the outermost frame of our 2D table
    // The ASCII figures in the comments below always indicates the currently executed comparsion of
    // two elements.
    // 

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

    clustered_groups = BasicClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
    clustered_groups = UppermostBarClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
    clustered_groups = LeftmostBarClusterFrame(clustered_groups, table, rows, columns, i, j, dx);

    // Activates if the element in the UPPER LEFT corner (here 0-0) is in the radii criterium compared to the (i-1)j# (here 0-1) element
    //  X.X.O
    //  O.O.O
    //  O.O.O
    if((table[(i - 1) * columns + (j - 1)].second < table[(i - 1) * columns + j].second + dx) && (table[(i - 1) * columns + (j - 1)].second > table[(i - 1) * columns + j].second - dx))
    {
        table_element_1 = table[(i - 1) * columns + j];
        table_element_2 = table[(i - 1) * columns + (j - 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    // Activates if the element in the UPPER LEFT corner (here 0-0) is in the radii criterium compared to the i(j-1)# (here 1-0) element
    //  X.O.O
    //  X.O.O
    //  O.O.O
    if((table[(i - 1) * columns + (j - 1)].second < table[i * columns + (j - 1)].second + dx) && (table[(i - 1) * columns + (j - 1)].second > table[i * columns + (j - 1)].second - dx))
    {
        table_element_1 = table[i * columns + (j - 1)];
        table_element_2 = table[(i - 1) * columns + (j - 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    // 1./b
    // UPPER RIGHT corner
    i = 1;
    j = columns - 2;
    clustered_groups = BasicClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
    clustered_groups = RightmostBarClusterFrame(clustered_groups, table, rows, columns, i, j, dx);

    // Activates if the element in the cell (i-1)j# (here 0-(columns - 2)) is in the radii criterium compared to the ij# (here 1-(Cols - 2)) element
    //  O.X.O
    //  O.X.O
    //  O.O.O
    if((table[(i - 1) * columns + j].second < table[i * columns + j].second + dx) && (table[(i - 1) * columns + j].second > table[i * columns + j].second - dx))
    {
        table_element_1 = table[i * columns + j];
        table_element_2 = table[(i - 1) * columns + j];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    // Activates if the element in the UPPER RIGHT corner (here 0-(Cols - 1)) is in the radii criterium compared to the i(j+1)# (here 1-(Cols - 1)) element
    //  O.O.X
    //  O.O.X
    //  O.O.O
    if((table[(i - 1) * columns + (j + 1)].second < table[i * columns + (j + 1)].second + dx) && (table[(i - 1) * columns + (j + 1)].second > table[i * columns + (j + 1)].second - dx))
    {
        table_element_1 = table[i * columns + (j + 1)];
        table_element_2 = table[(i - 1) * columns + (j + 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    // Activates if the element in the UPPER RIGHT corner (here 0-(Cols - 1)) is in the radii criterium compared to the (i-1)j# (here 0-(Cols - 2)) element
    //  O.X.X
    //  O.O.O
    //  O.O.O
    if((table[(i - 1) * columns + (j + 1)].second < table[(i - 1) * columns + j].second + dx) && (table[(i - 1) * columns + (j + 1)].second > table[(i - 1) * columns + j].second - dx))
    {
        table_element_1 = table[(i - 1) * columns + j];
        table_element_2 = table[(i - 1) * columns + (j + 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    // 1./c
    // BOTTOM LEFT corner
    i = rows - 2;
    j = 1;

    clustered_groups = BasicClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
    clustered_groups = BottommostBarClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
    
    // Activates if the element in the cell (here (rows - 2)-0)# is in the radii criterium compared to the ij# (here (rows - 2)-1) element
    //  O.O.O
    //  X.X.O
    //  O.O.O
    if((table[i * columns + (j - 1)].second < table[i * columns + j].second + dx) && (table[(i + 1) * columns + (j - 1)].second > table[i * columns + j].second - dx))
    {
        table_element_1 = table[i * columns + j];
        table_element_2 = table[i * columns + (j - 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    // Activates if the element in the BOTTOM LEFT corner (here (rows - 1)-0) is in the radii criterium compared to the (i+1)j# (here (rows - 1)-1) element
    //  O.O.O
    //  O.O.O
    //  X.X.O
    if((table[(i + 1) * columns + (j - 1)].second < table[(i + 1) * columns + j].second + dx) && (table[(i + 1) * columns + (j - 1)].second > table[(i + 1) * columns + j].second - dx))
    {
        table_element_1 = table[(i + 1) * columns + j];
        table_element_2 = table[(i + 1) * columns + (j - 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    // Activates if the element in the BOTTOM LEFT corner (here (rows - 1)-0) is in the radii criterium compared to the i(j-1)# (here (rows - 2)-0) element
    //  O.O.O
    //  X.O.O
    //  X.O.O
    if((table[(i + 1) * columns + (j - 1)].second < table[i * columns + (j - 1)].second + dx) && (table[(i + 1) * columns + (j - 1)].second > table[i * columns + (j - 1)].second - dx))
    {
        table_element_1 = table[i * columns + (j - 1)];
        table_element_2 = table[(i + 1) * columns + (j - 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    // 1./d
    // BOTTOM RIGHT corner
    i = rows - 2;
    j = columns - 2;

    clustered_groups = BasicClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
                
    // Activates if the element in the BOTTOM RIGHT corner (here rows-columns) is in the radii criterium compared to the (i+1)j# (here (rows - 1)-(columns - 2)) element
    //  O.O.O
    //  O.O.O
    //  O.X.X
    if((table[(i + 1) * columns + (j + 1)].second < table[(i + 1) * columns + j].second + dx) && (table[(i + 1) * columns + (j + 1)].second > table[(i + 1) * columns + j].second - dx))
    {
        table_element_1 = table[(i + 1) * columns + j];
        table_element_2 = table[(i + 1) * columns + (j + 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
    }

    // Activates if the element in the BOTTOM RIGHT corner (here rows-columns) is in the radii criterium compared to the i(j+1)# (here (rows - 2)-(columns - 1)) element
    //  O.O.O
    //  O.O.X
    //  O.O.X
    if((table[(i + 1) * columns + (j + 1)].second < table[i * columns + (j + 1)].second + dx) && (table[(i + 1) * columns + (j + 1)].second > table[i * columns + (j + 1)].second - dx))
    {
        table_element_1 = table[i * columns + (j + 1)];
        table_element_2 = table[(i + 1) * columns + (j + 1)];
        clustered_groups = ClusterStep(clustered_groups, table_element_1, table_element_2);
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
    for(int j = 2; j <= columns - 3; j++)
    {
        clustered_groups = BasicClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
        clustered_groups = UppermostBarClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
    }

    // 2./b
    // Run throught the bottommost bar of tiles
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.X.X.X.O.O
    // O.O.O.O.O.O.O
    i = rows - 2;
    for(int j = 2; j <= columns - 3; j++)
    {
        clustered_groups = BasicClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
        clustered_groups = BottommostBarClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
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
    for(i = 2; i <= rows - 3; i++)
    {
        clustered_groups = BasicClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
        clustered_groups = LeftmostBarClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
    }

    // 2./d
    // Run throught the rightmost bar of tiles
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.X.O
    // O.O.O.O.O.X.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    j = columns - 2;
    for(i = 2; i <= rows - 3; i++)
    {
        clustered_groups = BasicClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
        clustered_groups = RightmostBarClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
    }

    // STEP 3.
    // We run through on the inside of our table two frame deep
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    // O.O.X.X.X.O.O
    // O.O.X.X.X.O.O
    // O.O.O.O.O.O.O
    // O.O.O.O.O.O.O
    for(int i = 2; i <= rows - 3; i++)
    {
        for(int j = 2; j <= columns - 3; j++)
        {
            // Basic step in the clustering, needed for every step
            clustered_groups = BasicClusterFrame(clustered_groups, table, rows, columns, i, j, dx);
        }
    }

    return clustered_groups;
}


// ####### 3. WRITE DATA INTO FILE #######

void write_to_file(std::vector<std::pair<std::string,double>> table, int rows, int columns,
                   std::map<int, std::vector<std::string>> clustered_groups)
{
    std::ofstream output_file;

    // 1. Save generated table
    output_file.open("..//output//table.txt");

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            output_file << table[i * columns + j].second << " ";
        }
        output_file << "\n";
    }
    output_file.close();

    // 2. Save created clusters
    // Iterator for the map structure
    std::map<int, std::vector<std::string>>::iterator map_iterator;

    // Iterator for the vector structure
    std::vector<std::string>::iterator string_vector_iterator;

    // Make a textfile to store output data
    output_file.open("..//output//clustered_data.txt");

    // Write a header for the output file
    output_file << "Indeces of clusters\tElements of clusters" << std::endl;
    // Write the clusters' data into the file
    for(map_iterator = clustered_groups.begin(); map_iterator != clustered_groups.end(); ++map_iterator)
    {
        output_file << "cluster_" << map_iterator->first << "\t[[";
        for(string_vector_iterator = map_iterator->second.begin(); string_vector_iterator != map_iterator->second.end(); ++string_vector_iterator)
        {
            output_file << *string_vector_iterator << "],[";
        }
        output_file << "-1,-1]]" << std::endl;
    }
    output_file.close();
}


int main()
{
    std::cout << ">>>    CLUSTERING WITH C++ PARALLEL PROGRAMMING    <<<" << std::endl;
	std::cout << ">>>                  VERSION " << version_number << "                   <<<\n\n" << std::endl;

    // Parameter Initialization at the beginning of the program
    auto initial_parameters = Initialization();
    // Used variables
    int rows = std::get<0>(initial_parameters);
    int columns = std::get<1>(initial_parameters);
    double max_element = std::get<2>(initial_parameters);
    double dx = std::get<3>(initial_parameters);

    // Measuring time for generating data table begin
    auto time_begin_gen = std::chrono::high_resolution_clock::now();
    // Generate a 2D table with random floats, valued between 0 and a choosen number
    std::vector<std::pair<std::string,double>> table = gen_2d_table(rows, columns, max_element);
    std::cout << "INFORMATIVE MESSAGE: The data table has been created!\n" << std::endl;
    // Random 2D matrix genereation runtime
    auto time_end_gen = std::chrono::high_resolution_clock::now() - time_begin_gen;

    // Run test
    //PrintElements(table, rows, columns);

    // Define the clustered map first
    std::map<int, std::vector<std::string>> clustered_groups;

    // NATIVE BRUTEFORCE
    // Measuring time for bruteforce clustering begin
    auto time_begin_cls_nat = std::chrono::high_resolution_clock::now();
    // Cluster with bruteforce method
    clustered_groups = ClusteringBruteForce(table, rows, columns, dx);
    std::cout << "INFORMATIVE MESSAGE: Clustering steps finished!\n" << std::endl;
    // Bruteforce clustering runtime
    auto time_end_cls_nat = std::chrono::high_resolution_clock::now() - time_begin_cls_nat;

    // ASYNC THREADED BRUTEFORCE
    // Measuring time for bruteforce clustering begin
    auto time_begin_cls_asy = std::chrono::high_resolution_clock::now();
    // Cluster with bruteforce method
    std::future<std::map<int, std::vector<std::string>>> clusering_out = std::async(std::launch::async, ClusteringBruteForce, table, rows, columns, dx);
    clustered_groups = clusering_out.get();
    std::cout << "INFORMATIVE MESSAGE: Clustering steps finished!\n" << std::endl;
    // Bruteforce clustering runtime
    auto time_end_cls_asy = std::chrono::high_resolution_clock::now() - time_begin_cls_asy;

    // Measuring time for writing data begin
    auto time_begin_wrt = std::chrono::high_resolution_clock::now();
    // Write data to output file
    write_to_file(table, rows, columns, clustered_groups);
    std::cout << "INFORMATIVE MESSAGE: Output file created!\n" << std::endl;
    // Data writing runtime
    auto time_end_wrt = std::chrono::high_resolution_clock::now() - time_begin_wrt;

    std::cout << "INFORMATIVE MESSAGE: Clustering is completed!\n" << std::endl;

    // Time information
    std::cout << "INFORMATION ABOUT RUNNING TIME" << std::endl;
    std::cout << "Time needed for generating data table: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end_gen).count() << " usec\n";
    std::cout << "Time needed for native bruteforce clustering: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end_cls_nat).count() << " usec\n";
    std::cout << "Time needed for threaded bruteforce clustering: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end_cls_asy).count() << " usec\n";
    std::cout << "Time needed for writing data into file: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end_wrt).count() << " usec\n";
    std::cout << "Total native runtime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end_gen + time_end_cls_nat + time_end_wrt).count() << " usec\n";
    std::cout << "Total threaded runtime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end_gen + time_end_cls_asy + time_end_wrt).count() << " usec\n";
}