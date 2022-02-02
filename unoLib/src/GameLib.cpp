#include <iostream>
#include <string>
#include "uno/GameLib.h"
#include "uno/Hand.h"
#include "uno/Game.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "uno/Texture.h"
#include "uno/shaderClass.h"
#include "uno/VAO.h"
#include "uno/VBO.h"
#include "uno/EBO.h"


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
    Game UNO;
    UNO.Run();    
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


const unsigned int width = 800;
const unsigned int height = 800;
// Vertices coordinates
GLfloat vertices[] = {
    // Coordinats        /    Colors          /   TexCoord
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f,  0.44f,  0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f,  0.44f,  5.0f, 0.0f,
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f,  0.44f,  0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f,  0.44f,  5.0f, 0.0f,
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f,  0.76f,  2.5f, 5.0f
};
// Indices for vertices order
GLuint indices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};


void
OpenGLTest()
{
    //
    // Refer to this video for an OpenGL tutorial.
    // https://www.youtube.com/watch?v=45MIykWJ-C4&ab_channel=freeCodeCamp.org

    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we are using.
    // In this case we are using OpenGL 3.3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    // so that means we only have the modern functions.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object of 800 by 800 pixels, naming it "UNO"
    GLFWwindow* window = glfwCreateWindow(width, height, "UNO", NULL, NULL);
    // Error check if the window fails to create.
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    // Introduce the window into the current context.
    glfwMakeContextCurrent(window);

    // Load GLAD so it configures OpenGL.
    gladLoadGL();
    // Specify the viewport of OpenGL in the Window.
    // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, width, height);


    // Generates Shader object using shaders default.vert and default.frag.
    Shader shaderProgram("../../../../unoLib/rsrc/default.vert", "../../../../unoLib/rsrc/default.frag");



    // Generates Vertex Array Object and binds it.
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices.
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices.
    EBO EBO1(indices, sizeof(indices));

    // Links VBO attributes such as coordinates and colors to VAO.
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them.
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Gets ID of uniform called "scale"
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");




    // Texture
    Texture brickTex("../../../../unoLib/rsrc/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brickTex.texUnit(shaderProgram, "tex0", 0);



    // Variables that help the rotation of the pyramid
    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);

    // Main while loop
    while (!glfwWindowShouldClose(window)) {
        // Specify the color of the background.
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clear the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use.
        shaderProgram.Activate();

        // Simple timer
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60) {
            rotation += 0.5f;
            prevTime = crntTime;
        }

        // Initializes matrices so they are not the null matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        // Assigns different transformations to each matrix
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

        // Outputs the matrices into the Vertex Shader
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
        glUniform1f(uniID, 0.5f);
        // Binds texture so that it appears in rendering.
        brickTex.Bind();
        // Bind the VAO so OpenGL knows to use it.
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices.
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer.
        glfwSwapBuffers(window);
        // Take care of all GLFW events.
        glfwPollEvents();
    }



    // Delete all the objects we have created.
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    brickTex.Delete();
    shaderProgram.Delete();
    // Delete window before ending the program.
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program.
    glfwTerminate();
}
