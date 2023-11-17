#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <string.h>
const std::string RESET = "\033[0m";
const std::string BLUE = "\033[34m";
const std::string YELLOW = "\033[33m";

class Jucator {
public:
    std::string nume;
int roll_dice() {
return (rand() % 6) + 1;
}
    Jucator(std::string nume) : nume(nume) {}
};

class InformatiiJucator : virtual public Jucator {
public:
    int bani;
    int pozitie;

    InformatiiJucator(std::string nume, int bani) : Jucator(nume), bani(bani), pozitie(0) {}
};
class Inchisoare;//nu stia ca exista un Inchisoare
class InformatiiProprietati : virtual public Jucator {
public:
    int pret;
    bool available;
    Inchisoare *proprietar;

    InformatiiProprietati(std::string nume, int pret) : Jucator(nume), pret(pret), available(true), proprietar(nullptr) {}
};

class Inchisoare : public InformatiiJucator, public InformatiiProprietati {
public:
    bool in_inchisoare;
    int runde_in_inchisoare;
    static const std::string nume_jail;
    static const std::string nume_start;
    Inchisoare(std::string nume, int bani, int pret) : Jucator(nume), InformatiiJucator(nume, bani), InformatiiProprietati(nume, pret) {}

    void cumpara_proprietate(InformatiiProprietati &proprietate,const std::string &proprietate_curenta) {
    if(proprietate_curenta!=nume_jail){
    if (!in_inchisoare) {
            if(proprietate_curenta!=nume_start){
        if (proprietate.available && bani >= proprietate.pret) {
            std::cout << "Vrei sa cumperi proprietatea " << proprietate.nume << " pentru $" << proprietate.pret << "? (y/n): ";
            char alegere;
            std::cin >> alegere;

            if (alegere == 'y' || alegere == 'Y') {
                bani -= proprietate.pret;
                proprietate.available = false;
                std::cout << nume << " cumparat " << proprietate.nume << std::endl;
            } else {
                std::cout << nume << " a decis sa nu cumper " << proprietate.nume << std::endl;
            }

            // Clear input buffer..
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } } }else {
        std::cout << "Vrei sa platesti $200 ca sa iesi din inchisoare sau vrei sa iti incerci norocul dand cu zarul? (p/r): ";
char alegere;
std::cin >> alegere;
if (alegere == 'p' || alegere == 'P') {
            if (bani >= 200) {
                bani -= 200;
                in_inchisoare = false;
                runde_in_inchisoare = 0;
                std::cout << nume << " A platit $200 ca sa iasa din inchisoare " << std::endl;
            } else {
                std::cout << nume << " nu are suficienti bani sa iasa din inchisoare " << std::endl;
            }
        } else {
            int dice_roll = roll_dice();
            if (dice_roll == 6) {
                in_inchisoare = false;
                runde_in_inchisoare = 0;
                std::cout << nume << " a dat un " << dice_roll << " si a iesit din inchisoare " << std::endl;
            } else {
                ++runde_in_inchisoare;
                std::cout << nume << " a dat un " << dice_roll << " si ramane in  inchisoare pana la urmatoarea runda " << std::endl;
            }
        }

        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}


    void vinde_proprietate(InformatiiProprietati &proprietate) {
        proprietate.available = true;
        proprietate.proprietar = nullptr;/// este un pointer null care nu face trimitere catre nicio zona de memorie..acesta ajuta la eliberare de orice adresa de memorie detinhta anterior
        bani += proprietate.pret;
        std::cout << nume << " vanduta " << proprietate.nume << " pentru $" << proprietate.pret << std::endl;
    }
     int alege_proprietate_de_vanzare(const std::vector<InformatiiProprietati> &proprietati) {
        std::vector<int> indice_proprietati_detinute;

        for (size_t i = 0; i < proprietati.size(); ++i) {
            if (proprietati[i].proprietar == this) {
                indice_proprietati_detinute.push_back(i);
            }
        }

        if (indice_proprietati_detinute.empty()) {
            return -1;
        }

        std::cout << nume << ", alege o proprietate de vanzare (introdu indicele):" << std::endl;
        for (int indice : indice_proprietati_detinute) {
            std::cout << indice << ". " << proprietati[indice].nume << " ($" << proprietati[indice].pret << ")" << std::endl;
        }

        int indice_ales;
        std::cin >> indice_ales;
        while (std::find(indice_proprietati_detinute.begin(), indice_proprietati_detinute.end(), indice_ales) == indice_proprietati_detinute.end()) {
            std::cout << "Indice invalid, alege un indice valid din lista: ";
            std::cin >> indice_ales;
        }

        return indice_ales;
    }




 void afisare_proprietati(const std::vector<InformatiiProprietati> &proprietati) const {
    std::cout << nume << " proprietati: ";
    bool are_proprietati = false;
    for (const auto &proprietate : proprietati) {
        if (proprietate.proprietar == this) {
            std::cout << proprietate.nume << " ($" << proprietate.pret << "), ";
            are_proprietati = true;
        }
    }

    if (!are_proprietati) {
        std::cout << " nu ";
    }

    std::cout << std::endl;
}


};
const std::string Inchisoare::nume_jail = "Jail";
const std::string Inchisoare::nume_start = "Start";
class Game {
private:
    std::vector<Inchisoare> jucator;
    std::vector<InformatiiProprietati> proprietati;
    int num_jucatori;

    int roll_dice() {
        return (rand() % 6) + 1;
    }

public:
    Game(int num_jucatori) : num_jucatori(num_jucatori) {
        for (int i = 1; i <= num_jucatori; i++) {
            jucator.emplace_back("jucator" + std::to_string(i), 1000, 0);
        }

         proprietati.emplace_back("Start", 400);
        proprietati.emplace_back("Pietrosani", 300);
        proprietati.emplace_back("Domnesti", 400);
        proprietati.emplace_back("Varful Goila", 500);
        proprietati.emplace_back("Rapa", 600);
        proprietati.emplace_back("Raul Doamnei", 200);
        proprietati.emplace_back("Retevoiesti", 200);
        proprietati.emplace_back("Badesti", 400);
        proprietati.emplace_back("Biserica", 300);
        proprietati.emplace_back("Stadion", 400);
        proprietati.emplace_back("Jail", 300);
        proprietati.emplace_back("Targ", 100);


    }
     bool is_game_over() {
        for (const auto &jucator : jucator) {
            if (jucator.bani < 0) {
                return true;
            }
        }
        return false;
    }
 void afisare_buget_jucator() {
    for (const auto &jucator : jucator) {
        std::cout << jucator.nume << " buget: $" << jucator.bani << std::endl;
        jucator.afisare_proprietati(proprietati);

    }
}


   void afisare_tabla() {

    std::cout << BLUE << "---------------   ---------------   ---------------   ---------------" << RESET << std::endl;
    std::cout << "| " << YELLOW << std::setw(10) << std::left << proprietati[0].nume << RESET << " |";
    std::cout << " | " << YELLOW << std::setw(10) << std::left << proprietati[1].nume << RESET << " |";
    std::cout << " | " << YELLOW << std::setw(10) << std::left << proprietati[2].nume << RESET << " |";
    std::cout << " | " << YELLOW << std::setw(10) << std::left << proprietati[3].nume << RESET << " |" << std::endl;
    std::cout << BLUE << "---------------   ---------------   ---------------   ---------------" << RESET << std::endl;

    std::cout << "---------------";
    std::cout << "               ";
    std::cout << "               ";
    std::cout << "---------------" << std::endl;
    std::cout << "| " << YELLOW << std::setw(10) << std::left << proprietati[11].nume << RESET << " |";
    std::cout << "               ";
    std::cout << "               ";
    std::cout << "| " << YELLOW << std::setw(10) << std::left << proprietati[4].nume << RESET << " |" << std::endl;
    std::cout << "---------------";
    std::cout << "               ";
    std::cout << "               ";
    std::cout << "---------------" << std::endl;

    std::cout << "---------------";
    std::cout << "               ";
    std::cout << "               ";
    std::cout << "---------------" << std::endl;
    std::cout << "| " << YELLOW << std::setw(10) << std::left << proprietati[10].nume << RESET << " |";
    std::cout << "               ";
    std::cout << "               ";
    std::cout << "| " << YELLOW << std::setw(10) << std::left << proprietati[5].nume << RESET << " |" << std::endl;
    std::cout << "---------------";
    std::cout << "               ";
    std::cout << "               ";
    std::cout << "---------------" << std::endl;

    std::cout << BLUE << "---------------   ---------------   ---------------   ---------------" << RESET << std::endl;
    std::cout << "| " << YELLOW << std::setw(10) << std::left << proprietati[9].nume << RESET << " |";
    std::cout << " | " << YELLOW << std::setw(10) << std::left << proprietati[8].nume << RESET << " |";
    std::cout << " | " << YELLOW << std::setw(10) << std::left << proprietati[7].nume << RESET << " |";
    std::cout << " | " << YELLOW << std::setw(10) << std::left << proprietati[6].nume << RESET << " |" << std::endl;
     std::cout << BLUE << "---------------   ---------------   ---------------   ---------------" << RESET << std::endl;

}


void play_round() {
    for (auto &jucator : jucator) {
        std::cout << jucator.nume << ", apasa ENTER pentru a arunca zarul ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        int dice_roll = roll_dice();
        int new_pozitie = (jucator.pozitie + dice_roll) % proprietati.size();

        // Verificați dacă jucătorul a trecut de Start
        if (new_pozitie < jucator.pozitie) { ///daca noua pozitie mai mica decat cea pe care era inseamna ca a trecut de start
            jucator.bani += 300;
            std::cout << jucator.nume << " a trecut de Start si a primit $300." << std::endl;
        }

        jucator.pozitie = new_pozitie;
        InformatiiProprietati &proprietate = proprietati[jucator.pozitie];
        std::cout << jucator.nume << " a dat un " << dice_roll << " si a aterizat pe " << proprietate.nume << std::endl;
        if (proprietate.available) {
            jucator.cumpara_proprietate(proprietate,proprietate.nume);
            if (!proprietate.available) {
                proprietate.proprietar = &jucator;
            }
        } else {
            if (proprietate.proprietar != &jucator) {
                jucator.bani -= proprietate.pret;
                proprietate.proprietar->bani += proprietate.pret;
                std::cout << jucator.nume << " a platit $" << proprietate.pret << " chirie catre " << proprietate.proprietar->nume << std::endl;
            } else {
                std::cout << proprietate.nume << " este deja detinuta de " << jucator.nume << std::endl;
            }
        }

        while (jucator.bani < 0) {
            int proprietate_indice = jucator.alege_proprietate_de_vanzare(proprietati);
            if (proprietate_indice == -1) {
                std::cout << jucator.nume << " nu mai are proprietati de vanzare..Bankrupt.. Game over!" << std::endl;
                exit(0);
            }
            jucator.vinde_proprietate(proprietati[proprietate_indice]);
            if (jucator.bani >= 0) {
                break;
            }
        }
    }
    std::cout << "Apasa Enter ca sa continui.";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    system("cls");///sterge continutul afisat in consola
    afisare_tabla();
}
const std::vector<InformatiiProprietati>& getProprietati() const {
    return proprietati;
}

};

int main() {
    srand(static_cast<unsigned int>(time(NULL)));//cast seteaza tipul de date din paranteze la ce e intre <>

    Game game(2); //se poate si in mai multi
    int round = 1;
    game.afisare_tabla();
    game.afisare_buget_jucator();
    while (!game.is_game_over()) {
        std::cout << "Round " << round << std::endl;
        game.play_round();
        game.afisare_buget_jucator();

        ++round;
    }

    std::cout << "Game over!" << std::endl;

    return 0;
}

