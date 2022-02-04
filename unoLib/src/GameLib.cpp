#include <iostream>
#include <string>
#include "uno/GameLib.h"
#include "uno/Hand.h"
#include "uno/Game.h"



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "uno/Game.h"
#include "uno/ResourceManager.h"


//
// Headers for getch() implementation
#ifdef _WIN32
#include "conio.h"
#elif defined __APPLE__
#include <unistd.h>
#include <termios.h>
// #include <ncurses.h>
#else
// DEFINE YOUR OS!
#endif


void gameLoop()
{
    //Game UNO;
    //UNO.Run();    
}

void ClearScreen()
{
    //std::cout << "\033[2J\033[H";
    std::cout << "\033c";
}

void PrintBorder()
{
    size_t nBorderLen = 100;
    std::string strStarRow(nBorderLen, '*');
    ColorString(false, Card::Color::Yellow, strStarRow);
    std::string strSlashRow(nBorderLen, '/');
    ColorString(true, Card::Color::Red, strSlashRow);
    std::cout << strStarRow << '\n' << strSlashRow << '\n' << strStarRow << '\n';
}

int GetCharInput()
{
    //
    // Used to interact with the console/terminal.
    // Implements getch() on windows/linux.
    // Refer to https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
    // for preprocessor directives.
#ifdef _WIN32
    return _getch();
#elif defined __APPLE__
    //
    // Refer to https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux
    int buf = 0;
    struct termios old = { 0 };
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 4) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    if (buf & 0xffffff00) //if more than one key was input
        buf >>= 16; // ignore first 2 inputs
    return buf;
#endif
}

void IncrementIndex(bool bUp, size_t nSize, size_t& nCurrentIndex)
{
    if (nSize == 0)
        return;
    //
    // Selects the next value by either moving up or down.
    if (bUp) {
        ++nCurrentIndex;
        if (nCurrentIndex == nSize)
            nCurrentIndex = 0;
    }
    else {
        if (nCurrentIndex == 0)
            nCurrentIndex = nSize;
        --nCurrentIndex;
    }
}

void FeatureTest()
{
    PrintBorder();
    std::cout << "\nFEATURE TEST\n\n";

    Hand Deck;
    Deck.SetFullDeck();
    //
    // Before shuffling.
    Deck.Display();
    //
    // After shuffling.
    Deck.Shuffle();
    Deck.Display();

    std::cout << "\n\n";
    PrintBorder();
    std::cout << "Press enter to continue...";
    std::string s;
    std::getline(std::cin, s);

    ClearScreen();
}

void KeyInputTest()
{
    //
    // You may need to tweak the GetCharInput() method
    // for your system.
    while (true) {
        char cInput = GetCharInput();
        std::cout << (int)cInput << '\n';
    }
}

void
ColorString(bool bSelected, Card::Color eColor, std::string& Text)
{
    //
    // Colors the given string with the given color. If the string is selected the colors (bg/fg) are reversed.
    //
    // Refer to https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html
    const std::string EscapeSeq = "\u001b[";
    const char cResetFont = '0';
    const char cBoldFont = '1';
    const char cFaintFont = '2';
    const char cUnderlineFont = '4';
    const char cSlowBlinkFont = '5';
    const char cForeground = '3';
    const char cBackground = '4';
    const char cBlack = '0';
    const char cRed = '1';
    const char cGreen = '2';
    const char cYellow = '3';
    const char cBlue = '4';
    const char cMagenta = '5';
    const char cCyan = '6';
    const char cWhite = '7';
    const char cDelimeter = ';';
    const char cFinalByte = 'm';
    //
    // Printing Flags
    const bool bBold = false;
    const bool bBackground = bSelected;

    char cTextColor;
    switch (eColor) {
    case Card::Color::Red:
        cTextColor = cRed;
        break;
    case Card::Color::Yellow:
        cTextColor = cYellow;
        break;
    case Card::Color::Green:
        cTextColor = cGreen;
        break;
    case Card::Color::Blue:
        cTextColor = cBlue;
        break;
    case Card::Color::Wild:
        cTextColor = cMagenta;
        break;
    case Card::Color::Cyan:
        cTextColor = cCyan;
        break;
    case Card::Color::White:
        cTextColor = cWhite;
        break;
    default:
        cTextColor = cWhite;
    }

    std::string NewText = EscapeSeq;
    if (bBold) {
        NewText += cBoldFont;
        NewText += cDelimeter;
    }
    if (bBackground) {
        NewText += cBackground;
        NewText += cTextColor;
        NewText += cDelimeter;
        NewText += cForeground;
        NewText += cTextColor == cBlack ? cWhite : cBlack;
        NewText += cFinalByte;
    }
    else {
        NewText += cForeground;
        NewText += cTextColor;
        NewText += cFinalByte;
    }

    NewText += Text;
    NewText += EscapeSeq;
    NewText += cResetFont;
    NewText += cFinalByte;

    std::swap(NewText, Text);
}

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/


// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

// GLFW callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            Breakout.Keys[key] = false;
    }
}

void OpenGLTest()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize game
    // ---------------
    Breakout.Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // manage user input
        // -----------------
        Breakout.ProcessInput(deltaTime);

        // update game state
        // -----------------
        Breakout.Update(deltaTime);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.Render();

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

    glfwTerminate();
}

