#include <typeinfo>
#include <iostream>
#include <string>
#include <cassert>

class Fraction {
    public:
        // constructors
        Fraction();
        Fraction(int x, int y);
        Fraction(int x);
        Fraction(std::string args);
        
        // operator overloading
        Fraction operator+(Fraction f);
        Fraction operator-(Fraction f);
        Fraction operator*(Fraction f);
        Fraction operator/(Fraction f);
        void operator=(Fraction f);
        bool operator==(Fraction f);
        bool operator!=(Fraction f);
        bool operator<(Fraction f);

        Fraction add(int x);
        Fraction subtract(int x);
        Fraction multiply(int x);
        Fraction divide(int x);
        
        float value();

        void string2fraction(std::string args);
        std::string fraction2string();
        void print();
        int get_numerator();
        void set_numerator(int x);
        int get_denominator();
        void set_denominator(int x);
        void reduce(); 
    
    private:
        int numerator;
        int denominator;      
        int gcd(int x, int y);
};

Fraction::Fraction() {
    this->numerator = 0;
    this->denominator = 1;
}

Fraction::Fraction(int x) {
    this->numerator = x;
    this->denominator = 1;
}
Fraction::Fraction(int x, int y) {
    this->numerator = x;
    this->denominator = y;
}

Fraction::Fraction(std::string args) {
    this->string2fraction(args);
}

Fraction Fraction::operator+(Fraction f){
    Fraction temp;
    temp.set_numerator(this->numerator * f.get_denominator() + this->denominator * f.get_numerator());
    temp.set_denominator(this->denominator * f.get_denominator());
    temp.reduce();
    return temp;
}

Fraction Fraction::operator-(Fraction f){
    Fraction temp;
    temp.set_numerator(this->numerator * f.get_denominator() - this->denominator * f.get_numerator());
    temp.set_denominator(this->denominator * f.get_denominator());
    temp.reduce();
    return temp;
}

Fraction Fraction::operator*(Fraction f){
    Fraction temp;
    temp.set_numerator(this->numerator * f.get_numerator());
    temp.set_denominator(this->denominator * f.get_denominator());
    temp.reduce();
    return temp;
}

Fraction Fraction::operator/(Fraction f){
    Fraction temp;
    temp.set_numerator(this->numerator * f.get_denominator());
    temp.set_denominator(this->denominator * f.get_numerator());
    temp.reduce();
    return temp;
}

void Fraction::operator=(Fraction f) {
    this->numerator = f.get_numerator();
    this->denominator = f.get_denominator();
}

bool Fraction::operator==(Fraction f) {
    return (this->value() == f.value());
}

bool Fraction::operator!=(Fraction f) {
    return (this->value() != f.value());
}

bool Fraction::operator<(Fraction f) {
    return (this->value() < f.value());
}

Fraction Fraction::add(int x) {
    Fraction temp;
    temp.set_numerator(this->numerator + this->denominator * x);
    temp.set_denominator(this->denominator);
    temp.reduce();
    return temp;
}

Fraction Fraction::subtract(int x) {
    Fraction temp;
    temp.set_numerator(this->numerator - this->denominator * x);
    temp.set_denominator(this->denominator);
    temp.reduce();
    return temp;
}

Fraction Fraction::multiply(int x) {
    Fraction temp;
    temp.set_numerator(this->numerator * x);
    temp.set_denominator(this->denominator);
    temp.reduce();
    return temp;
}

Fraction Fraction::divide(int x) {
    Fraction temp;
    temp.set_numerator(this->numerator);
    temp.set_denominator(this->denominator * x);
    temp.reduce();
    return temp;
}

float Fraction::value() {
    return (float)this->numerator/this->denominator;
}

void Fraction::string2fraction(std::string args) {
    size_t start = args.find("/");
    if(start != std::string::npos) { // 1/2 type
        this->numerator = std::stoi(args.substr(0, start));
        this->denominator = std::stoi(args.substr(start+1));
        assert(this->denominator != 0);
        this->reduce();
    } else { // 1 type
        this->numerator = std::stoi(args);
        this->denominator = 1;
    }
}

std::string Fraction::fraction2string() {
    std::string temp = std::to_string(this->numerator);
    if(this->denominator != 1) {
        temp += "/" + std::to_string(this->denominator);
    }
    
    return temp;     
}

void Fraction::print() {
    std::cout << this->fraction2string();
}

int Fraction::get_numerator() {
    return this->numerator;
}

void Fraction::set_numerator(int x) {
    this->numerator = x;
} 

int Fraction::get_denominator() {
    return this->denominator;
}

void Fraction::set_denominator(int x) {
    this->denominator = x;
} 

void Fraction::reduce() {
    int hcf = this->gcd(this->numerator, this->denominator);
    this->numerator /= hcf;
    this->denominator /= hcf;

    // make sure that denominator is always positive
    if(this->denominator < 0) {
        this->numerator /= -1;
        this->denominator /= -1;
    }
}

int Fraction::gcd(int x, int y)
{
    int r = x % y;
    while(r != 0){
        x = y;
        y = r;
        r = x % y;
    }
    return y;
}

void check_fraction(std::string argv1, std::string argv2) {
    Fraction f1(argv1), f2(argv2);
    Fraction sum = f1 + f2;
    Fraction diff = f1 - f2;
    Fraction prod = f1 * f2;
    Fraction div = f1 / f2;
    std::cout << f1.fraction2string() << " + " << f2.fraction2string() << " = " << sum.fraction2string() << std::endl;
    std::cout << f1.fraction2string() << " - " << f2.fraction2string() << " = " << diff.fraction2string() << std::endl;
    std::cout << f1.fraction2string() << " * " << f2.fraction2string() << " = " << prod.fraction2string() << std::endl;
    std::cout << f1.fraction2string() << " / " << f2.fraction2string() << " = " << div.fraction2string() << std::endl;
    f1 = f2;
    std::cout << "After performindg f1 = f2, f1 = " << f1.fraction2string() << std::endl;
}    
