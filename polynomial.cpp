#include <iostream>
#include <string>
#include <cstring>
#include "monomial.cpp"

class Polynomial {
    public:
        // constructors
        Polynomial();
        Polynomial(std::string args);
 
        // operator overloading
        Polynomial operator+(Polynomial p);
        Polynomial operator-(Polynomial p);
        Polynomial operator*(Polynomial p);
        Polynomial operator/(Polynomial p);
        void operator=(Polynomial p);

        template <class T> float evaluate(T x);
 
        void add(Polynomial* p);
        void subtract(Polynomial* p);
        void multiply(Polynomial* p);
        void multiply(int x);
        void divide(Polynomial* p);
 
        Polynomial derivative();
        Polynomial indefinite_integral();
        template <class T> float definite_integral(T x1, T x2);
 
        void string2polynomial(std::string args);
        std::string polynomial2string();
        void print();
        void set_monomial(Monomial m);
        Polynomial deep_copy();
        void add_monomial(Monomial m);
        void subtract_monomial(Monomial m);
        void multiply_monomial(Monomial m);
  
    private:
        Monomial m; 
        Polynomial* next = NULL;
};

Polynomial::Polynomial() {
    this->next = NULL;
}

Polynomial::Polynomial(std::string args) {   
    this->string2polynomial(args);
}

Polynomial Polynomial::operator+(Polynomial p) {
    Polynomial* temp = new Polynomial();
    *temp = *this;
    Polynomial* current = &p;
    while(current) {
        temp->add_monomial(current->m);
        current = current->next;
    }    
    
    return *temp;
}

Polynomial Polynomial::operator-(Polynomial p) {
    Polynomial* temp = new Polynomial();
    *temp = *this;
    Polynomial* current = &p;
    while(current) {
        //std::cout << temp->polynomial2string() << " - " << current->m.monomial2string() << " = ";
        temp->subtract_monomial(current->m);
        //std::cout << temp->polynomial2string() << std::endl;
        current = current->next;
    }    
    
    return *temp;
    
}

Polynomial Polynomial::operator*(Polynomial p) {
    Polynomial* current = &p;
    
    Polynomial* sum = new Polynomial();
    *sum = *this;
    Polynomial* t = new Polynomial();
    while(current) {
        *t = *this;
        t->multiply_monomial(current->m);
        *sum = *sum + *t;
        delete(t);
        current = current->next;
    }

    *sum = *sum - *this;
    return *sum;
}

void Polynomial::operator=(Polynomial p) {
    Polynomial temp = p.deep_copy();
    this->m = temp.m;
    this->next = temp.next;
}

Polynomial Polynomial::deep_copy() {
    Polynomial* current = this;
    Polynomial* new_head = NULL;
    Polynomial* new_tail = NULL;

    while(current) {
        Polynomial* t = new Polynomial();
        t->set_monomial(current->m); 
        if(new_head == NULL) {
            new_head = t; 
            new_tail = new_head;            
        } else {
            t->next = new_tail->next;
            new_tail->next = t;
            new_tail = new_tail->next;   
        }
        current = current->next;
    }

    return *new_head;
}

template <class T> float Polynomial::evaluate(T x) {
    float sum = 0.0;
    Polynomial* current = this;
    while(current) {
        sum += current->m.evaluate(x);
        current = current->next;
    }

    return sum;
}

/*
Polynomial Polynomial::derivative() {
    Polynomial* temp = new Polynomial();
    *temp = *this;
    
    Polynomial* current = temp;
    while(current) {

        current->m = current->m.derivative();
        current = current->next;
    }
}

Polynomial Polynomial::indefinite_integral() {
    Polynomial* current = this;
    while(current) {
        current->m.indefinite_integral();
        current = current->next;
    }
}
*/

template <class T> float Polynomial::definite_integral(T x1, T x2) {
    float sum = 0;
    Polynomial* current = this;
    while(current) {
        sum += current->m.definite_integral(x1, x2);
        current = current->next;
    }

    return sum;
}

void Polynomial::string2polynomial(std::string args) {
     //parse the argument to extract monomials
     std::string monomial_string;
     std::string::iterator it, start;
     start = args.begin();
     Polynomial* current = this;
     int f = 0;
     for(it = args.begin(); it < args.end(); it++) {
        if((*it == '+' || *it == '-') && (*(it - 1) != '^' && *(it - 1) != '/')){ // x^-2/3 or 2/-3 type should not be included
            monomial_string = std::string(start, it);
         
            // make sure that monomial_string is not empty
            if(monomial_string == "") {
                start = it;
                continue;
            }
            
            // if the coefficient is 0 then continue
            Monomial t(monomial_string);
            if(t.get_coefficient().value() == 0) {
                start = it;
                continue;
            }

            //std::cout << monomial_string << std::endl;
            if(f == 0) {
                //current->m = new Monomial(monomial_string);
                current->m.string2monomial(monomial_string);
                current->next = NULL;
                f++;
            } else {
                Polynomial* temp = new Polynomial();
                temp->m.string2monomial(monomial_string);
                temp->next = NULL;
                current->next = temp;
                current = current->next;
            }
            start = it;
        }
     }
     monomial_string =  std::string(start, it);
     //std::cout << monomial_string << std::endl;
     if(f == 0) {
        current->m.string2monomial(monomial_string);
        current->next = NULL;
        f++;
     } else {
        Polynomial* temp = new Polynomial();
        temp->m.string2monomial(monomial_string);
        temp->next = NULL;
        current->next = temp;
        current = current->next;
     }
  
}

std::string Polynomial::polynomial2string() {
    std::string temp = "";

    Polynomial* current = this;
    int f = 0;
    while(current) {
        float sign = current->m.get_coefficient().value();
        if(sign > 0 && f != 0) {
            temp += "+";
        }
        if(sign == 0) {
            current = current->next;
            continue;
        }
 
        temp += current->m.monomial2string();
        current = current->next;
        f++;
    }
    
    return temp;   
}

void Polynomial::print() {
    std::cout << this->polynomial2string() << std::endl;
}

void Polynomial::set_monomial(Monomial m) {
    this->m = m;
}

void Polynomial::add_monomial(Monomial m) {
    Polynomial* previous = NULL;
    Polynomial* current = this;
    while(current) {
        if(current->m.get_power() == m.get_power()) {
            if((current->m.get_coefficient() + m.get_coefficient()).value() == 0) { // remove current->m from the polynomial(this)
                if(previous == NULL) {
                    this->m = (current->next) ? current->next->m : current->m + m;
                    this->next = (current->next) ? current->next->next : NULL;                 
                } else {
                    previous->next = current->next;
                    delete(current);
                }
            } else {
                current->m = current->m + m;
            }
            return; 
        }
        if(current->m.get_power() < m.get_power()) {
            Polynomial* t = new Polynomial();
            if(previous == NULL) { // inserting in the beginning
                t->m = this->m;
                t->next = this->next;
                this->m = m;
                this->next = t;
            } else { // inserting in the middle
                t->m = m;
                t->next = current;
                previous->next = t;
            }
            return;
        }
        previous = current;
        current = current->next;
    }

    Polynomial* t = new Polynomial();
    if(previous == NULL) { // inserting in the beginning
        t->m = this->m;
        t->next = this->next;
        this->m = m;
        this->next = t;
    } else { // inserting in the middle
        t->m = m;
        t->next = current;
        previous->next = t;
    }
    return;
}

void Polynomial::subtract_monomial(Monomial m) {
    Polynomial* previous = NULL;
    Polynomial* current = this;
    Monomial minus_one("-1");
    while(current) {
        if(current->m.get_power() == m.get_power()) {
            if((current->m.get_coefficient() - m.get_coefficient()).value() == 0) { // remove current->m from the polynomial(this)
                if(previous == NULL) {
                    this->m = (current->next) ? current->next->m : current->m - m;
                    this->next = (current->next) ? current->next->next : NULL;                 
                } else {
                    previous->next = current->next;
                    delete(current);
                }
            } else {
                current->m = current->m - m;
            }
            return; 
        }
        if(current->m.get_power() < m.get_power()) {
            Polynomial* t = new Polynomial();
            if(previous == NULL) { // inserting in the beginning
                t->m = this->m;
                t->next = this->next;
                this->m = m * minus_one;
                this->next = t;
            } else { // inserting in the middle
                t->m = m * minus_one;
                t->next = current;
                previous->next = t;
            }
            return;
        }
        previous = current;
        current = current->next;
    }

    Polynomial* t = new Polynomial();
    if(previous == NULL) { // inserting in the beginning
        t->m = this->m;
        t->next = this->next;
        this->m = m * minus_one;
        this->next = t;
    } else { // inserting in the middle
        t->m = m * minus_one;
        t->next = current;
        previous->next = t;
    }
    return;
}

void Polynomial::multiply_monomial(Monomial m) {
    Polynomial* current = this;
    while(current) {
        current->m = current->m * m;
        current = current->next;
    }
}

// 4 not freed per polynomial

int main(int argc, char** argv) {
    Polynomial p1(argv[1]), p2(argv[2]);
    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;
    
    std::cout << p1.polynomial2string() << " + " << p2.polynomial2string()  << " = " << sum.polynomial2string() << std::endl;
    std::cout << p1.polynomial2string() << " - " << p2.polynomial2string()  << " = " << diff.polynomial2string() << std::endl;
    std::cout << p1.polynomial2string() << " * " << p2.polynomial2string()  << " = " << prod.polynomial2string() << std::endl;
    
    //Monomial m1(argv[3]);
    //p1.multiply_monomial(m1);
    //std::cout << p1.polynomial2string() << std::endl;

    return 0;
}
