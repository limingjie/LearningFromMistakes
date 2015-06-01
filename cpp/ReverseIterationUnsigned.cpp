//
// Reverse Iteration on Unsigned Type Causes Infinite Loop
//
// June 1, 2015 - by Mingjie Li (limingjie@outlook.com)
// https://github.com/limingjie/libcsvmm
//
// I ran into this several times, normally I use a -1 to terminate the loop.
// After some search, I found there are some other solutions, cool solutions ;)
// 1. for (size_t i = n; i --> 0 ;)
// 2. for (size_t i = 9; i-- != 0; )
// 3. for (size_t i = 9; i != -1; i--)
// 4. for (size_t i = 9; i != UINT_MAX; i--)
// http://stackoverflow.com/questions/3623263/reverse-iteration-with-an-unsigned-loop-variable
// http://stackoverflow.com/questions/9044059/unsigned-integers-in-c-for-loops
//

#include <iostream>
#include <vector>

//
// Best One --> ;)
//
// Note the loop compares i with 0, but uses i - 1 for actual loop.
//
void LoopUnsigned(const std::vector<int> &v)
{
    std::cout << "=== LoopUnsigned()" << std::endl;
    for (size_t i = v.size(); i --> 0 ;)
    {
        std::cout << "v[" << i << "] = " << v[i] << std::endl;
    }
}

//
// Use Iterator
//
// Note the loop compares i with 0, but uses i - 1 for actual loop.
//
void LoopUnsignedByIterator(const std::vector<int> &v)
{
    std::cout << "=== LoopUnsignedByIterator()" << std::endl;
    for (auto it = v.crbegin(); it != v.crend(); ++it)
    {
        std::cout << "v[" << v.crend() - it - 1 << "] = " << *it << std::endl;
    }
}

//
// Infinite Loop
//
// $ ./ReverseIterationUnsigned.exe
// v[9] = 9
// v[8] = 8
// v[7] = 7
// v[6] = 6
// v[5] = 5
// v[4] = 4
// v[3] = 3
// v[2] = 2
// v[1] = 1
// v[0] = 0
// v[18446744073709551615] = 134512981
// v[18446744073709551614] = 373187824
// ...
//
void LoopUnsignedInfinite(const std::vector<int> &v)
{
    size_t count = 0;

    std::cout << "=== LoopUnsignedInfinite()" << std::endl;
    for (size_t i = v.size() - 1; i >= 0; --i)
    {
        std::cout << "v[" << i << "] = " << v[i] << std::endl;

        // Protection against infinitely loop.
        if (++count >= 20)
        {
            std::cout << "Press Ctrl+C to exit. Enter to continue..." << std::endl;
            std::cin.get();
            count = 0;
        }
    }
}

int main()
{
    std::vector<int> v {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    LoopUnsignedByIterator(v);
    LoopUnsigned(v);
    LoopUnsignedInfinite(v);

    return 0;
}
