#include <iostream>
#include <cstring>

class Chicken
{
private:
    int age = 24;
    char *name = nullptr;

public:

    Chicken() = default; 

    Chicken(int _age) : age(_age) {}

    Chicken(int _age, const char *_name) : age(_age)
    {
        if (_name != nullptr)
        {
            int len = strlen(_name) + 1;
            name = new char[len];
            std::strcpy(name, _name); 
        }
    }

    Chicken(const Chicken &other)
    {
        age = other.age;
        if (other.name != nullptr)
        {
            int len = strlen(other.name) + 1;
            name = new char[len];
            std::strcpy(name, other.name);
        }
        else
        {
            name = nullptr;
        }
    }

    Chicken &operator=(const Chicken &other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (name != nullptr)
        {
            delete[] name;
        }

        age = other.age;

        if (other.name != nullptr)
        {
            int len = strlen(other.name) + 1;
            name = new char[len];
            std::strcpy(name, other.name); 
        }
        else
        {
            name = nullptr;
        }

        return *this;
    }

    ~Chicken()
    {
        delete[] name;
    }

    void setAge(int _age)
    {
        age = _age;
    }

    void setName(const char *_name)
    {
        
        if (name != nullptr)
        {
            delete[] name;
        }

        if (_name != nullptr)
        {
            int len = strlen(_name) + 1;
            name = new char[len];
            std::strcpy(name, _name); 
        }
        else
        {
            name = nullptr; 
        }
    }

    const char *getName() const
    {
        return name;
    }

    const int &getAge() const
    {
        return age;
    }
};

int main(){
	auto print = [](const Chicken &c){
		std::cout << "Hi everyone! My name is " << c.getName()
		          << ", I am " << c.getAge() << " years old." << std::endl;
	};
	Chicken c(24, "Kunkun");
	print(c);
	
	Chicken d;
	d=c;
	print(d);
	
	Chicken a=c;
	print(a);
	
	c.setName("Xukun Cai");
	print(c);
	print(a);
	print(d);
	
	Chicken b;
	b = d = c;
	print(b);
	print(d);
	return 0;
} 
