#include <iostream>
#include <string>
#include "uno/GameLib.h"
#include "uno/Hand.h"
#include "uno/Game.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"


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

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\0";

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
    // Vertices coordinates
    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
    };
    // Create a GLFWwindow object of 800 by 800 pixels, naming it "UNO"
    GLFWwindow* window = glfwCreateWindow(800, 800, "UNO", NULL, NULL);
    // Error check if the window fails to create.
    if (window == NULL) {
        std::cout << "Failed to create a GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    // Introduce the window into the current context.
    glfwMakeContextCurrent(window);
    // Load GLAD so it configures OpenGL.
    gladLoadGL();
    // Specify the viewport of OpenGL in the Window.
    glViewport(0, 0, 800, 800);
    // Create Vertex Shader Object and get its reference.
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the Vertex Shader Object.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compile the Vertex Shader into machine code.
    glCompileShader(vertexShader);

    // Create Fragment Shader Object and get its referece.
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the Fragment Shader Object.
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compile the Fragment Shader into machine code.
    glCompileShader(fragmentShader);

    // Create Shader Program Object and get its reference.
    GLuint shaderProgram = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program.
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Link all the shaders together into the Shader Program.
    glLinkProgram(shaderProgram);
    // Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // Create reference containers for the Vertex Array Object and the Vertex Buffer Object.
    GLuint VAO, VBO;
    // Generate the VAO and VBO with only 1 object each.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Make the VAO the current Vertex Array Object by binding it.
    glBindVertexArray(VAO);
    // Bind the VBO, specifically, it's a GL_ARRAY_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Introduce the vertices into the VBO.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable the Vertex Attribute so that OpenGL knows how to use it.
    glEnableVertexAttribArray(0);
    // Bind both the VBO and VAO to 0 so that we don't accidentally modify them.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Specify the color of the background.
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clear the back buffer and assign the new color to it.
    glClear(GL_COLOR_BUFFER_BIT);
    // Swap the back buffer with the front buffer.
    glfwSwapBuffers(window);
    // Main while loop.
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use.
        glUseProgram(shaderProgram);
        // Bind the VAO so OpenGL knows how to use it.
        glBindVertexArray(VAO);
        // Draw the triangle using the GL_TRIANGLES primitive.
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        // Take care of all GLFW events.
        glfwPollEvents();
    }

    // Delete all the objects we have created.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Clean up.
    glfwDestroyWindow(window);
    glfwTerminate();
}
