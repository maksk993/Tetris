#pragma once

enum class Figures { O, L, J, T, S, Z, I };

struct Figure {
    Figure() = default;
    Figure(Figures type) {
        switch (type)
        {
        case Figures::O:
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 4; j++) {
                    if (j == 1 || j == 2) figureCells[i][j] = true;
                    else figureCells[i][j] = false;
                }
            }
            break;
        case Figures::L:
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 4; j++) {
                    if (i == 0) figureCells[i][j] = true;
                    else figureCells[i][j] = false;
                }
            }
            figureCells[1][3] = true;
            figureCells[0][0] = false;
            break;
        case Figures::J:
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 4; j++) {
                    if (i == 0) figureCells[i][j] = true;
                    else figureCells[i][j] = false;
                }
            }
            figureCells[1][1] = true;
            figureCells[0][0] = false;
            break;
        case Figures::S:
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 4; j++) {
                    if (i == 0 && (j == 1 || j == 2)) figureCells[i][j] = true;
                    else if (i == 1 && (j == 2 || j == 3)) figureCells[i][j] = true;
                    else figureCells[i][j] = false;
                }
            }
            break;
        case Figures::Z:
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 4; j++) {
                    if (i == 0 && (j == 2 || j == 3)) figureCells[i][j] = true;
                    else if (i == 1 && (j == 1 || j == 2)) figureCells[i][j] = true;
                    else figureCells[i][j] = false;
                }
            }
            break;
        case Figures::T:
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 4; j++) {
                    if (i == 1 && j == 2) figureCells[i][j] = true;
                    else if (i == 0 && j != 0) figureCells[i][j] = true;
                    else figureCells[i][j] = false;
                }
            }
            break;
        case Figures::I:
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 4; j++) {
                    if (i == 0) figureCells[i][j] = true;
                    else figureCells[i][j] = false;
                }
            }
            break;
        default:
            std::memset(figureCells, 0, sizeof(figureCells));
            break;
        }
        this->type = type;
    }
    bool figureCells[2][4];
    Figures type;
};