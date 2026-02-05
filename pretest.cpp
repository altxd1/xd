    #include <iostream>
    #include <string>
    using namespace std;
    #include <exception>
    #include <vector>
    #include <memory>
//Classe Abstraite
   class Document {
protected:
    string code;

public:
    Document(string c) : code(c) {}

    // 1 méthode virtuelle pure
    virtual void afficher() const = 0;

    // 2 getter
    string getCode() const { return code; }

    // 3 destructeur virtuel
    virtual ~Document() = default;
};

// Heritage + override

    class Livre : public Document {
        string titre;
        int pages;

    public:
    //1) Constructeur
        Livre(string c, string t, int p)
            : Document(c), titre(t), pages(p) {}
    //2) Afficher
        void afficher() const override {
            cout << "Livre : code=" << code
                << ", titre=" << titre
                << ", pages=" << pages << endl;
        }
    };

    //Surcharge operateur
    bool operator==(const Document& d1, const Document& d2) {
        return d1.getCode() == d2.getCode();
    }

//Exception personnalisee
    class DocumentInvalide : public exception {
    public:
        const char* what() const noexcept override {
            return "Document invalide détecté !";
        }
    };

    // Interface facturable
    class Facturable {
    protected:
        double tva;

    public:
        Facturable(double t = 0.2) : tva(t) {}

        double total(double prix) {
            return prix * (1 + tva);
        }
    };


// Polymorphisme avec vector + shared_ptr
    void afficherTous(vector<shared_ptr<Document>> liste) {
        for (const auto& doc : liste) {
            if (doc) doc->afficher();
        }
    }
//Bonus - fonction friend
    class Note {
        double val;

    public:
        Note(double v) : val(v) {}

        friend double moyenne(Note a, Note b);
    };

    double moyenne(Note a, Note b) {    
        return (a.val + b.val) / 2;
    }


    int main() {
    vector<shared_ptr<Document>> docs;
    docs.push_back(make_shared<Livre>("L1", "POO", 200));
    docs.push_back(make_shared<Livre>("L2", "Algo", 300));

    afficherTous(docs);

    Livre l1("L1", "Algo", 200);
    Livre l2("L1", "Math", 150);
    if (l1 == l2) cout << "Meme code" << endl;

    Facturable f(0.2);
    cout << "TTC de 100 = " << f.total(100) << endl;

    cout << "Moyenne = " << moyenne(Note(12), Note(16)) << endl;
    return 0;
}

