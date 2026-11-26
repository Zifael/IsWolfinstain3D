#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#define PI 3.1415926535


float px, py, pdx, pdy, pRadian; // позиция игрока  // pa - угол игрока, pdx это нижний катет pdy = катет вертикальный

int mapX = 8, mapY = 8, mapS = 64;
int map[] =
{
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
};

void drawMap2D() {
    int x, y, xo, yo;
    // Крч мы просто пробегаемся по масиву и рисуем квадратики (белый где в масиве есть 1 и черные где 0)
    for (y = 0; y < mapY; y++) {
        for (x = 0; x < mapX; x++) {
            // просто пробегаем по масиву тк он не двухмерный пример map(строка 3 колонка 5  3 * 5 +  1 = 16 элемент и проверяем если ли у него 1 если да рисуем белый )
            if (map[y * mapX + x] == 1) { glColor3f(1, 1, 1); } else { glColor3f(0, 0, 0);}
            xo = x * mapS; yo = y * mapS;
            glBegin(GL_QUADS); // Рисуем квадрат
            glVertex2i(xo + 1, yo); // левый верхний угол
            glVertex2i(xo + 1, yo + mapS - 1); // левый нижний
            glVertex2i(xo + mapS - 1, yo + mapS - 1); // правый нижний
            glVertex2i(xo + mapS - 1, yo + 1); // правый вверхний
            glEnd();
        }
    }
}


void drawPlayer() {
    glColor3f(1.0f, 1.0f, 0.0f);   // жёлтый цвет (R, G, B от 0 до 1)
    glPointSize(8.0f);             // размер точки в пикселях
    glBegin(GL_POINTS);
    glVertex2i(px, py);        // координаты точки игрока
    glEnd();

    glLineWidth(3); // Толищна луча
    glBegin(GL_LINES);
    glVertex2i(px, py); // начало луча 
    glVertex2i(px + pdx * 5, py + pdy * 5); // конец луча
    glEnd();
}

void move(GLFWwindow* window) {

    std::cout << "pa = " << pRadian << " px = " << pdx << " py = " << pdy <<"\n";
    // меня кординаты игрока и каждый кадр меняются кордианты в drwaPLayer()
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { 
        pRadian -= 0.1;
        if (pRadian < 0) {  // получается если число будет отрицательным оно уйдет  на второй оборот в круге
            pRadian += 2 * PI;
        }
        pdx = cos(pRadian) * 2; // горизонтальный катет
        pdy = sin(pRadian) * 2; // вертикальный катет
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        pRadian += 0.1;
        if (pRadian > 2 * PI) {
            pRadian -= 2 * PI;
        } 
        pdx = cos(pRadian) * 2;
        pdy = sin(pRadian) * 2;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { px += pdx; py += pdy; } // вперед куда смотрит игрок
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { px -= pdx; py -= pdy; } // назад 
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
    drawMap2D();
    drawPlayer();
}

void init() {
    // Цвет фона
    glClearColor(0.3, 0.3, 0.3, 0);
    //Какая часть 3-ех мерного мира видна 
    glOrtho(0, 1240, 512, 0, -1, 1);
    // задаем позицию игрока
    px = 300; 
    py = 300;
};

int main() {
    
    if (!glfwInit()) return -1;

   
  
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1240, 512, "Wolfinstain3D_YOUKNOW", nullptr, nullptr);

    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    // вертикальная сенхронизация то есть 60 кадров лок чтоб игрок не улетал из-за большого кол-во кадров
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Не удалось инициализировать GLAD\n";
        return -1;
    }

    init();

    while (!glfwWindowShouldClose(window)) {       
        display();
        move(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //glfwTerminate();
    return 0;
}