#pragma once

#include "IDrawableObject.hpp"
#include "Score.hpp"
#include <chrono>
#include <thread>

class Field : public IDrawableObject {
public:
    struct Cell {
        bool used;
        bool canMove;
        bool toDelete;
        int color;
    };

    Field();

    std::vector<Cell>& operator[](size_t index);

    void init(const unsigned int width, const unsigned int height, std::vector<std::shared_ptr<Sprite>>& sprites, glm::vec2& position, float offset);
    virtual void render() override;
    void clear();
    void setScore(Score* pScore);
    void makeFiguresMotionless();
    void moveAllFiguresDownFrom(int y);
    bool shouldAnyLineBeDeleted();
    void deleteLinesAnimation(int j);

    size_t getWidth() { return m_width; }
    size_t getHeight() { return m_height; }

private:
    size_t m_width;
    size_t m_height;
    std::vector<std::vector<Cell>> m_field;

    Score* m_pScore;
    
    std::vector<std::shared_ptr<Sprite>> m_pSprites;
    glm::vec2 m_position;
    float m_offset;
};

using MiniScreen = Field;