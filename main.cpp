#include <SFML/Graphics.hpp>
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
using namespace sf;
using namespace std;
int COLUMS = 5, ROWS = 4;
random_device rnd;

/*
 * Пока что игру запускать просто из компилятора
 * Окно меню: играть, рекорды
 * Окно игры: поле 4х5 или 6х5 клеток, счетчик ходов и времени
 * Ход игры: каждый ход можешь выбрать (перевернуть) два квадратика, какое-то время они тебе видны,
 * потом переворачиваются обратно и начинается следующий ход
 *
 * осталось сделать:
 * разобраться как сделать кнопку чтобы нажималась один раз и дописать спрайты
 * понять как нормально отрисовывать текст и сделать меню/экран входа в игру
 */


struct Coordinates{
    int x;
    int y;
    Coordinates(int _x, int _y) {
        x = _x;
        y = _y;
    }
};

struct Button{
    int x, y;
    int width, height;
    Sprite button;
    // RectangleShape ButtonArea;
    Texture texture1, texture2, pictexture;
    int pic = 1;
    Button() {
        x = 0, y = 0;
        width = height = 0;
        // button.setTextureRect(IntRect(0, 0, x, y));
    }
    Button(int _x, int _y, int _width, int _height, const string& texture1FileName, const string& texture2FileName) {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
        texture1.loadFromFile(texture1FileName);
        texture2.loadFromFile(texture2FileName);
        button.setTextureRect(IntRect(0, 0, width, height));
        button.setPosition(x, y);
        pictexture = texture1;
    }
    void setTexture(int a) {
        if (a == 1) {
            pictexture = texture1;
        } else {
            pictexture = texture2;
        }
        pic = a;
        button.setTexture(pictexture);
    }
    bool isPressed(int _x, int _y) {
        if (x <= _x && _x <= x + width && y <= _y && _y <= y + height) {
            return true;
        }
        else {
            return false;
        }
    }
    void switchPictures() {
        if (pic == 1) {
            pictexture = texture2;
            pic = 2;
        } else {
            pictexture = texture1;
            pic = 1;
        }
        //ButtonArea.setTexture(&picexture);
    }
};

void ButtonCreate(vector <vector <Button>> &Buttons, vector <vector <string>> &ButtonsNames, vector <string> &Pictures, const string& t1) {
    // shuffle(Pictures.begin(), Pictures.end(), rnd());
    // почему-то выдает ошибку, понять почему и починить
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMS; j++) {
            Buttons[i][j] = Button(80 + j * 170, 500 + i * 170, 150, 150, t1, Pictures[i * COLUMS + j]);
            ButtonsNames[i][j] = Pictures[i * COLUMS + j];
            Buttons[i][j].setTexture(1);
        }
    }
}

void PicturesCreate(vector <string> &Pictures) {
    for (int i = 0; i < COLUMS * ROWS / 2; i++) {
        Pictures[i] = Pictures[i + COLUMS * ROWS / 2] = "Images/Picture" + to_string(i + 1) + ".jpeg";
    }
    mt19937 g(rnd());
    shuffle(Pictures.begin(), Pictures.end(), g);
}

int main() {

    RenderWindow window(VideoMode(1000, 1600), "MEMO");

    Clock clock;

    vector <vector <Button>> Buttons(ROWS, vector <Button> (COLUMS));
    vector <vector <string>> ButtonsNames(ROWS, vector <string> (COLUMS));
    vector <string> Pictures(COLUMS * ROWS);

    Font font;
    font.loadFromFile("ComicMono.ttf");
    Text name;
    name.setFont(font);
    name.setString("  MEMO\nfor Mark");
    name.setCharacterSize(150);
    name.setColor(Color::Red);
    name.setStyle(Text::Bold);
    name.setPosition(190, 0);

    Text score;
    int scores = 0;
    score.setFont(font);
    score.setCharacterSize(50);
    score.setColor(Color::Red);
    score.setStyle(Text::Bold);
    score.setPosition(100, 350);

    Text move;
    int moves = 0;
    move.setFont(font);
    move.setCharacterSize(50);
    move.setColor(Color::Red);
    move.setStyle(Text::Bold);
    move.setPosition(500, 350);

    string t1;
    t1 = "Images/ButtonBack.jpeg";
    PicturesCreate(Pictures);
    ButtonCreate(Buttons, ButtonsNames, Pictures, t1);

    string mode = "game", setting = "none";
    // bool game_is_going = false;
    /* когда играю, выбираю две карточки,
     * Если они разные, то они должны повисеть открытыми 2 секунды,
     * в эти секунды мне нельзя нажимать на другие карточки
     * дальше они закрываются и игра продолжается
     * Если они одинаковые, то они остаются открытыми и прибавляется очко к счету
    */
    int countOpenedButtons = 0;
    pair <int ,int> tile1, tile2;
    while (window.isOpen()) {
        window.clear(Color(255, 255, 255));

        vector <Coordinates> MousePresses;
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                MousePresses.push_back(Coordinates(event.mouseButton.x, event.mouseButton.y));
            }
        }// работает

        if (mode == "game") {

            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLUMS; j++) {
                    window.draw(Buttons[i][j].button);
                }
            }

            if (setting == "none") {
                for (auto[x, y] : MousePresses) {
                    for (int i = 0; i < ROWS; i++) {
                        for (int j = 0; j < COLUMS; j++) {
                            if (countOpenedButtons < 2 && Buttons[i][j].isPressed(x, y) && Buttons[i][j].pic != 2) {
                                Buttons[i][j].switchPictures();
                                if (countOpenedButtons == 0) {
                                    tile1 = {i, j};
                                } else {
                                    tile2 = {i, j};
                                }
                                countOpenedButtons++;
                            }
                        }
                    }
                }// работает
                if (countOpenedButtons == 2) {
                    if (ButtonsNames[tile1.first][tile1.second] == ButtonsNames[tile2.first][tile2.second]) {
                        scores++;
                    } else {
                        setting = "pause_2s";
                        clock.restart();
                    }
                    countOpenedButtons = 0;
                    moves++;
                }
            } else if (setting == "pause_2s"){
                if (clock.getElapsedTime().asSeconds() > 2) {
                    Buttons[tile1.first][tile1.second].switchPictures();
                    Buttons[tile2.first][tile2.second].switchPictures();
                    setting = "none";
                }
            } else if (setting == "game_ower") {
                
            }

            window.draw(name);

            score.setString("Score: " + to_string(scores));
            window.draw(score);

            move.setString("Moves: " + to_string(moves));
            window.draw(move);

            window.display();
        }
    }
}
