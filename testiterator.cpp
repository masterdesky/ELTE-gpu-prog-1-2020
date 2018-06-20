#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

void TestFunction(std::map<int, std::vector<int>> TestMap)
{
    std::map<int, std::vector<int>>::iterator MapIterator;
    std::vector<int>::iterator VectorIterator;

    int TempStrorageForIndeces;

    for(MapIterator = TestMap.begin(); MapIterator != TestMap.end(); ++MapIterator)
    {
        if(MapIterator->first == 7)
        {
            TestMap.erase(MapIterator->first);
            TempStrorageForIndeces = MapIterator->first;
        }
        std::cout << MapIterator->first << std::endl;
    }

    for(MapIterator = TestMap.begin(); MapIterator != TestMap.end(); ++MapIterator)
    {
        int TempIndexFind = TempStrorageForIndeces + 1;
        if(MapIterator->first == TempIndexFind)
        {
            TestMap[TempStrorageForIndeces] = TestMap[TempIndexFind];
            TestMap.erase(TempIndexFind);
            ++TempStrorageForIndeces;
        }
    }

    for(MapIterator = TestMap.begin(); MapIterator != TestMap.end(); ++MapIterator)
    {
        std::cout << MapIterator->first << "{";
        for(VectorIterator = MapIterator->second.begin(); VectorIterator != MapIterator->second.end(); ++VectorIterator)
        {
            std::cout << *VectorIterator << ",";
        }
        std::cout << "}" << std::endl;
    }
}

int main()
{
    std::map<int, std::vector<int>> TestMap;
    TestMap[1] = {1,1,1};
    TestMap[2] = {2,2,2};
    TestMap[3] = {3,3,3};
    TestMap[4] = {4,4,4};
    TestMap[5] = {5,5,5};
    TestMap[6] = {6,6,6};
    TestMap[7] = {7,7,7};
    TestMap[8] = {8,8,8};
    TestMap[9] = {9,9,9};
    TestMap[10] = {10,10,10};
    TestMap[11] = {11,11,11};

    TestFunction(TestMap);
}