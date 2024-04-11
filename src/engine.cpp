#include "engine.h"

// TODO: hover effect when you are on a button

enum state {start, play, over};
state screen;

// Colors
color originalFill, hoverFill, pressFill;


Engine::Engine() : keys() {
    this->initWindow();
    this->initShaders();
    this->initShapes();

    originalFill = {1, 0, 0, 1};
    hoverFill.vec = originalFill.vec + vec4{0.5, 0.5, 0.5, 0};
    pressFill.vec = originalFill.vec - vec4{0.5, 0.5, 0.5, 0};
}

Engine::~Engine() {}

unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(width, height, "engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}


void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    // Configure text shader and renderer
    textShader = shaderManager->loadShader("../res/shaders/text.vert", "../res/shaders/text.frag", nullptr, "text");
    fontRenderer = make_unique<FontRenderer>(shaderManager->getShader("text"), "../res/fonts/MxPlus_IBM_BIOS.ttf", 24);

    // Set uniforms
    textShader.setVector2f("vertex", vec4(100, 100, .5, .5));
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes() {
    // All buttons we use
    //spawnButton = make_unique<Rect>(shapeShader, vec2{width/2,height/2}, vec2{80, 80}, color{1, 0, 0, 1});
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 - 200,height/2 + 200}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 - 100,height/2 + 200}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2,height/2 + 200}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 + 100,height/2 + 200}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 + 200,height/2 + 200}, vec2{80, 80}, color{255, 255, 0, 1}));

    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 - 200,height/2 + 100}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 - 100,height/2 + 100}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2,height/2 + 100}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 + 100,height/2 + 100}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 + 200,height/2 + 100}, vec2{80, 80}, color{255, 255, 0, 1}));

    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 - 200,height/2}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 - 100,height/2}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2,height/2}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 + 100,height/2}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 + 200,height/2}, vec2{80, 80}, color{255, 255, 0, 1}));

    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 - 200,height/2 - 100}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 - 100,height/2 - 100}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2,height/2 - 100}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 + 100,height/2 - 100}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 + 200,height/2 - 100}, vec2{80, 80}, color{255, 255, 0, 1}));

    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 - 200,height/2 - 200}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 - 100,height/2 - 200}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2,height/2 - 200}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 + 100,height/2 - 200}, vec2{80, 80}, color{255, 255, 0, 1}));
    this->buttonVec.push_back(make_unique<Rect>(shapeShader, vec2{width/2 + 200,height/2 - 200}, vec2{80, 80}, color{255, 255, 0, 1}));

}


void Engine::processInput() {
    glfwPollEvents();
    // Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            keys[key] = true;
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
            keys[key] = false;
    }

    // Close window if escape key is pressed
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

    // Mouse position saved to check for collisions
    glfwGetCursorPos(window, &MouseX, &MouseY);

    // Hint: The index is GLFW_KEY_S
    if (screen == start) {
        if (keys[GLFW_KEY_S]) {
            screen = play;
        }
    }
    // Mouse position is inverted because the origin of the window is in the top left corner
    MouseY = height - MouseY; // Invert y-axis of mouse position

    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (screen == play) {
        if (mousePressedLastFrame && !mousePressed) {
            // Function turns lights on or off
            int counter = 0;
            for (const auto& button : buttonVec) {
                if (MouseY < button->getBottom() && MouseY > button->getTop() &&
                    MouseX < button->getRight() && MouseX > button->getLeft()) {


                    turnLight(counter);
                    break;
                }
                counter++;
            }

        }
    }
    // Save mousePressed for next frame
    mousePressedLastFrame = mousePressed;
}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // TODO: Change this to end when all lights are off
    int count = 0;
    for (const unique_ptr<Shape>& b : buttonVec) {
        vec3 color = b->getColor3();
        if (color[0]== originalFill.red && color[1] == originalFill.green && color[2] == originalFill.blue){
            count++;
        }
        if (buttonVec.size() <= count) {
            screen = over;
        }
    }
}

void Engine::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Set shader to use for all shapes
    shapeShader.use();

    // Render differently depending on screen
    switch (screen){
        case start: {
            string title = "Lights Out";
            // displayed at top of screen
            this->fontRenderer->renderText(title, width/2 - (10 * title.length()), height/8, 1, vec3{1, 1, 1});

            // Instructions
            this->fontRenderer->renderText(title, width/2 - (10 * title.length()), height/8, 1, vec3{1, 1, 1});

            // Each instruction
            string sentence1 = "Click on a cell to toggle that cell and all of its immediate neighbors.";
            string sentence2 = "Click again to untoggle.";
            string sentence3 = "The goal of this game is to switch off all the lights with the least number of clicks.";

            // Positioning
            this->fontRenderer->renderText(sentence1, width/2 - (5 * title.length()), height/2 - 50, 0.7, vec3{1, 1, 1});
            this->fontRenderer->renderText(sentence2, width/2 - (5 * title.length()), height/2, 0.7, vec3{1, 1, 1});
            this->fontRenderer->renderText(sentence3, width/2 - (5 * title.length()), height/2 + 50, 0.7, vec3{1, 1, 1});

            string message = "Press s to start";
            // (12 * message.length()) is the offset to center text.
            // 12 pixels is the width of each character scaled by 1.
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/4, 1, vec3{1, 1, 1});
            break;

        }
        case play: {
            for (const unique_ptr<Shape>& b : buttonVec) {

                b->setUniforms();
                b->draw();
              }


            break;
        }
        case over: {
            string message = "You win!";
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, 1, vec3{1, 1, 1});
            break;
        }
    }

    glfwSwapBuffers(window);
}

// TODO: Function changes color of button and affects the other buttons
void Engine::turnLight(int saved) {
    // Change color of initial button
    checkLight(saved);

    //Check LHS
    if (saved == 0 || saved == 5 || saved == 10 || saved == 15 || saved == 20) {
        if (saved > 4) {
            checkLight(saved - 5);
        }
        if (saved < 19) {
            checkLight(saved+5);
        }
        checkLight(saved+1);
    }

    //Check RHS
    else if (saved == 4 || saved == 9 || saved == 14 || saved == 19 || saved == 24) {
        if (saved > 4) {
            checkLight(saved - 5);
        }
        if (saved < 19) {
            checkLight(saved+5);
        }
        checkLight(saved-1);
    }

        //Check all else
    else {
        if (saved > 4) {
            checkLight(saved-5);
            if (saved < 19) {
                checkLight(saved+5);
            }
        }
        checkLight(saved-1);
        checkLight(saved+1);
    }
}

void Engine::checkLight(int saved) {
    if (buttonVec[saved]->getRed() == 1 && buttonVec[saved]->getBlue() == 1 && buttonVec[saved]->getGreen() == 1) {
        //If the original color, set to "lit" color
        //buttonVec[saved]->setColor(pressFill);
        buttonVec[saved]->setRed(1);
        buttonVec[saved]->setGreen(1);
        buttonVec[saved]->setBlue(0);
    }
    else {
        //If not, set back to original color
        buttonVec[saved]->setRed(1);
        buttonVec[saved]->setGreen(1);
        buttonVec[saved]->setBlue(1);
    }
}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}

GLenum Engine::glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        cout << error << " | " << file << " (" << line << ")" << endl;
    }
    return errorCode;
}