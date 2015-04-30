//------------------------------------------------------------------------------
// UnexpectedString.cpp
//   - To describe temporary object lifetime
//   - To demonstrate ctor/copy ctor/dtor/implicit conversion
//
// Date: April 30, 2015
//------------------------------------------------------------------------------
// PROBLEM - Pointer invalidated after temporary object destroyed.
//   std::string getName() {};             // function returns temporary object
//   const char *str = getName().c_str();
//                                         // temporary object destroyed
//   someFunc(str);                        // <-- str points to unknown content
//------------------------------------------------------------------------------
// $ g++ -Wall -g -o UnexpectedString.exe UnexpectedString.cpp
// $ ./UnexpectedString.exe
// ### StringWrapper Object[0] c_str() = 0x4ab3b8. Constructor()
// ### StringWrapper Object[1] c_str() = 0x6e76c8. Constructor(str)
// ### StringWrapper Object[0] c_str() = 0x6e7698. Copy Assign Operator
// ### StringWrapper Object[1] c_str() = 0x6e7708. Destructor
// ### StringWrapper Object[2] c_str() = 0x6e76c8. Copy Constructor
// oname.getName() = "Valid Name+++"
// ### StringWrapper Object[2] c_str() = 0x6e7708. Destructor
// ### StringWrapper Object[3] c_str() = 0x6e76c8. Copy Constructor
// ### StringWrapper Object[3] c_str() = 0x6e76c8. c_str()
// oname.getName().c_str() = "Valid Name+++"
// ### StringWrapper Object[3] c_str() = 0x6e7708. Destructor
// ### StringWrapper Object[4] c_str() = 0x6e76c8. Copy Constructor
// ### StringWrapper Object[4] c_str() = 0x6e76c8. c_str()
// ### StringWrapper Object[4] c_str() = 0x6e7b08. Destructor
// str = "Destructor"   <<< the value should be garbage here.
// ### StringWrapper Object[0] c_str() = 0x6e7b08. Destructor
//------------------------------------------------------------------------------

#include <string>
#include <iostream>

class StringWrapper
{
private:
    static size_t _count;
    size_t _cnt;

    std::string _str;

    friend std::ostream& operator<<(std::ostream &output, const StringWrapper &sw);

    void print(std::string where) const
    {
        std::cout << "### StringWrapper Object[" << _cnt
                  << "] c_str() = " << (void *)_str.c_str()
                  << ". " << where << '\n';
    }

public:
    // Constructor
    StringWrapper()
    {
        _cnt = _count++;
        print("Constructor()");
    }

    // Constructor for implicit type conversion
    StringWrapper(const char *str)
    {
        _cnt = _count++;
        print("Constructor for implicit type conversion");
        _str = std::string(str) + "+"; // make sure copy
    }

    // Copy constructor
    StringWrapper(const StringWrapper &sw)
    {
        _cnt = _count++;
        print("Copy Constructor");
        _str = sw._str + "+"; // make sure copy
    }

    // Destructor
    ~StringWrapper()
    {
        print("Destructor");
        _str = "[!!!Error: Destructed Memory!!!]";
    }

    // Copy assign operator
    StringWrapper& operator= (const StringWrapper &other)
    {
        if (this != &other)
        {
            print("Copy Assign Operator");
            _str = other._str + "+"; // make sure copy
        }

        return *this;
    }

    // Return data
    const char *c_str()
    {
        print("c_str()");
        return _str.c_str();
    }
};

std::ostream& operator<< (std::ostream &output, const StringWrapper &sw)
{
    output << sw._str;
    return output;
}

size_t StringWrapper::_count = 0;

class OnlyName
{
private:
    StringWrapper _name;

public:
    // Constructor for implicit type conversion
    OnlyName(const char *name)
    {
        std::cout << "### OnlyName Constructor for implicit type conversion\n";
        _name = name;
    }

    void setName(StringWrapper name)
    {
        _name = name;
    }

    // Return value, that means a temporary copy.
    //--------------------------------------------------------------------------
    // Return const std::string could avoid invoking c_str() as c_str is not
    // defined as const, to avoid c_str() changes the const std::string.
    //   const char *c_str();      // this is not const function
    //   const char *c_str() const // const function
    //--------------------------------------------------------------------------
    // gcc 4.9.1 compilation error for following code:
    //   const StringWrapper getName();
    //   const char *str = oname.getName().c_str();
    // error: passing 'const StringWrapper' as 'this' argument of
    // 'const char* StringWrapper::c_str()' discards qualifiers [-fpermissive]
    //--------------------------------------------------------------------------
    StringWrapper getName()
    {
        return _name;
    }
};

int main()
{
    OnlyName oname = "Initial Name";
    oname.setName("Valid Name");

    // Print _str of a temporary StringWrapper object.
    std::cout << "oname.getName() >>> \"" << oname.getName() << "\"\n";

    // Print _str.c_str() of a temporary StringWrapper object.
    std::cout << "oname.getName().c_str() >>> \""
              << oname.getName().c_str() << "\"\n";

    // Assign _str.c_str() to str
    //--------------------------------------------------------------------------
    // THE PROBLEM IS:
    // - oname.getName() returns a temporary object,
    // - then c_str() return the address to the temporary object's data,
    // - the object destructed after assigning the the address to variable str,
    // - so after this statement, the str is pointing to unknown content.
    //--------------------------------------------------------------------------
    // Avoid this by copying the string, and invoking c_str() when necessary.
    //   std::string str = oname.getName();
    //   func(str.c_str());
    //--------------------------------------------------------------------------
    const char *str = oname.getName().c_str();

    // Output str. The output should be garbage.
    std::cout << "str >>> \"" << str
              << "\" <<< the value is uncertain here.\n";

    std::cout.flush();
    //std::cin.get();

    return 0;
}
