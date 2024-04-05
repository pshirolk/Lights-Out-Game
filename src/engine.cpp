#include "engine.h"

enum state {start, play, over};
state screen;

// Colors
color originalFill, hoverFill, pressFill;
int countConfetti = 1;


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
    // TODO: Make all "Buttons" here
    // TODO: They will be our lights, and will be affected by other buttons being pressed. Use the size of 80x80
    // red spawn button centered in the top left corner
    spawnButton = make_unique<Rect>(shapeShader, vec2{width/2,height/2}, vec2{80, 80}, color{1, 0, 0, 1});

    button1 = make_unique<Rect>(shapeShader, vec2{width/2 - 200,height/2 + 200}, vec2{80, 80}, color{1, 0, 0, 1});
    button2 = make_unique<Rect>(shapeShader, vec2{width/2 - 100,height/2 + 200}, vec2{80, 80}, color{1, 0, 0, 1});
    button3 = make_unique<Rect>(shapeShader, vec2{width/2,height/2 + 200}, vec2{80, 80}, color{1, 0, 0, 1});
    button4 = make_unique<Rect>(shapeShader, vec2{width/2 + 100,height/2 + 200}, vec2{80, 80}, color{1, 0, 0, 1});
    button5 = make_unique<Rect>(shapeShader, vec2{width/2 + 200,height/2 + 200}, vec2{80, 80}, color{1, 0, 0, 1});

    button6 = make_unique<Rect>(shapeShader, vec2{width/2 - 200,height/2 + 100}, vec2{80, 80}, color{1, 0, 0, 1});
    button7 = make_unique<Rect>(shapeShader, vec2{width/2 - 100,height/2 + 100}, vec2{80, 80}, color{1, 0, 0, 1});
    button8 = make_unique<Rect>(shapeShader, vec2{width/2,height/2 + 100}, vec2{80, 80}, color{1, 0, 0, 1});
    button9 = make_unique<Rect>(shapeShader, vec2{width/2 + 100,height/2 + 100}, vec2{80, 80}, color{1, 0, 0, 1});
    button10 = make_unique<Rect>(shapeShader, vec2{width/2 + 200,height/2 + 100}, vec2{80, 80}, color{1, 0, 0, 1});

    button11 = make_unique<Rect>(shapeShader, vec2{width/2 - 200,height/2}, vec2{80, 80}, color{1, 0, 0, 1});
    button12 = make_unique<Rect>(shapeShader, vec2{width/2 - 100,height/2}, vec2{80, 80}, color{1, 0, 0, 1});
    button13 = make_unique<Rect>(shapeShader, vec2{width/2,height/2}, vec2{80, 80}, color{1, 0, 0, 1});
    button14 = make_unique<Rect>(shapeShader, vec2{width/2 + 100,height/2}, vec2{80, 80}, color{1, 0, 0, 1});
    button15 = make_unique<Rect>(shapeShader, vec2{width/2 + 200,height/2}, vec2{80, 80}, color{1, 0, 0, 1});

    button16 = make_unique<Rect>(shapeShader, vec2{width/2 - 200,height/2 - 100}, vec2{80, 80}, color{1, 0, 0, 1});
    button17 = make_unique<Rect>(shapeShader, vec2{width/2 - 100,height/2 - 100}, vec2{80, 80}, color{1, 0, 0, 1});
    button18 = make_unique<Rect>(shapeShader, vec2{width/2,height/2 - 100}, vec2{80, 80}, color{1, 0, 0, 1});
    button19 = make_unique<Rect>(shapeShader, vec2{width/2 + 100,height/2 - 100}, vec2{80, 80}, color{1, 0, 0, 1});
    button20 = make_unique<Rect>(shapeShader, vec2{width/2 + 200,height/2 - 100}, vec2{80, 80}, color{1, 0, 0, 1});

    button21 = make_unique<Rect>(shapeShader, vec2{width/2 - 200,height/2 - 200}, vec2{80, 80}, color{1, 0, 0, 1});
    button22 = make_unique<Rect>(shapeShader, vec2{width/2 - 100,height/2 - 200}, vec2{80, 80}, color{1, 0, 0, 1});
    button23 = make_unique<Rect>(shapeShader, vec2{width/2,height/2 - 200}, vec2{80, 80}, color{1, 0, 0, 1});
    button24 = make_unique<Rect>(shapeShader, vec2{width/2 + 100,height/2 - 200}, vec2{80, 80}, color{1, 0, 0, 1});
    button25 = make_unique<Rect>(shapeShader, vec2{width/2 + 200,height/2 - 200}, vec2{80, 80}, color{1, 0, 0, 1});

    //vector<unique_ptr<Shape>> buttonVec;
    //buttonVec.push_back(button1); // {button1, button2, button3, button4, button5, };
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
    // Hint: one of the indices is GLFW_KEY_UP
    if (screen == play) {
        // TODO: Remove moving the initial button
        if (keys[GLFW_KEY_RIGHT]) {
            spawnButton->moveX(5);
        }
        if (keys[GLFW_KEY_LEFT]) {
            spawnButton->moveX(-5);
        }
        if (keys[GLFW_KEY_DOWN]) {
            spawnButton->moveY(-5);
        }
        if (keys[GLFW_KEY_UP]) {
            spawnButton->moveY(5);
        }


        if (spawnButton->getLeft() <= 0) {
            spawnButton->moveX(5);
        }
        if (spawnButton->getRight() >= width) {
            spawnButton->moveX(-5);
        }
        if (spawnButton->getBottom() >= height) {
            spawnButton->moveY(-5);
        }
        if (spawnButton->getTop() <= 0) {
            spawnButton->moveY(5);
        }
    }
    // Mouse position is inverted because the origin of the window is in the top left corner
    MouseY = height - MouseY; // Invert y-axis of mouse position
    // TODO: Remove "overlapping" function, we only need if a mouse is pressed
    //bool buttonOverlapsMouse = spawnButton->isOverlapping(vec2(MouseX, MouseY));
    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    // TODO: We can use this following code to change the lights to on or off
    if (screen == play) {
        if (MouseY < spawnButton->getBottom() && MouseY > spawnButton->getTop() && MouseX < spawnButton->getRight() && MouseX > spawnButton->getLeft()) {
            if (mousePressed) {
                spawnButton->setColor(pressFill);
            }
        }
        if (MouseY < button1->getBottom() && MouseY > button1->getTop() && MouseX < button1->getRight() && MouseX > button1->getLeft()) {
            if (mousePressed) {
                button1->setColor(pressFill);
            }
        }

        if (mousePressedLastFrame && !mousePressed) {
            // TODO: Have function here that turns lights on or off
            spawnConfetti();
        }

        if (!mousePressed) { // !buttonOverlapsMouse
            spawnButton->setColor(originalFill);
            button1->setColor(originalFill);
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

    // If the size of the confetti vector reaches 100, change screen to over
    // TODO: Change this to end when all lights are off
    if (confetti.size() == 100)
        screen = over;
}

void Engine::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Set shader to use for all shapes
    shapeShader.use();

    // Render differently depending on screen
    switch (screen) {
        case start: {
            string message = "Press s to start";
            // (12 * message.length()) is the offset to center text.
            // 12 pixels is the width of each character scaled by 1.
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, 1, vec3{1, 1, 1});
            break;
        }
        case play: {
            // Render font on top of spawn button
            for (const unique_ptr<Shape>& c : confetti) {
                c->setUniforms();
                c->draw();
            }

            spawnButton->setUniforms();
            spawnButton->draw();

            // TODO: Initialize all buttons
            button1->setUniforms();
            button1->draw();
            button2->setUniforms();
            button2->draw();
            button3->setUniforms();
            button3->draw();
            button4->setUniforms();
            button4->draw();
            button5->setUniforms();
            button5->draw();
            button6->setUniforms();
            button6->draw();
            button7->setUniforms();
            button7->draw();
            button8->setUniforms();
            button8->draw();
            button9->setUniforms();
            button9->draw();
            button10->setUniforms();
            button10->draw();
            button11->setUniforms();
            button11->draw();
            button12->setUniforms();
            button12->draw();
            button13->setUniforms();
            button13->draw();
            button14->setUniforms();
            button14->draw();
            button15->setUniforms();
            button15->draw();
            button16->setUniforms();
            button16->draw();
            button17->setUniforms();
            button17->draw();
            button18->setUniforms();
            button18->draw();
            button19->setUniforms();
            button19->draw();
            button20->setUniforms();
            button20->draw();
            button21->setUniforms();
            button21->draw();
            button22->setUniforms();
            button22->draw();
            button23->setUniforms();
            button23->draw();
            button24->setUniforms();
            button24->draw();
            button25->setUniforms();
            button25->draw();

            // TODO: Remove naming the button
            this->fontRenderer->renderText("Spawn", spawnButton->getPos().x - 30, spawnButton->getPos().y - 5, 0.5, vec3{1, 1, 1});

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

// TODO: CHANGE this confetti function, use it to affect the other buttons
void Engine::spawnConfetti() {
    vec2 pos = {rand() % (int)width, rand() % (int)height};

    vec2 size = {countConfetti, countConfetti};
    countConfetti++;

    color color = {float(rand() % 10 / 10.0), float(rand() % 10 / 10.0), float(rand() % 10 / 10.0), 1.0f};
    confetti.push_back(make_unique<Rect>(shapeShader, pos, size, color));
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