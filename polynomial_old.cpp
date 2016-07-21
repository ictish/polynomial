#include <iostream>
#include <string>
#include <cstring>
#include "monomial.cpp"

class Polynomial {
    public:
        Polynomial();
        Polynomial(std::string args);
 
        float evaluate(int x);
        float evaluate(float x);
 
        void add(Polynomial* p);
        void subtract(Polynomial* p);
        void multiply(Polynomial* p);
        void multiply(int x);
        void divide(Polynomial* p);
 
        void derivative();
        void indefinite_integral();
        float definite_integral(int x1, int x2);
        float definite_integral(float x1, float x2);
 
        void print();
        Polynomial* copy();
        
    private:
        Monomial* m = NULL; 
        Polynomial* next = NULL;
        void add_monomial(Monomial* m);
        void subtract_monomial(Monomial* m);
        void multiply_monomial(Monomial* m);
};

Polynomial::Polynomial() {
    this->m = NULL;
    this->next = NULL;
}

Polynomial::Polynomial(std::string args) {   
     //parse the argument to extract monomials
     std::string monomial_string;
     std::string::iterator it, start;
     start = args.begin();
     Polynomial* current = this;
     for(it = args.begin(); it < args.end(); it++) {
        if((*it == '+' || *it == '-') && (*(it - 1) != '^' && *(it - 1) != '/')){ // x^-2/3 or 2/-3 type should not be included
            monomial_string = std::string(start, it);
            // make sure that monomial_string is not empty
            if(monomial_string == "") {
                start = it;
                continue;
            }
            //std::cout << monomial_string << std::endl;
            if(current->m == NULL) {
                current->m = new Monomial(monomial_string);
            } else {
                Polynomial* t = new Polynomial();
                t->m = new Monomial(monomial_string);
                current->next = t;
                current = current->next;
            }
            start = it;
        }
     }
     monomial_string =  std::string(start, it);
     //std::cout << monomial_string << std::endl;
     if(current->m == NULL) {
        current->m = new Monomial(monomial_string);
     } else {
        Polynomial* t = new Polynomial();
        t->m = new Monomial(monomial_string);
        current->next = t;
        current = current->next;
     }
}

Polynomial* Polynomial::copy() {
    Polynomial* current = this;
    Polynomial* new_head = NULL;
    Polynomial* new_tail = NULL;

    while(current) {
        Polynomial* t = new Polynomial();
        t->m = new Monomial();
        t->m->set_coefficient(new Fraction(current->m->get_coefficient()));
        t->m->set_base(current->m->get_base());
        t->m->set_power(new Fraction(current->m->get_power()));
        
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

    return new_head;
}

float Polynomial::evaluate(int x) {
    float sum = 0;
    Polynomial* current = this;
    while(current) {
        sum += current->m->evaluate(x);
        current = current->next;
    }

    return sum;
}

float Polynomial::evaluate(float x) {
    float sum = 0;
    Polynomial* current = this;
    while(current) {
        sum += current->m->evaluate(x);
        current = current->next;
    }

    return sum;
}

void Polynomial::add(Polynomial* p) {
    Polynomial* current = p;
    while(current) {
        this->add_monomial(current->m);
        current = current->next;
    }
}

void Polynomial::subtract(Polynomial* p) {
    Polynomial* current = p;
    while(current) {
        this->subtract_monomial(current->m);
        current = current->next;
    }
}


void Polynomial::multiply(Polynomial* p) {
    Polynomial* current = p;
    
    Polynomial* orig = this->copy();

    while(current) {
        Polynomial* t = orig->copy();
        t->multiply_monomial(current->m);
        this->add(t);
        delete(t);
        current = current->next;
    }

    this->subtract(orig);
    delete(orig);
}

void Polynomial::derivative() {
    Polynomial* current = this;
    while(current) {
        current->m->derivative();
        current = current->next;
    }
}

void Polynomial::indefinite_integral() {
    Polynomial* current = this;
    while(current) {
        current->m->indefinite_integral();
        current = current->next;
    }
}

float Polynomial::definite_integral(int x1, int x2) {
    float sum = 0;
    Polynomial* current = this;
    while(current) {
        sum += current->m->definite_integral(x1, x2);
        current = current->next;
    }

    return sum;
}

float Polynomial::definite_integral(float x1, float x2) {
    float sum = 0;
    Polynomial* current = this;
    while(current) {
        sum += current->m->definite_integral(x1, x2);
        current = current->next;
    }

    return sum;
}

void Polynomial::print() {
    Polynomial* current = this;
    int f = 0;
    while(current) {
        float sign = current->m->get_coefficient()->value();
        if(sign > 0 && f != 0) {
            std::cout << "+";
        }
        if(sign == 0) {
            current = current->next;
            continue;
        }
        current->m->print();
        current = current->next;
        f++;
    }
}

void Polynomial::add_monomial(Monomial* m) {
    // if m is NULL, return
    if(m == NULL) {
        return;
    }

    // if this is NULL
    if(this == NULL) {
        this->m = m;
        this->next = NULL;
    }

    float mp = m->get_power()->value();
    Polynomial* previous = NULL;
    Polynomial* current = this;
    while(current) {
        float pp = current->m->get_power()->value();
        if(pp == mp) {
            current->m->add(m);
            return; 
        }
        if(pp < mp) {
            if(previous == NULL) {
                Polynomial* p = new Polynomial();
                p->m = this->m;
                p->next = this->next;
                this->m = m;
                this->next = p;
            } else {
                Polynomial* p = new Polynomial();
                p->m = m;
                p->next = current;
                previous->next = p;
            }
            return;
        }
        previous = current;
        current = current->next;
    }
}

void Polynomial::subtract_monomial(Monomial* m) {   
    // if m is NULL, return
    if(m == NULL) {
        return;
    }

    // if this is NULL
    if(this == NULL) {
        m->multiply(-1);
        this->m = m;
        this->next = NULL;
    }

    float mp = m->get_power()->value();
    Polynomial* previous = NULL;
    Polynomial* current = this;
    while(current) {
        float pp = current->m->get_power()->value();
        if(pp == mp) {
            current->m->subtract(m);
            return; 
        }
        if(pp < mp) {
            m->multiply(-1);
            if(previous == NULL) {
                Polynomial* p = new Polynomial();
                p->m = this->m;
                p->next = this->next;
                this->m = m;
                this->next = p;
            } else {
                Polynomial* p = new Polynomial();
                p->m = m;
                p->next = current;
                previous->next = p;
            }
            return;
        }
        previous = current;    
        current = current->next;
    }
}

void Polynomial::multiply_monomial(Monomial* m) {
    // if m is NULL, return
    if(m == NULL || this == NULL) {
        return;
    }

    Polynomial* current = this;
    while(current) {
        current->m->multiply(m);
        current = current->next;
    }
}

int main(int argc, char** argv) {
    Polynomial* p = new Polynomial(argv[1]);
    p->print();
    std::cout << std::endl;
/*
    Polynomial* t0 = new Polynomial(argv[1]);
    float r = t0->evaluate(1);
    std::cout << "evaluate(1) :" << r <<  std::endl;

    Polynomial* t1 = new Polynomial(argv[1]);
    t1->derivative();
    std::cout << "differentiate :";
    t1->print();
    std::cout << std::endl;

    Polynomial* t2 = new Polynomial(argv[1]);
    t2->indefinite_integral();
    std::cout << "indefinite_integral :";
    t2->print();
    std::cout << std::endl;
    
    Polynomial* t3 = new Polynomial(argv[1]);
    float a = t3->definite_integral(0, 1);
    std::cout << "definite_integral(0, 1) :" << a << std::endl;
  */  
    /*
    Monomial* m = new Monomial(argv[2]);
    Polynomial* t4 = new Polynomial(argv[1]);
    t4->add_monomial(m);
    std::cout << "adding_monomial :";
    t4->print();
    std::cout << std::endl;

    Polynomial* t5 = new Polynomial(argv[1]);
    t5->subtract_monomial(m);
    std::cout << "subtracting_monomial :";
    t5->print();
    std::cout << std::endl;
    */
    Polynomial* s = new Polynomial(argv[2]);
    Polynomial* t6 = new Polynomial(argv[1]);
    t6->add(s);
    std::cout << "adding :";
    t6->print();
    std::cout << std::endl;
 
    std::cout << "copying :";  
    Polynomial* c = p->copy();
    c->print();
    std::cout << std::endl;
    
    Polynomial* t7 = new Polynomial(argv[1]);
    t7->multiply(s);
    std::cout << "multiplying :";
    t7->print();
    std::cout << std::endl;
    
    return 0;
}
