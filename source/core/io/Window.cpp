#include "Window.h"

#include "FileIO.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/common.hpp>
#include <iostream>


#include "patterns/singleton/EventDispatcher.h"
#include "patterns/events/ExportFileEvent.h"
#include "patterns/events/RenderModeEvent.h"
#include "patterns/events/TimerEndEvent.h"
#include "patterns/events/KeyboardEvent.h"
#include "patterns/events/LoadFileEvent.h"
#include "patterns/events/MouseDragEvent.h"
#include "patterns/events/MouseScrollEvent.h"


namespace n2m::io {
Window::~Window () {
    shutdown ();
}

void Window::framebuffer_size_callback (GLFWwindow* window,
    int width,
    int height) {
    glViewport (0, 0, width, height);
}

void Window::scroll_callback (GLFWwindow* window,
    double xoffset,
    double yoffset) {
    // Retrieve the Window object associated with this GLFWwindow
    auto* win = static_cast<Window*> (glfwGetWindowUserPointer (window));

    if (!win) return;

    MouseScrollEvent mouse_scroll_event (xoffset, yoffset);
    EventDispatcher::Instance ().publish (mouse_scroll_event);
}

// Mouse button callback
void Window::mouse_button_callback (GLFWwindow* window,
    int button,
    int action,
    int mods) {
    // Retrieve the Window object associated with this GLFWwindow
    auto* win = static_cast<Window*> (glfwGetWindowUserPointer (window));

    if (!win) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glfwGetCursorPos (window, &win->lastMouseX, &win->lastMouseY);

            // MouseDragEvent mouse_drag_event (win->lastMouseX, win->lastMouseY);
            // EventDispatcher::Instance ().publish (mouse_drag_event);
        }
    }
}

// Mouse movement callback
void Window::mouse_callback (GLFWwindow* window,
    double xpos,
    double ypos) {
    if (firstMouse) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos;
    lastMouseX    = xpos;
    lastMouseY    = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;


    MouseDragEvent mouse_drag_event (xoffset, yoffset);
    EventDispatcher::Instance ().publish (mouse_drag_event);
}

bool Window::init_imgui () {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION ();
    ImGui::CreateContext ();
    io = &ImGui::GetIO ();
    (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark ();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL (window, true);

    const char* glsl_version = "#version 150";
    ImGui_ImplOpenGL3_Init (glsl_version);
    return true;
}

void
Window::static_mouse_callback (GLFWwindow* window, double xpos, double ypos) {
    auto* win = static_cast<Window*> (glfwGetWindowUserPointer (window));
    if (win) {
        win->mouse_callback (window, xpos, ypos);
    }
}

bool Window::init (int width, int height, const std::string& title) {
    // Initialize GLFW
    if (!glfwInit ()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create window
    window =
        glfwCreateWindow (width, height, title.c_str (), NULL,NULL);

    if (!window) {
        std::cerr << "Failed to create GLFW window.\n";
        glfwTerminate ();
        return false;
    }

    glfwMakeContextCurrent (window);

    glfwSetWindowUserPointer (window, this);

    // Callbacks
    glfwSetFramebufferSizeCallback (window, framebuffer_size_callback);
    glfwSetMouseButtonCallback (window, mouse_button_callback);
    glfwSetCursorPosCallback (window, &Window::static_mouse_callback);
    glfwSetScrollCallback (window, scroll_callback);

    // init imgui
    this->init_imgui ();

    // Hide the cursor
    glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL via glad
    if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD.\n";
        return false;
    }

    // Set a default viewport, enable some GL states if you want
    glViewport (0, 0, width, height);
    glEnable (GL_DEPTH_TEST);

    // Sub to events
    EventDispatcher::Instance ().subscribe<SceneStateEvent> (
        [this](SceneStateEvent event) {
            this->sceneMetrics = event.metrics; // update scene metrics
        });

    EventDispatcher::Instance ().subscribe<TimerEndEvent> (
        [this](TimerEndEvent event) {
            this->executionTime = event.duration; // update scene metrics
        });
    return true;
}

void Window::shutdown () {
    if (window) {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown ();
        ImGui_ImplGlfw_Shutdown ();
        ImGui::DestroyContext ();

        glfwDestroyWindow (window);
        window = nullptr;
        glfwTerminate ();
    }
}

bool Window::pollEvents () const {
    glfwPollEvents ();

    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    if (glfwGetWindowAttrib (window, GLFW_ICONIFIED) != 0) {
        ImGui_ImplGlfw_Sleep (10);
        return false;
    }

    return true;
}

void Window::render_imgui () const {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame ();
    ImGui_ImplGlfw_NewFrame ();
    ImGui::NewFrame ();

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    static float f     = 0.0f;
    static int counter = 0;

    // Create a window and append into it.
    ImGui::Begin ("Settings!");

    if (ImGui::BeginMenu ("File")) {
        if (ImGui::MenuItem ("Open..", "Ctrl+O")) {
        }
        if (ImGui::MenuItem ("Export as OBJ")) {
            ExportFileEvent export_file_event (
                "resources/models/exported_mesh.obj",
                ExportFormat::OBJ);

            EventDispatcher::Instance ().publish (export_file_event);
        }

        if (ImGui::MenuItem ("Export as STL")) {
            ExportFileEvent export_file_event (
                "resources/models/exported_mesh.stl",
                ExportFormat::STL);

            EventDispatcher::Instance ().publish (export_file_event);
        }

        if (ImGui::MenuItem ("Close", "Ctrl+W")) {
        }
        ImGui::EndMenu ();
    }

    ImGui::Separator ();


    ImGui::Separator ();
    ImGui::SeparatorText ("Vizualization mode");
    const char* items[]             = {"Solid", "Wireframe"};
    static const char* current_item = items[0];

    //RenderModeEvent render_mode_event(RenderModeEvent::renderMode::Solid);
    for (int n = 0; n < IM_ARRAYSIZE (items); n++) {
        bool is_selected = (current_item == items[n]);
        // You can store your selection however you want, outside or inside your objects
        if (ImGui::Selectable (items[n], is_selected)) {
            current_item = items[n];
            RenderModeEvent render_mode_event (current_item);
            EventDispatcher::Instance ().publish (render_mode_event);
        }
    }

    ImGui::SeparatorText ("Animation frame");
    ImGui::SliderInt("frame", &graphics::Scene::t, 0, 500);

    ImGui::SeparatorText ("Statistics");
    ImGui::Text ("Number of vertex %d", this->sceneMetrics.vertexCount);
    ImGui::Text ("Number of triangles %d", this->sceneMetrics.triangleCount);

    ImGui::Text ("Average %.3f ms/frame (%.1f FPS)",
        1000.0f / io->Framerate, io->Framerate);


    ImGui::Separator ();

    ImGui::End ();
}

void Window::render () const {
    if (!window) return;

    // Render imgui
    render_imgui ();

    // Rendering
    ImGui::Render ();
    int display_w, display_h;
    glfwGetFramebufferSize (window, &display_w, &display_h);
    glViewport (0, 0, display_w, display_h);
    // glClearColor (clear_color.x * clear_color.w, clear_color.y * clear_color.w,
    //     clear_color.z * clear_color.w, clear_color.w);
    // glClear (GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData ());

    processInput ();
    glfwSwapBuffers (window);
}

void Window::processInput () const {
    // Array of keys to monitor
    const std::vector<int> keys = {GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S,
                                   GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_BACKSPACE};

    float deltaTime = getDeltaTime ();

    // Loop through keys and check their state
    for (int key : keys) {
        if (glfwGetKey (window, key) == GLFW_PRESS) {
            EventDispatcher::Instance ().publish (
                KeyboardEvent (key, KeyAction::PRESS, deltaTime));
        } else if (glfwGetKey (window, key) == GLFW_RELEASE) {
            EventDispatcher::Instance ().publish (
                KeyboardEvent (key, KeyAction::RELEASE, deltaTime));
        }
    }
}

float Window::getDeltaTime () const {
    static float lastTime = 0.0f; // Keep track of the last frame time
    float currentTime     = static_cast<float> (glfwGetTime ());
    float deltaTime       = currentTime - lastTime;
    lastTime              = currentTime;
    return deltaTime;
}

bool Window::shouldClose () const {
    return (window && glfwWindowShouldClose (window));
}
}

