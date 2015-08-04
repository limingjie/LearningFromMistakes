//
// Implicit Conversion
//   - Conversion Constructor
//   - Assignment Operator with lvalue and rvalue
//
// Aug 4, 2015 - by Mingjie Li (limingjie@outlook.com)
// https://github.com/limingjie/LearningFromMistakes
//
#include <iostream>
#include <string>

class Fruit
{
private:
    std::string _name;

public:
    Fruit(const std::string& name)
    {
        std::cout << "Implicit Type Conversion.\n";
        _name = name;
    }

    Fruit& operator= (const std::string& name)
    {
        std::cout << "Assignment Operator (const std::string& name).\n";
        _name = name;

        return *this;
    }

    Fruit& operator= (std::string&& name)
    {
        std::cout << "Assignment Operator (std::string&& name).\n";
        _name = std::move(name);

        return *this;
    }
};

int main()
{
    Fruit p = std::string("Alligator pear");

    p = std::string("Avocado");

    std::string avocado("Avocado");
    p = avocado;

    std::cout.flush();

    return 0;
}
