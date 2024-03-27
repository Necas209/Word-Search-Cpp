#include "Board.h"

void Board::Create_matrix() {
    Ask_DimX();
    Ask_DimY();
    matrix = new Letter *[DimY];
    for (int i = 0; i < DimY; i++) {
        matrix[i] = new Letter[DimX];
        for (int j = 0; j < DimX; j++)
            matrix[i][j].Set_point(j, i);
    }
}

void Board::Show_matrix() const {
    std::cout << std::endl << "    ";
    for (int i = 0; i < DimX; i++)
        std::cout << i % 10 << ' ';
    std::cout << std::endl;
    for (int i = 0; i < DimY; i++) {
        std::cout << "  " << i % 10 << ' ';
        for (int j = 0; j < DimX; j++) {
            if (matrix[i][j].Get_state() == FOUND)
                setForeColor(MY_COLOR_LIGTH_RED);
            std::cout << matrix[i][j];
            resetColor();
        }
        std::cout << std::endl;
    }
}

int Board::Number_USED() const {
    int k = 0;
    for (int i = 0; i < num_words; i++) {
        if (words[i] != NOT_USED)
            k++;
    }
    return k;
}

int Board::Number_NOT_FOUND() const {
    int k = 0;
    for (int i = 0; i < num_words; i++) {
        if (words[i] == NOT_FOUND)
            k++;
    }
    return k;
}

bool Board::Load_list() {
    int option;
    puts("\n\tEscolha o tema da sopa de letras:");
    puts(" 1 -> Distritos de Portugal");
    puts(" 2 -> Estados Unidos da América");
    printf("\n\tOpção: ");
    std::cin >> option;
    if (!std::cin.good() or (option != 1 and option != 2)) {
        PreventLoop();
        return false;
    }
    std::ifstream file;
    switch (option) {
        case 1:
            file.open("distritos.txt");
            break;
        case 2:
            file.open("usa.txt");
            break;
        default:
            break;
    }
    if (!file.is_open()) {
        std::cout << std::endl << "Erro ao abrir o ficheiro." << std::endl;
        return false;
    }
    int i = 0;
    std::string aux;
    while (!file.eof()) {
        getline(file, aux);
        words.emplace_back(aux);
        words[i].Upper_Case();
        i++;
    }
    num_words = words.size();
    file.close();
    return true;
}

void Board::Show_list() {
    int k = 0;
    gotoxy(2 * DimX + 20, 1);
    std::cout << "Palavras encontradas: " << Number_USED() - Number_NOT_FOUND() << " de " << Number_USED();
    for (int i = 0; i < num_words; i++) {
        if (words[i] != FOUND) continue;
        gotoxy(2 * DimX + 20, 3 + 2 * k);
        std::cout << words[i];
        k++;
    }
}

bool Board::Check_Crossing(const int i) const {
    const int x = words[i].Get_initial_x();
    const int y = words[i].Get_initial_y();
    const size_t l = words[i].length();
    const int o = words[i].Get_orientation();
    if (i == 0)
        return true;
    switch (o) {
        case FRONT:
            for (int j = x; j < (x + l); j++) {
                if (!Check_Letter(i, j, y, j - x))
                    return false;
            }
            break;
        case BACK:
            for (int j = x; j > (x - l); j--) {
                if (!Check_Letter(i, j, y, x - j))
                    return false;
            }
            break;
        case DOWN:
            for (int j = y; j < (y + l); j++) {
                if (!Check_Letter(i, x, j, j - y))
                    return false;
            }
            break;
        case UP:
            for (int j = y; j > (y - l); j--) {
                if (!Check_Letter(i, x, j, y - j))
                    return false;
            }
            break;
        case FRONT_DOWN:
            for (int j1 = x, j2 = y; j1 < (x + l) and j2 < (y + l); j1++, j2++) {
                if (!Check_Letter(i, j1, j2, j1 - x))
                    return false;
            }
            break;
        case BACK_UP:
            for (int j1 = x, j2 = y; j1 > (x - l) and j2 > (y - l); j1--, j2--) {
                if (!Check_Letter(i, j1, j2, x - j1))
                    return false;
            }
            break;
        case BACK_DOWN:
            for (int j1 = x, j2 = y; j1 > (x - l) and j2 < (y + l); j1--, j2++) {
                if (!Check_Letter(i, j1, j2, x - j1))
                    return false;
            }
            break;
        case FRONT_UP:
            for (int j1 = x, j2 = y; j1 < (x + l) and j2 > (y - l); j1++, j2--) {
                if (!Check_Letter(i, j1, j2, j1 - x))
                    return false;
            }
            break;
        default: ;
    }
    return true;
}

bool Board::Check_Letter(const int i, const int x, const int y, const int k) const {
    Letter l = matrix[y][x];
    const char c = words[i].Get_str()[k];
    return (l == ' ' or l == c);
}

void Board::Insert_Word(const int i) const {
    const std::string w = words[i].Get_str();
    const int x = words[i].Get_initial_x();
    const int y = words[i].Get_initial_y();
    const size_t l = words[i].length();
    const int o = words[i].Get_orientation();
    const int s = words[i].Get_state();
    switch (o) {
        case FRONT:
            for (int j = x; j < (x + l); j++) {
                matrix[y][j] = w[j - x];
                matrix[y][j].Set_state(s);
            }
            break;
        case BACK:
            for (int j = x; j > (x - l); j--) {
                matrix[y][j] = w[x - j];
                matrix[y][j].Set_state(s);
            }
            break;
        case DOWN:
            for (int j = y; j < (y + l); j++) {
                matrix[j][x] = w[j - y];
                matrix[j][x].Set_state(s);
            }
            break;
        case UP:
            for (int j = y; j > (y - l); j--) {
                matrix[j][x] = w[y - j];
                matrix[j][x].Set_state(s);
            }
            break;
        case FRONT_DOWN:
            for (int j1 = x, j2 = y; j1 < (x + l) and j2 < (y + l); j1++, j2++) {
                matrix[j2][j1] = w[j1 - x];
                matrix[j2][j1].Set_state(s);
            }
            break;
        case BACK_UP:
            for (int j1 = x, j2 = y; j1 > (x - l) and j2 > (y - l); j1--, j2--) {
                matrix[j2][j1] = w[x - j1];
                matrix[j2][j1].Set_state(s);
            }
            break;
        case BACK_DOWN:
            for (int j1 = x, j2 = y; j1 > (x - l) and j2 < (y + l); j1--, j2++) {
                matrix[j2][j1] = w[j2 - y];
                matrix[j2][j1].Set_state(s);
            }
            break;
        case FRONT_UP:
            for (int j1 = x, j2 = y; j1 < (x + l) and j2 > (y - l); j1++, j2--) {
                matrix[j2][j1] = w[j1 - x];
                matrix[j2][j1].Set_state(s);
            }
            break;
        default:
            break;
    }
}

void Board::Read(std::ifstream &is) {
    char c;
    std::string s;
    getline(is, s);
    is >> DimX >> DimY;
    matrix = new Letter *[DimY];
    for (int i = 0; i < DimY; i++)
        matrix[i] = new Letter[DimX];
    getline(is, s);
    getline(is, s);
    for (int y = 0; y < DimY; y++) {
        for (int x = 0; x < DimX; x++) {
            matrix[y][x].Read(is);
            is >> c;
        }
        getline(is, s);
    }
    getline(is, s);
    is >> num_words;
    getline(is, s);
    words.resize(num_words);
    getline(is, s);
    for (int i = 0; i < num_words; i++) {
        words[i].Read(is);
        getline(is, s);
    }
}

void Board::Save(std::ofstream &os) {
    os << "Dimensions:\n";
    os << DimX << ' ' << DimY << ";\n";
    os << "Letters:\n";
    for (int y = 0; y < DimY; y++) {
        for (int x = 0; x < DimX; x++)
            matrix[y][x].Save(os);
        os << std::endl;
    }
    os << "Number of Words:\n";
    os << num_words << ";\n";
    os << "Words:\n";
    for (int i = 0; i < num_words; i++) {
        words[i].Save(os);
        if (i != num_words - 1) {
            os << std::endl;
        }
    }
}

bool Board::Check_If_Word_Is_Present(Word word) {
    word.Upper_Case();
    for (int i = 0; i < num_words; i++) {
        if (word != words[i]) continue;
        if (words[i] != FOUND) {
            words[i].Set_state(FOUND);
            Insert_Word(i);
            return true;
        }
        std::cout << std::endl << " A palavra j� foi encontrada." << std::endl;
        return false;
    }
    std::cout << std::endl << " A palavra n�o se encontra na sopa de letras." << std::endl;
    return false;
}
