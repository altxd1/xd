#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <ctime>
#include <iomanip>
#include <sstream>

//Classe abstraite Operation
class Operation {
protected:
    std::string type;
    std::time_t date;

public:
    Operation(const std::string& t) : type(t), date(std::time(nullptr)) {}

    // 1) méthode virtuelle pure
    virtual void afficher() const = 0;

    // 2) getter
    std::string getType() const { return type; }

    // 3) destructeur virtuel
    virtual ~Operation() = default;
};

std::string formatDate(std::time_t t) {
    std::tm* tmStruct = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tmStruct, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Héritage + override : Depot
class Depot : public Operation {
    double montant;

public:
    // 1) constructeur
    Depot(double m) : Operation("Depot"), montant(m) {}

    // 2) override afficher()
    void afficher() const override {
        std::cout << "Depot : montant=" << std::fixed << std::setprecision(2)
                  << montant << " MAD, date=" << formatDate(date) << std::endl;
    }
};

// Opérateur == sur User (par username)
struct User {
    std::string username;
    std::string password;
    double balance;
};

bool operator==(const User& u1, const User& u2) {
    return u1.username == u2.username;
}

// Exception personnalisée MontantInvalide
class MontantInvalide : public std::exception {
public:
    const char* what() const noexcept override {
        return "Montant invalide detecte !";
    }
};

// Interface Convertisseur
class Convertisseur {
protected:
    double taux;

public:
    Convertisseur(double t = 0.10) : taux(t) {}

    double convertir(double montantMAD) const {
        return montantMAD * taux;
    }
};

// Polymorphisme vector<shared_ptr<Operation>>
void afficherToutes(const std::vector<std::shared_ptr<Operation>>& ops) {
    for (const auto& op : ops) {
        if (op) op->afficher();
    }
}

// BONUS — friend moyenne de deux soldes
class Solde {
    double val;

public:
    Solde(double v) : val(v) {}

    friend double moyenne(const Solde& a, const Solde& b);
};

double moyenne(const Solde& a, const Solde& b) {
    return (a.val + b.val) / 2.0;
}

//main
int main() {
    std::vector<std::shared_ptr<Operation>> ops;
    ops.push_back(std::make_shared<Depot>(150.0));
    ops.push_back(std::make_shared<Depot>(200.0));
    afficherToutes(ops);

    User u1{"omar","123",0};
    User u2{"omar","abc",50};
    if (u1 == u2) std::cout << "Meme utilisateur\n";

    Convertisseur c(0.10);
    std::cout << "100 MAD = " << c.convertir(100) << " USD\n";

    std::cout << "Moyenne soldes = " << moyenne(Solde(120), Solde(80)) << "\n";

    try {
        double montant = -10;
        if (montant <= 0) throw MontantInvalide();
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }

    return 0;
}
