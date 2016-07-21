#include <iostream>
#include <string>
#include <cmath>
#include "fraction.cpp"

class Monomial
{
    public:
        // constructors
        Monomial();
        Monomial(std::string args);
        
        // operator overloading
        Monomial operator+(Monomial m);
        Monomial operator-(Monomial m);
        Monomial operator*(Monomial m);
        Monomial operator/(Monomial m);
        void operator=(Monomial m);

        template <class T> float evaluate(T x);

        Monomial derivative();
        Monomial indefinite_integral();
        template <class T> float definite_integral(T x1, T x2);
 
        void string2monomial(std::string args);
        std::string monomial2string();
        void print();
        Fraction get_coefficient();
        std::string get_base();
        Fraction get_power();
        void set_coefficient(Fraction f);
        void set_base(std::string base);
        void set_power(Fraction f);
        void is_like_monomial(Monomial m1, Monomial m2);
 

    private:
        Fraction coefficient;
        std::string base;
        Fraction power;     
}; 

Monomial::Monomial() {
    this->base = "x";
}

Monomial::Monomial(std::string args) {
    this->string2monomial(args); 
}

Monomial Monomial::operator+(Monomial m) {
    is_like_monomial(*this, m);

    Monomial temp;
    
    temp.set_coefficient(this->coefficient + m.get_coefficient());
    temp.set_base(this->base);
    
    Fraction zero;
    if(temp.get_coefficient() == zero) {
        temp.set_power(zero);
    } else {
        temp.set_power(this->power);
    }

    return temp;
}

Monomial Monomial::operator-(Monomial m) {
    is_like_monomial(*this, m);

    Monomial temp;

    temp.set_coefficient(this->coefficient - m.get_coefficient());
    temp.set_base(this->base);

    Fraction zero;
    if(temp.get_coefficient() == zero) {
        temp.set_power(zero);
    } else {
        temp.set_power(this->power);
    }
    
    return temp;
}

Monomial Monomial::operator*(Monomial m) {
    if(this->base != m.get_base()) {
        std::cout << "Monomials with different bases not supported for multiplication." << std::endl;
        exit(0);
    }

    Monomial temp;
    temp.set_coefficient(this->coefficient * m.get_coefficient());
    temp.set_base(this->base);

    Fraction zero;
    if(temp.get_coefficient() == zero) {
        temp.set_power(zero);
    } else {
        temp.set_power(this->power + m.get_power());
    }

    return temp;
}

Monomial Monomial::operator/(Monomial m) {
    if(this->base != m.get_base()) {
        std::cout << "Monomials with different bases not supported for division." << std::endl;
        exit(0);
    }
    
    Fraction zero;
    if(m.get_coefficient() == zero) {
        std::cout << "Division by zero not allowed." << std::endl;
        exit(0);
    }

    Monomial temp;
    temp.set_coefficient(this->coefficient / m.get_coefficient());
    temp.set_base(this->base);
    temp.set_power(this->power - m.get_power());
    return temp;
}

void Monomial::operator=(Monomial m) {
    this->set_coefficient(m.get_coefficient());
    this->set_base(m.get_base());
    this->set_power(m.get_power());
}

template <class T> float Monomial::evaluate(T x) {
    float c = this->coefficient.value();
    float p = this->power.value();
    float v = (float)pow((double)x, (double) p);
    return c*v;     
} 

Monomial Monomial::derivative() {
    Monomial temp;

    // if m is not a constant
    if((int) this->power.value() != 0) {
        temp.set_coefficient(this->coefficient * this->power);
        temp.set_base(this->base);
        temp.set_power(this->power.subtract(1)); 
    }

    return temp;
}


Monomial Monomial::indefinite_integral() {
    Monomial temp;

    // if m is x^-1 type
    if(this->power.value() == -1) {
        std::cout << "Integration is out of scope of the polynomial range." << std::endl;
        exit(0);
    } else {
        temp.set_coefficient(this->coefficient / this->power.add(1));
        temp.set_base(this->base);
        if(temp.get_coefficient().value() == 0) {
            Fraction zero;
            temp.set_power(zero);
        } else {
            temp.set_power(this->power.add(1));
        }
    }

    return temp;
}

template <class T> float Monomial::definite_integral(T x1, T x2) {
    Monomial t = this->indefinite_integral();
    float X1 = t.evaluate(x1);
    float X2 = t.evaluate(x2);
    return X2-X1;
}

void Monomial::string2monomial(std::string args) {
    size_t start = args.find("*");
    size_t end = args.find("^");
    std::string coefficient_string;
    std::string power_string;
 
    if(start != std::string::npos && end != std::string::npos) { // 2*x^3 type or 0*x^3 type
        //printf("1\n");
        coefficient_string = args.substr(0, start); 
        base = args.substr(start+1, end - start - 1);
        power_string = (coefficient_string == "0") ? "0" : args.substr(end+1);
   } else if (start == std::string::npos && end != std::string::npos) { // x^3 type or -x^3 type
        //printf("2\n");
        if(args[0] == '-') {
            coefficient_string = "-1";
            base = args.substr(1, end-1);
        } else {
            coefficient_string = "1";
            base = args.substr(0, end);
        }
        power_string = args.substr(end+1);
    } else if (start != std::string::npos && end == std::string::npos) { // 2*x type or 0*x
        //printf("3\n");
        coefficient_string = args.substr(0, start);
        base = args.substr(start+1);
        power_string = (coefficient_string == "0") ? "0" : "1";
    } else { // x type or or +x type or -x type or 1 type
        //printf("4\n");
        if((args[0] >= 'A' && args[0] <= 'Z') || (args[0] >= 'a' && args[0] <= 'z')){
            coefficient_string = "1";
            base = args;
            power_string = "1";
        } else if (args[0] == '-' && ((args[1] >= 'A' && args[1] <= 'Z') || (args[1] >= 'a' && args[1] <= 'z'))) {
            coefficient_string = "-1";
            base = args.substr(1);
            power_string = "1";
        } else if (args[0] == '+' && ((args[1] >= 'A' && args[1] <= 'Z') || (args[1] >= 'a' && args[1] <= 'z'))) {
            coefficient_string = "1";
            base = args.substr(1);
            power_string = "1";
        }else {
            coefficient_string = args;
            base = "x";
            power_string = "0";
        }
    }
 
    //std::cout << "coefficient:" << coefficient_string << "\tbase:" << base << "\tpower:" << power_string << std::endl;
    Fraction t1(coefficient_string), t2(power_string);
    this->coefficient = t1;
    this->power = t2;    
}

std::string Monomial::monomial2string() {
    std::string temp = "";

    // rule for constants only
    if((int)this->power.value() == 0) {
        temp += this->coefficient.fraction2string();
        return temp;
    }

    // rule for coefficient to print
    if(abs((int)this->coefficient.value()) != 1) {
        temp += this->coefficient.fraction2string();
    } 
    if((int)this->coefficient.value() == -1) {
        temp += "-";
    }

    // rule for * to print
    if(abs((int)this->coefficient.value()) != 1) {
        temp += "*";
    }

    // rule for variable to print
    if((int)this->power.value() != 0) {    
        temp +=  this->base;
    }

    // rule for power to print
    if((int)this->power.value() != 0 && (int)this->power.value() != 1) {
        temp += "^";
        temp += this->power.fraction2string();
    }

    return temp;
}

void Monomial::print() {
    std::cout << this->monomial2string();
}

Fraction Monomial::get_coefficient() {
    return this->coefficient;
}

std::string Monomial::get_base() {
    return this->base;
}

Fraction Monomial::get_power() {
    return this->power;
}

void Monomial::set_coefficient(Fraction f) {
    this->coefficient = f;
}

void Monomial::set_base(std::string base) {
    this->base = base;
}

void Monomial::set_power(Fraction f) {
    this->power = f;
}

void Monomial::is_like_monomial(Monomial m1, Monomial m2) {
    // ensure the variables are the same
    if(m1.get_base() != m2.get_base()) {
        std::cout << "Monomials with different bases are unlike monomials. Not fit for addition/subtraction." << std::endl;
        exit(0);
    }    
    
    // ensure that monomials have same power
    if(m1.get_power() != m2.get_power()) {
        std::cout << "Monomials with different powers are unlike monomials. Not fit for addition/subtraction." << std::endl;
        exit(0);
    }
    
}

void check_monomial(std::string argv1, std::string argv2) {
    Monomial m1(argv1), m2(argv2);
    Monomial sum = m1+m2;
    Monomial diff = m1-m2;
    Monomial prod = m1 * m2;
    Monomial div = m1 / m2;
    Monomial dc = m1.derivative();
    Monomial ii = m1.indefinite_integral();
    
    std::cout << m1.monomial2string() << " + " << m2.monomial2string() << " = " << sum.monomial2string() << std::endl;
    std::cout << m1.monomial2string() << " - " << m2.monomial2string() << " = " << diff.monomial2string() << std::endl;
    std::cout << m1.monomial2string() << " * " << m2.monomial2string() << " = " << prod.monomial2string() << std::endl;
    std::cout << m1.monomial2string() << " / " << m2.monomial2string() << " = " << div.monomial2string() << std::endl;

    std::cout << m1.monomial2string() << " (5) = " << m1.evaluate(5) << std::endl;
    std::cout << m1.monomial2string() << " (5.0) = " << m1.evaluate(5.0) << std::endl;
    std::cout << "Derivative of " << m1.monomial2string() << " is " << dc.monomial2string() << std::endl;
    std::cout << "Integral of " << m1.monomial2string() << " is " << ii.monomial2string() << std::endl;
    std::cout << "Integration of  = " << m1.monomial2string() << " from 0 to 1 is " << m1.definite_integral(0,1) << std::endl;
}

