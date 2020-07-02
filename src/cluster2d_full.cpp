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
            
            if(Choosedx.compare("Y") == 0
               ||
               Choosedx.compare("y") == 0
               ||
               Choosedx.compare("Yes") == 0
               ||
               Choosedx.compare("yes") == 0
               ||
               Choosedx.compare("YEs") == 0
               ||
               Choosedx.compare("yEs") == 0
               ||
               Choosedx.compare("yeS") == 0
               ||
               Choosedx.compare("YeS") == 0
               ||
               Choosedx.compare("yES") == 0)
            {
                std::cout << "\n\n";
                break;
            }
            else if(Choosedx.compare("N") == 0
                    ||
                    Choosedx.compare("n") == 0
                    ||
                    Choosedx.compare("No") == 0
                    ||
                    Choosedx.compare("no") == 0
                    ||
                    Choosedx.compare("nO") == 0)
            {
                std::cout << "\n\n" << std::endl;
            } // Do nothing; go to the start of the loop
        }

        else if(dx < max_element && dx >= 0)
        {
            std::cout << "\n\n";
            break;
        }

        else
        {
            std::cout << "ERROR: Not permitted input!\n";
            std::cout << "       The value of \'dx\' should be a non-negative real number! Please try again!" << std::endl;
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
std::vector<std::pair<std::pair<int, int>, double>> gen_2d_table(int rows, int columns, double max_element)
{
    std::pair<int, int> pos_idx;
    std::vector<std::pair<std::pair<int, int>, double>> table;

    //Randomize
    std::random_device Randomized;
    std::mt19937 Generate(Randomized());
    std::uniform_real_distribution<> Distribution(0, max_element);

    for(int i = 1; i <= rows; i++)
    {
        for(int j = 1; j <= columns; j++)
        {

            // Create a pair which indicates the position of the element in the 2D matrix
            pos_idx = {i, j};

            //Generate a random double
            double random_element = Distribution(Generate);

            // Create a pair for the current element in the table:
            // 1. First element is the indeces above
            // 2. Second is the actual number in that particular location
            table.push_back(std::make_pair(pos_idx, random_element));
        }
    }

    return(table);
}


// Test
// Print out the generated 2D data table's elements
void PrintElements(std::vector<std::pair<std::pair<int, int>, double>> table, int rows, int columns)
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
std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> flood_fill_step(std::vector<std::pair<std::pair<int, int>, double>> table,
                                                                                              std::vector<std::pair<int, int>> cluster_elements,
                                                                                              std::pair<std::pair<int, int>, double> previous,
                                                                                              std::vector<std::pair<int, int>> already_seen,
                                                                                              int rows, int columns, int i, int j, double dx)
{
    // If index is out of bounds, return
    if(i < 0 || j < 0 || i >= rows || j >= columns)
    {
        return(std::make_pair(cluster_elements, already_seen));
    }

    // This element would be compared to the first one in the previous position
    std::pair<std::pair<int, int>, double> current = table[i * columns + j];
    
    // If tile has been already observed, skip it
    if(std::find(already_seen.begin(), already_seen.end(), current.first) != already_seen.end())
    {
        return(std::make_pair(cluster_elements, already_seen));
    }
    // Adding tile to already seen tiles
    already_seen.push_back(current.first);

    // If the neighbouring condition is satisfied, add it to the cluster elements
    if(abs(previous.second - current.second) < dx)
    {
        cluster_elements.push_back(current.first);
    }
    else
    {
        return(std::make_pair(cluster_elements, already_seen));
    }
    
    // Change the current to the previous
    previous = current;

    std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> params;

    // Move into the four cardinal directions
    params = flood_fill_step(table, cluster_elements,
                             previous, already_seen,
                             rows, columns, i+1, j, dx);
    cluster_elements = std::get<0>(params);
    already_seen = std::get<1>(params);

    params = flood_fill_step(table, cluster_elements,
                             previous, already_seen,
                             rows, columns, i-1, j, dx);
    cluster_elements = std::get<0>(params);
    already_seen = std::get<1>(params);

    params = flood_fill_step(table, cluster_elements,
                             previous, already_seen,
                             rows, columns, i, j+1, dx);
    cluster_elements = std::get<0>(params);
    already_seen = std::get<1>(params);

    params = flood_fill_step(table, cluster_elements,
                             previous, already_seen,
                             rows, columns, i, j-1, dx);
    cluster_elements = std::get<0>(params);
    already_seen = std::get<1>(params);

    return(std::make_pair(cluster_elements, already_seen));
}

std::vector<std::pair<int, int>> flood_fill_agg(std::vector<std::pair<std::pair<int, int>, double>> table,
                                                int rows, int columns, int i, int j, double dx)
{
    // Placeholder for the indeces of tiles in the current cluster
    std::vector<std::pair<int, int>> cluster_elements;

    // Placeholder for the indeces of already checked tiles
    std::vector<std::pair<int, int>> already_seen;

    // Initialize the "previous" element as the starting one
    std::pair<std::pair<int, int>, double> previous = table[i * columns + j];

    auto params = flood_fill_step(table, cluster_elements,
                                  previous, already_seen,
                                  rows, columns, i, j, dx);
    cluster_elements = std::get<0>(params);
    //already_seen = std::get<1>(params);

    return(cluster_elements);
}

// Flood fill started from every point of the original matrix
// Put these individual starts to different async threads and accumulate an array of futures
std::vector<std::vector<std::pair<int, int>>> flood_fill(std::vector<std::pair<std::pair<int, int>, double>> table,
                                                                      int rows, int columns, double dx)
{

    // Placeholder for generated clusters (Still contains duplicates)
    std::vector<std::vector<std::pair<int, int>>> clusters;

    // Individual futures
    std::future<std::vector<std::pair<int, int>>> cluster_elements_future;

    // Vector of futures of flood fills
    std::vector<std::future<std::vector<std::pair<int, int>>>> flood_fill_futures;

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            flood_fill_futures.push_back(std::async(std::launch::async, flood_fill_agg,
                                                    table,
                                                    rows, columns, i, j, dx));
        }
    }

    for(std::vector<int>::size_type i = 0; i != flood_fill_futures.size(); i++)
    {
        clusters.push_back(flood_fill_futures[i].get());
    }

    return(clusters);
}


// ####### 3. WRITE DATA INTO FILE #######

void write_to_file(std::vector<std::pair<std::pair<int, int>, double>> table, int rows, int columns,
                   std::vector<std::vector<std::pair<int, int>>> clusters)
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
    

    // Make a textfile to store output data
    output_file.open("..//output//clustered_data.txt");

    // Write a header for the output file
    output_file << "Indeces of clusters\tElements of clusters" << std::endl;
    // Write the clusters' data into the file
    for(std::vector<int>::size_type i = 0; i != clusters.size(); i++)
    {
        output_file << "cluster_" << i << "\t[";
        for(std::vector<int>::size_type j = 0; j != clusters[i].size(); j++)
        {
            output_file << "[" << clusters[i][j].first << "," << clusters[i][j].second << "],";
        }
        output_file << "]" << std::endl;
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
    std::vector<std::pair<std::pair<int, int>, double>> table = gen_2d_table(rows, columns, max_element);
    std::cout << "INFORMATIVE MESSAGE: The data table has been created!\n" << std::endl;
    // Random 2D matrix genereation runtime
    auto time_end_gen = std::chrono::high_resolution_clock::now() - time_begin_gen;

    // Run test
    //PrintElements(table, rows, columns);

    // Define the clustered map first
    std::vector<std::vector<std::pair<int, int>>> clusters;

    // ASYNC THREADED BRUTEFORCE
    // Measuring time for bruteforce clustering begin
    auto time_begin_cls_asy = std::chrono::high_resolution_clock::now();
    // Cluster with bruteforce method
    clusters = flood_fill(table, rows, columns, dx);
    std::cout << "INFORMATIVE MESSAGE: Clustering steps finished!\n" << std::endl;
    // Bruteforce clustering runtime
    auto time_end_cls_asy = std::chrono::high_resolution_clock::now() - time_begin_cls_asy;

    // Measuring time for writing data begin
    auto time_begin_wrt = std::chrono::high_resolution_clock::now();
    // Write data to output file
    write_to_file(table, rows, columns, clusters);
    std::cout << "INFORMATIVE MESSAGE: Output file created!\n" << std::endl;
    // Data writing runtime
    auto time_end_wrt = std::chrono::high_resolution_clock::now() - time_begin_wrt;

    std::cout << "INFORMATIVE MESSAGE: Clustering is completed!\n" << std::endl;

    // Time information
    std::cout << "INFORMATION ABOUT RUNNING TIME" << std::endl;
    std::cout << "Time needed for generating data table: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end_gen).count() << " usec\n";
    std::cout << "Time needed for threaded bruteforce clustering: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end_cls_asy).count() << " usec\n";
    std::cout << "Time needed for writing data into file: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end_wrt).count() << " usec\n";
    std::cout << "Total threaded runtime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end_gen + time_end_cls_asy + time_end_wrt).count() << " usec\n";
}