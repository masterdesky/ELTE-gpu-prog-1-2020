#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

void TestFunction(std::map<int, std::vector<int>> TestMap)
{
    std::map<int, std::vector<int>>::iterator MapIterator;
    std::map<int, std::vector<int>>::iterator MapIteratorTemp;

    int Temp;

    for(auto const& [key, val] : TestMap)
    {
        if(key == 7)
        {
            TestMap.erase(7);
            Temp = 7;
        }
        std::cout << MapIterator->first << std::endl;
    }

    for(MapIterator = TestMap[6]; MapIterator != TestMap.end(); ++MapIterator)
    {
        std::cout << MapIterator->first << std::endl;
    }
}

int main()
{
    std::map<int, std::vector<int>> TestMap;
    TestMap[1] = {1,2,3};
    TestMap[2] = {1,2,3};
    TestMap[3] = {1,2,3};
    TestMap[4] = {1,2,3};
    TestMap[5] = {1,2,3};
    TestMap[6] = {1,2,3};
    TestMap[7] = {1,2,3};
    TestMap[8] = {1,2,3};
    TestMap[9] = {1,2,3};
    TestMap[10] = {1,2,3};
    TestMap[11] = {1,2,3};

    TestFunction(TestMap);
}