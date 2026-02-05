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
      Variante École
      Livre -> Etudiant
   ========================= */

class Etudiant : public Element {
    string nom;
    int niveau;

public:

    Etudiant(string i, string n, int nv)
        : Element(i), nom(n), niveau(nv) {}


    void afficher() const override {
        cout << "[Etudiant] id=" << id
             << ", nom=" << nom
             << ", niveau=" << niveau
             << endl;
    }
};

/* =========================
      Variante École
      CD -> Prof
   ========================= */

class Prof : public Element {
    string matiere;
    int heuresSemaine;

public:

    Prof(string i, string m, int h)
        : Element(i), matiere(m), heuresSemaine(h) {}

    void afficher() const override {
        cout << "[Prof] id=" << id
             << ", matiere=" << matiere
             << ", heures/semaine=" << heuresSemaine
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
      Emprunt -> Inscription
   ========================= */

class Inscription : public Element, public Facturable {
    int mois;
    double fraisMensuel;

public:

    Inscription(string i, int m, double f, double tva_ = 0.2)
        : Element(i), Facturable(tva_), mois(m), fraisMensuel(f) {}


    double coutTotal() {
        double ht = mois * fraisMensuel;
        return total(ht); 
    }

    void afficher() const override {
        double ht = mois * fraisMensuel;
        double ttc = ht * (1.0 + tva);

        cout << "[Inscription] id=" << id
             << ", mois=" << mois
             << ", frais/mois=" << fraisMensuel
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

        elements.push_back(make_shared<Etudiant>("E1", "Amina", 3));
        elements.push_back(make_shared<Prof>("P1", "POO C++", 6));
        elements.push_back(make_shared<Inscription>("I1", 5, 300)); // 5 mois * 300

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