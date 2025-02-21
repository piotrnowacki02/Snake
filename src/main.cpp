#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <deque>
#include <string>

using namespace std;
using namespace sf;

struct Point
{
    int x;
    int y;
};

int isHeadinSnake(Point* head, std::deque<Point>& snake)
{
    for(int i = 1; i < snake.size(); i++)
    {
        if(head->x == snake[i].x && head->y == snake[i].y)
            return 1;
    }
    return 0;
}

int isHeadinWall(Point* head)
{
    if(head->x == 0 || head->y == 0 || head->x == 870 || head->y == 570)
        return 1;
    return 0;
}

int isItGameOver(std::deque<Point>& snake)
{
    if(isHeadinSnake(&snake[0], snake) || isHeadinWall(&snake[0]))
        return 1;
    return 0;
}

int isItEndOfScreen(Point* head, RenderWindow* window)
{
    if(head->x < 0 || head->y < 0 || head->x >= window->getSize().x || head->y >= window->getSize().y)
        return 1;
    return 0;
}

void endOfScreen(Point* head, RenderWindow* window)
{
    if(head->x < 0)
        head->x = window->getSize().x;
    else if(head->y < 0)
        head->y = window->getSize().y;
    else if(head->x >= window->getSize().x)
        head->x = 0;
    else if(head->y >= window->getSize().y)
        head->y = 0;
}

int isFoodinSnakeArea(std::deque<Point>& snake, Sprite* food)
{
    for(int i = 0; i < snake.size(); i++)
    {
        if(food->getPosition().x == snake[i].x && food->getPosition().y == snake[i].y)
            return 1;
    }
    return 0;
}

int isFoodinHeadArea(Point* head, Sprite* food)
{
    if(food->getPosition().x == head->x && food->getPosition().y == head->y)
        return 1;
    return 0;
}

void newFoodPosition(Sprite* food, std::deque<Point>& snake)
{
    srand(time(0));
    do
    {
        float x = ((rand() % (28))+1)*30;
        float y = ((rand() % (18))+1)*30;
        food->setPosition(x, y);
    } while (isFoodinSnakeArea(snake, food));
}

void updateTextOfPoints(Text* text, int* points)
{
    std::string point_string = "PUNKTY: " + std::to_string(*points);
    text->setString(point_string);
}

void eatFood(Sprite* food, int* points, Text* points_text, std::deque<Point>& snake)
{
    if (isFoodinHeadArea(&snake[0], food))
    {
        snake.push_back(Point{snake[snake.size() - 1].x, snake[snake.size() - 1].y});
        newFoodPosition(food, snake);
        (*points)++;
        updateTextOfPoints(points_text, points);
    }
}

int isMovingLeft(Point* prev, Point* next)
{
    return next->x - prev->x < 0;
}

int isMovingRight(Point* prev, Point* next)
{
    return next->x - prev->x > 0;
}

int isMovingDown(Point* prev, Point* next)
{
    return next->y - prev->y > 0;
}

int isMovingUp(Point* prev, Point* next)
{
    return next->y - prev->y < 0;
}

void updateBodyTexture(Texture* texture, Point* prev, Point* middle, Point* next)
{
    if((isMovingLeft(prev, middle) && isMovingLeft(middle, next)) || (isMovingRight(prev, middle) && isMovingRight(middle, next)))
        texture->loadFromFile("assets/Textures/body/0.png");
    else if((isMovingDown(prev, middle) && isMovingDown(middle, next)) || (isMovingUp(prev, middle) && isMovingUp(middle, next)))
        texture->loadFromFile("assets/Textures/body/1.png");
    else if((isMovingUp(prev, middle) && isMovingRight(middle, next)) || (isMovingLeft(prev, middle) && isMovingDown(middle, next)))
        texture->loadFromFile("assets/Textures/body/2.png");
    else if((isMovingRight(prev, middle) && isMovingDown(middle, next)) || (isMovingUp(prev, middle) && isMovingLeft(middle, next)))
        texture->loadFromFile("assets/Textures/body/3.png");
    else if((isMovingDown(prev, middle) && isMovingRight(middle, next)) || (isMovingLeft(prev, middle) && isMovingUp(middle, next)))
        texture->loadFromFile("assets/Textures/body/4.png");
    else if((isMovingRight(prev, middle) && isMovingUp(middle, next)) || (isMovingDown(prev, middle) && isMovingLeft(middle, next)))
        texture->loadFromFile("assets/Textures/body/5.png");
}

void updateTailTexture(Texture* texture, Point* tail, Point* butt)
{
    if(tail->x - butt->x < 0)
        texture->loadFromFile("assets/Textures/tail/0.png");
    else if(tail->x - butt->x > 0)
        texture->loadFromFile("assets/Textures/tail/2.png");
    else if(tail->y - butt->y < 0)
        texture->loadFromFile("assets/Textures/tail/3.png");
    else if(tail->y - butt->y > 0)
        texture->loadFromFile("assets/Textures/tail/1.png");
}

void updateHeadTexture(Texture* texture, Sprite* head, Point* neck)
{
    if(neck->x - head->getPosition().x < 0)
        texture->loadFromFile("assets/Textures/head/1.png");   
    else if(neck->x - head->getPosition().x > 0)
        texture->loadFromFile("assets/Textures/head/3.png"); 
    else if(neck->y - head->getPosition().y > 0)
        texture->loadFromFile("assets/Textures/head/0.png"); 
    else if(neck->y - head->getPosition().y < 0)
        texture->loadFromFile("assets/Textures/head/2.png"); 
}

void updateVectorAfterKeyboardInput(Vector2i* v)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && v->y != 0)
    {
        v->x = -30;
        v->y = 0;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && v->y != 0)
    {
        v->x = 30;
        v->y = 0;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && v->x != 0)
    {
        v->x = 0;
        v->y = -30;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && v->x != 0)
    {
        v->x = 0;
        v->y = 30;
    }
}

void drawSnake(RenderWindow* window, std::deque<Point>& snake, Sprite* sprite, Texture* texture)
{
    // sprite->setScale(1.f, 1.f); // Ustaw skalowanie na 1:1, aby przywrócić domyślny rozmiar
    sprite->setPosition(snake[snake.size()-1].x, snake[snake.size()-1].y);
    updateTailTexture(texture, &snake[snake.size()-1], &snake[snake.size()-2]);
    window->draw((*sprite)); // Drawing tail

    for(int i = 1; i < snake.size() - 1; i++)
    {
        updateBodyTexture(texture, &snake[i-1], &snake[i], &snake[i+1]);
        sprite->setPosition(snake[i].x, snake[i].y);
        window->draw((*sprite)); // Drawing body parts
    }

    sprite->setPosition(snake[0].x, snake[0].y);
    updateHeadTexture(texture, sprite, &snake[1]);
    window->draw((*sprite)); // Drawing head
}


void moveSnake(std::deque<Point>& snake, Vector2i v)
{
    snake.pop_back();
    snake.push_front(Point{snake.front().x + v.x, snake.front().y + v.y});
}

int closingWindow(RenderWindow* window, Event* event)
{
    while (window->pollEvent(*event))
    {
        if (event->type == Event::Closed)
        {
            window->close();
            return 1;
        }
    }
    return 0;
}

void startScreen(RenderWindow* window, Event* event)
{
    Font font;
    font.loadFromFile("assets/arial.ttf");
    window->clear(Color(150, 255, 150));

    Texture texture;
    texture.loadFromFile("assets/Textures/snake_selfie.png");
    Sprite snake(texture);
    snake.setPosition(270, 120);

    Text text;
    text.setFont(font);
    text.setString("SNAKE GAME");
    text.setCharacterSize(50);
    text.setFillColor(Color::Black);
    text.setPosition(270, 50);

    Text instruction;
    instruction.setFont(font);
    instruction.setString("Press SPACE to Start");
    instruction.setCharacterSize(30);
    instruction.setFillColor(Color::Black);
    instruction.setPosition(300, 400);

    while (window->isOpen())
    {
        if (closingWindow(window, event))
            return;

        if (Keyboard::isKeyPressed(Keyboard::Space))
            return;

        window->clear(Color(150, 255, 150));
        window->draw(snake);
        window->draw(text);
        window->draw(instruction);
        window->display();
    }
}

void gameLoop(RenderWindow* window)
{
    std::deque<Point> snake = { {300, 300}, {270, 300}, {240, 300} };
    Vector2i direction(30, 0);

    Texture headTexture, bodyTexture, tailTexture, foodTexture;
    headTexture.loadFromFile("assets/Textures/head/0.png");
    bodyTexture.loadFromFile("assets/Textures/body/0.png");
    tailTexture.loadFromFile("assets/Textures/tail/0.png");
    foodTexture.loadFromFile("assets/Textures/food.png");

    Sprite bodySprite(bodyTexture);
    Sprite foodSprite(foodTexture);

    float scaleFactorX = 30.f / headTexture.getSize().x;
    float scaleFactorY = 30.f / headTexture.getSize().y;
    bodySprite.setScale(scaleFactorX, scaleFactorY);
    foodSprite.setScale(scaleFactorX, scaleFactorY);


    newFoodPosition(&foodSprite, snake);

    Font font;
    font.loadFromFile("assets/arial.ttf");
    Text pointsText;
    pointsText.setFont(font);
    pointsText.setCharacterSize(20);
    pointsText.setFillColor(Color::Black);
    pointsText.setPosition(10, 10);

    int points = 0;
    updateTextOfPoints(&pointsText, &points);

    Clock clock;
    Event event;

        while (window->isOpen())
    {
        while (window->pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window->close();
                return;
            }

            // Obsługa klawiszy
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Left && direction.x == 0)
                {
                    direction.x = -30;
                    direction.y = 0;
                }
                else if (event.key.code == Keyboard::Right && direction.x == 0)
                {
                    direction.x = 30;
                    direction.y = 0;
                }
                else if (event.key.code == Keyboard::Up && direction.y == 0)
                {
                    direction.x = 0;
                    direction.y = -30;
                }
                else if (event.key.code == Keyboard::Down && direction.y == 0)
                {
                    direction.x = 0;
                    direction.y = 30;
                }
            }
        }

        // Logika gry
        if (isItGameOver(snake))
            return;

        if (clock.getElapsedTime().asMilliseconds() > 200)
        {
            moveSnake(snake, direction);
            eatFood(&foodSprite, &points, &pointsText, snake);
            clock.restart();
        }

        // Renderowanie
        window->clear(Color(150, 255, 150));
        window->draw(foodSprite);
        drawSnake(window, snake, &bodySprite, &bodyTexture);
        window->draw(pointsText);
        window->display();
    }

}

int main()
{
    RenderWindow window(VideoMode(900, 600), "Snake Game");
    Event event;

    while (window.isOpen())
    {
        startScreen(&window, &event);
        gameLoop(&window);
    }

    return 0;
}
