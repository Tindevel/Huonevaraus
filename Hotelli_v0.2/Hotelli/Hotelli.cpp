#include <iostream>
#include <random>
#include <vector>
#include <limits>
#include <algorithm>


//Tässä on meidän listaat huoneet ja varatut huoneet
struct Huone {
    int numero;
    std::string tyyppi;
};

struct Varaus {
    int huoneNumero;
    std::string tyyppi;
};

std::vector<Huone> yhdenHengenHuoneet;
std::vector<Huone> kahdenHengenHuoneet;
std::vector<Varaus> varatutYhdenHengenHuoneet;
std::vector<Varaus> varatutKahdenHengenHuoneet;

// Tarkista onko huone varattu
bool HuoneOnVarattu(int huoneNumero, const std::vector<Varaus>& varatutHuoneet) {
    for (const auto& varaus : varatutHuoneet) {
        if (varaus.huoneNumero == huoneNumero) {
            return true; // huone on varattu
        }
    }
    return false; // huone ei ole varattu
}
// Tarkista onko huone olemassa
bool HuoneOnOlemassa(int huoneNumero, const std::vector<Huone>& huoneet) {
    for (const auto& huone : huoneet) {
        if (huone.numero == huoneNumero) {
            return true; // huone on olemassa
        }
    }
    return false; // huone ei ole olemassa
}
// Kysy numero ja tarkista että se on numero
int KysyNumero() {
    int numero;
    while (!(std::cin >> numero) || std::cin.peek() != '\n') {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Virheellinen syöttö! Anna kelvollinen numero: ";
    }

    return numero;
}
//Aliohjelma joka laske hinnan sen mukaan minkä huone on varattu
double LaskeKokonaishinta(const std::vector<Varaus>& varatutHuoneet, int oidenMaara) {
    double kokonaishinta = 0.0;
    for (const auto& varaus : varatutHuoneet) {
        if (varaus.tyyppi == "Yhden hengen") {
            kokonaishinta += 100.0;  // Huonehinta on yhden hengen huoneella 100 euroa yöltä
        } else if (varaus.tyyppi == "Kahden hengen") {
            kokonaishinta += 150.0;  // Huonehinta on kahden hengen huoneella 150 euroa yöltä
        }
    }
    return kokonaishinta * oidenMaara;
}
//Aliohjelma joka lisää astunnainen alenus ja tulosta laskun
void TulostaLasku(const Varaus& varaus, int oidenMaara) {
    double kokonaishinta = LaskeKokonaishinta({ varaus }, oidenMaara);

    // Satunnainen alennus 0 %, 10 % tai 20 %
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> alennusJakelu(0.0, 0.2);
    double alennus = static_cast<double>(static_cast<int>(alennusJakelu(gen) * 10.0)) / 10.0;
    double loppusumma = kokonaishinta * (1.0 - alennus);

    // Tulosta lasku
    std::cout << "\n\n\n************ LASKU ************\n" << std::endl;
    std::cout << "Varattu huoneet:" << std::endl;
    std::cout << "Huone numero " << varaus.huoneNumero << ", tyyppi: " << varaus.tyyppi << std::endl;
    std::cout << "\nKokonaishinta " << oidenMaara << " yöksi: " << kokonaishinta << " euroa" << std::endl;
    std::cout << "Alennus: " << alennus * 100.0 << "%" << std::endl;
    std::cout << "Loppusumma: " << loppusumma << " euroa" << std::endl;
    std::cout << "\n********************************\n" << std::endl;
}
//Aliohjelma joka varaa huoneet kahden tai yhden riipu tyypistä
void VaraaHuone(int oidenMaara, std::vector<Huone>& vapaatHuoneet, std::vector<Varaus>& varatutHuoneet, const std::string& huoneTyyppi) {
    std::cout << "Haluatko valita huonen numeron itse (k/e)? ";
    char valinta;
    std::cin >> valinta;

    if (valinta == 'k') {
        std::cout << "Valitse huonen numero: ";
        int valittuHuone = KysyNumero();
        auto it = std::find_if(vapaatHuoneet.begin(), vapaatHuoneet.end(), [valittuHuone](const Huone& h) {
            return h.numero == valittuHuone;
            });

        if (it != vapaatHuoneet.end()) {
            varatutHuoneet.push_back({ it->numero, huoneTyyppi });
            vapaatHuoneet.erase(it);
        }
        else {
            std::cout << "Virheellinen valinta! Tarkista, onko tiedot oikein tai huone on jo varattu." << std::endl;
            VaraaHuone(oidenMaara, vapaatHuoneet, varatutHuoneet, huoneTyyppi);
        }
    }
    else if (valinta == 'e') {
        if (!vapaatHuoneet.empty()) {
            varatutHuoneet.push_back({ vapaatHuoneet.back().numero, huoneTyyppi });
            vapaatHuoneet.pop_back();
        }
        else {
            std::cout << "Kaikki " << huoneTyyppi << " huoneet varattu! Valitse toinen vaihtoehto." << std::endl;
        }
    }
    else {
        std::cout << "Virheellinen syöttö!" << std::endl;
        VaraaHuone(oidenMaara, vapaatHuoneet, varatutHuoneet, huoneTyyppi);
    }
}
//Aliohjelma generoida meille huoneet
void GeneroiHuoneet() {
    // Randomi generaattori
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generoida huoneet 40-300
    std::uniform_int_distribution<int> huonekplJakelu(20, 150);
    int huonekpl = huonekplJakelu(gen) * 2; 
    // Jakaa generoidut huoneet
    int yhdenHengenHuonekpl = huonekpl / 2;
    int kahdenHengenHuonekpl = huonekpl / 2;

    // lisää yhden hengen huoneet
    for (int i = 1; i <= yhdenHengenHuonekpl; ++i) {
        Huone huone;
        huone.numero = i;
        huone.tyyppi = "Yhden hengen";
        yhdenHengenHuoneet.push_back(huone);
    }

    // lisää kahden hengen huoneet
    for (int i = yhdenHengenHuonekpl + 1; i <= huonekpl; ++i) {
        Huone huone;
        huone.numero = i;
        huone.tyyppi = "Kahden hengen";
        kahdenHengenHuoneet.push_back(huone);
    }
}

//Pääohjelma
int main() {
    //Kutsu generaatoria
    GeneroiHuoneet();

    int num = 0;
    while (num != 3) {
        std::cout << "************ HOTELI ************\n" << std::endl;
        std::cout << "Kpl. huoneita:               " << yhdenHengenHuoneet.size() + kahdenHengenHuoneet.size() << std::endl;
        std::cout << "Yhden hengen kpl.:                " << yhdenHengenHuoneet.size() << std::endl;
        std::cout << "Kahden hengen kpl.:               " << kahdenHengenHuoneet.size() << std::endl;
        std::cout << "********Valitse toiminnon*******\n" << std::endl;
        std::cout << "Varaa yhden hengen huone:      1\n" << std::endl;
        std::cout << "Varaa kahden hengen huone:     2\n" << std::endl;
        std::cout << "Lopeta:                        3\n" << std::endl;

        // Tarkistetaan onko syöttänyt numero
        num = KysyNumero();
        int oidenMaara = 0;

        switch (num) {
        case 1:
            VaraaHuone(oidenMaara, yhdenHengenHuoneet, varatutYhdenHengenHuoneet, "Yhden hengen");
            break;
        case 2:
            VaraaHuone(oidenMaara, kahdenHengenHuoneet, varatutKahdenHengenHuoneet, "Kahden hengen");
            break;
        case 3:
            break;
        default:
            std::cout << "Virheellinen valinta!" << std::endl;
            break;
        }
        if (num != 3) {
            while (oidenMaara <= 0) {
                std::cout << "Anna yöpymisten määrä: ";
                oidenMaara = KysyNumero();
            }

            if (num == 1 && !varatutYhdenHengenHuoneet.empty()) {
                TulostaLasku(varatutYhdenHengenHuoneet.back(), oidenMaara);
            }
            else if (num == 2 && !varatutKahdenHengenHuoneet.empty()) {
                TulostaLasku(varatutKahdenHengenHuoneet.back(), oidenMaara);
            }
        }
    }

    return 0;
}
