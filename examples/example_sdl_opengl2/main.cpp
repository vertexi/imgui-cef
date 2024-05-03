// dear imgui: standalone example application for SDL2 + OpenGL
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_sdl_opengl3/ folder**
// See imgui_impl_sdl.cpp for details.

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include <SDL_hints.h>
#include <SDL_video.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <cef_app.h>

#include "clip.h"
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <zmq.hpp>

static ImGuiWindowFlags WindowFlagsNothing()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    return window_flags;
}

ImVec2 ImageProportionalSize(const ImVec2& askedSize, const ImVec2& imageSize)
{
    ImVec2 r(askedSize);

    if ((r.x == 0.f) && (r.y == 0.f))
        r = imageSize;
    else if (r.y == 0.f)
        r.y = imageSize.y / imageSize.x * r.x;
    else if (r.x == 0.f)
        r.x = imageSize.x / imageSize.y * r.y;
    return r;
}

GLuint pic_tex_id = 0;
void impl_StoreTexture(int width, int height, unsigned char* image_data_rgba, int image_type)
{
    // it's ok to just delete 0's texture
    glDeleteTextures(1, &pic_tex_id);

    glGenTextures(1, &pic_tex_id);
    glBindTexture(GL_TEXTURE_2D, pic_tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#if defined(HELLOIMGUI_USE_GLES2) || defined(HELLOIMGUI_USE_GLES3)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                    width,
                    height, 0, image_type, GL_UNSIGNED_BYTE, image_data_rgba);
}

void png_copy(void *socket_, void *data, int size) {
    zmq::message_t request (size);
    memcpy(request.data(), data, size);
    std::cout << "Sending Image " << std::endl;
    zmq::socket_t *socket = (zmq::socket_t *)socket_;
    socket->send (request, zmq::send_flags::none);
}

// Main code
int main(int argc, char** argv)
{
    // Setup CEF
    int cefResult = ImGui_ImplSDL2_CefInit(argc, argv);
    if (cefResult >= 0)
        return cefResult;

    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    bool show_in_game_browser_window = true;
    // Our state
    // bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, zmq::socket_type::req);
    zmq::socket_t control_socket (context, zmq::socket_type::req);

    std::cout << "Connecting to hello world server..." << std::endl;
    socket.connect ("tcp://localhost:8848");
    control_socket.connect ("tcp://localhost:8849");

    zmq_pollitem_t items[] = {
        {socket,  0,  ZMQ_POLLIN,     0},
    };

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }
        zmq_poll(items, IM_ARRAYSIZE(items), 0);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        int windowW, windowH;
        SDL_GetWindowSize(window, &windowW, &windowH);
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(windowW, windowH));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        static bool p_open = true;
        std::string windowTitle = "Main window (title bar invisible)";
        ImGui::Begin(windowTitle.c_str(), &p_open, WindowFlagsNothing());
        ImGui::PopStyleVar(3);

        // // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // if (show_demo_window)
        //     ImGui::ShowDemoWindow(&show_demo_window);

        {
            static std::string value;
            static clip::image image;
            static bool update_image = false;
            static bool wait_reply = false;
            if (ImGui::Button("Show clipboard"))
            {
                if (!clip::get_text(value))
                {
                    value = "Could not get clipboard text";
                    if (!clip::get_image(image)) {
                        value = "Could not get clipboard image";
                    } else {
                        update_image = true;
                        wait_reply = true;
                        auto imageDataSize = image.spec().required_data_size();
                        auto imageData = image.data();
                        IM_ASSERT(image.spec().bits_per_pixel / 8 == 4);
                        for (unsigned int i = 0; i < imageDataSize / 4; i++)
                        {
                            std::swap(imageData[i * 4 + 0], imageData[i * 4 + 2]);
                        }
                        stbi_write_png_to_func(png_copy, &socket, image.spec().width, image.spec().height, 4, (void *)imageData, image.spec().bytes_per_row);
                    }
                }
            }
            ImGui::Text("%s", value.c_str());
            static ImVec2 imageDispSize;
            if (update_image) {
                impl_StoreTexture(image.spec().width, image.spec().height, (unsigned char *)image.data(), GL_RGBA);
                auto imageSize = ImVec2(image.spec().width, image.spec().height);
                imageDispSize = ImageProportionalSize(ImVec2(image.spec().width, image.spec().height), imageSize);
                update_image = false;
            }
            if (wait_reply && items[0].revents & ZMQ_POLLIN) {
                zmq::message_t reply;
                auto size = socket.recv (reply, zmq::recv_flags::none);
                if(size != -1)
                {
                    std::cout << "receive ok." << std::endl;
                    std::cout << reply.to_string() << std::endl;
                    ImGui::setTexString(reply.to_string().c_str());
                }
                wait_reply = false;
            }
            if (imageDispSize.x != 0)
            {
                ImGui::Image((void *)(intptr_t)pic_tex_id, imageDispSize);
            }
            if (ImGui::Button("Set clipboard"))
            {
                clip::set_text("Hello clipboard!");
            }
        }

        ImGui::ShowBrowserWindow(&show_in_game_browser_window, ImGui_ImplSDL2_GetCefTexture());
        ImGui::End();

        // // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        // {
        //     static float f = 0.0f;
        //     static int counter = 0;

        //     ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        //     ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

        //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //     ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        //     if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //         counter++;
        //     ImGui::SameLine();
        //     ImGui::Text("counter = %d", counter);

        //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //     ImGui::End();
        // }


        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    zmq::message_t request (10);
    memcpy(request.data(), "quit", 10);
    control_socket.send(request);

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    CefShutdown();

    socket.close();
    control_socket.close();
    return 0;
}
