#include <iostream>
#include <vector>
#include <memory>
#include <exception>
#include <string>

using namespace std;

/* =========================
      Classe abstraite
   ========================= */

class Element {
protected:
    string id;

public:
    Element(string i) : id(i) {}

    virtual void afficher() const = 0;   

    string getId() const {
        return id;
    }

    virtual ~Element() {}
};

/* =========================
      Variante Hôtel
      Livre -> Chambre
   ========================= */

class Chambre : public Element {
    string numero;   
    int capacite;    

public:
   
    Chambre(string i, string num, int cap)
        : Element(i), numero(num), capacite(cap) {}

   
    void afficher() const override {
        cout << "[Chambre] id=" << id
             << ", numero=" << numero
             << ", capacite=" << capacite << " personnes"
             << endl;
    }
};

/* =========================
      Variante Hôtel
      CD -> Suite
   ========================= */

class Suite : public Element {
    string type;   
    int surface;   

public:
    
    Suite(string i, string t, int s)
        : Element(i), type(t), surface(s) {}

    void afficher() const override {
        cout << "[Suite] id=" << id
             << ", type=" << type
             << ", surface=" << surface << " m2"
             << endl;
    }
};

/* =========================
      Surcharge opérateur ==
   ========================= */


bool operator==(const Element& e1, const Element& e2) {
    return e1.getId() == e2.getId();
}

/* =========================
      Exception métier
   ========================= */

class ElementInvalide : public exception {
public:
   
    const char* what() const noexcept override {
        return "ElementInvalide: operation impossible / element non valide.";
    }
};

/* =========================
      Classe Facturable
   ========================= */

class Facturable {
protected:
    double tva;

public:
    Facturable(double t = 0.2) : tva(t) {}

   
    double total(double prix) {
        return prix * (1.0 + tva);
    }
};

/* =========================
      Héritage multiple
      Emprunt -> Reservation
   ========================= */

class Reservation : public Element, public Facturable {
    int nuits;
    double prixNuit;

public:
   
    Reservation(string i, int n, double p, double tva_ = 0.2)
        : Element(i), Facturable(tva_), nuits(n), prixNuit(p) {}

   
    double coutTotal() {
        double ht = nuits * prixNuit;
        return total(ht); 
    }

    
    void afficher() const override {
       
        double ht = nuits * prixNuit;
        double ttc = ht * (1.0 + tva);

        cout << "[Reservation] id=" << id
             << ", nuits=" << nuits
             << ", prix/nuit=" << prixNuit
             << ", HT=" << ht
             << ", TVA=" << (tva * 100) << "%"
             << ", TTC=" << ttc
             << endl;
    }
};

/* =========================
      Fonction friend
   ========================= */

class Note {
    double valeur;

public:
    Note(double v) : valeur(v) {}

   
    friend double moyenne(Note a, Note b);
};


double moyenne(Note a, Note b) {
    return (a.valeur + b.valeur) / 2.0;
}

/* =========================
      Polymorphisme
   ========================= */


void afficherTous(vector<shared_ptr<Element>> liste) {
    for (const auto& e : liste) {
        e->afficher();
    }
}

/* =========================
            MAIN
   ========================= */

int main() {
    try {
        vector<shared_ptr<Element>> elements;

        
        elements.push_back(make_shared<Chambre>("H1", "101", 2));
        elements.push_back(make_shared<Suite>("S1", "Royal", 60));
        elements.push_back(make_shared<Reservation>("R1", 5, 200)); // 5 nuits * 200

        cout << "\n--- Affichage polymorphique ---\n";
        afficherTous(elements);

        cout << "\n--- Test operateur == ---\n";
        if (*elements[0] == *elements[1])
            cout << "Même ID\n";
        else
            cout << "IDs differents\n";

        cout << "\n--- Test moyenne friend ---\n";
        Note n1(12), n2(18);
        cout << "Moyenne = " << moyenne(n1, n2) << endl;

        cout << "\n--- Test exception ---\n";
        throw ElementInvalide();
    }
    catch (exception& e) {
        cout << "Erreur : " << e.what() << endl;
    }

    return 0;
}