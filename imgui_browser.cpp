
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#include <ctype.h>          // toupper, isprint
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>         // intptr_t
#else
#include <stdint.h>         // intptr_t
#endif


#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#define vsnprintf _vsnprintf
#endif
#ifdef __clang__
#pragma clang diagnostic ignored "-Wold-style-cast"             // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"    // warning : 'xx' is deprecated: The POSIX name for this item.. // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"   // warning : cast to 'void *' from smaller integer type 'int'
#pragma clang diagnostic ignored "-Wformat-security"            // warning : warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors"      // warning : declaration requires an exit-time destructor       // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic ignored "-Wreserved-id-macro"          // warning : macro name is a reserved identifier                //
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"          // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat-security"              // warning : format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion"             // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"                   // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#if (__GNUC__ >= 6)
#pragma GCC diagnostic ignored "-Wmisleading-indentation"       // warning: this 'if' clause does not guard this statement      // GCC 6.0+ only. See #883 on GitHub.
#endif
#endif

// Play it nice with Windows users. Notepad in 2017 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#else
#define IM_NEWLINE "\n"
#endif






void ImGui::ShowBrowserWindow(bool* p_open, ImTextureID tex_id)
{
    // We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(680, 680), ImGuiCond_FirstUseEver);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;

    if (!ImGui::Begin("ImGui Browser", p_open, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    // Menu
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            //ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Examples"))
        {
            //ImGui::MenuItem("Main menu bar", NULL, &show_app_main_menu_bar);
            //ImGui::MenuItem("Console", NULL, &show_app_console);
            //ImGui::MenuItem("Log", NULL, &show_app_log);
            //ImGui::MenuItem("Simple layout", NULL, &show_app_layout);
            //ImGui::MenuItem("Property editor", NULL, &show_app_property_editor);
            //ImGui::MenuItem("Long text display", NULL, &show_app_long_text);
            //ImGui::MenuItem("Auto-resizing window", NULL, &show_app_auto_resize);
            //ImGui::MenuItem("Constrained-resizing window", NULL, &show_app_constrained_resize);
            //ImGui::MenuItem("Simple overlay", NULL, &show_app_simple_overlay);
            //ImGui::MenuItem("Manipulating window titles", NULL, &show_app_window_titles);
            //ImGui::MenuItem("Custom rendering", NULL, &show_app_custom_rendering);
            //ImGui::MenuItem("Documents", NULL, &show_app_documents);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            //ImGui::MenuItem("Metrics", NULL, &show_app_metrics);
            //ImGui::MenuItem("Style Editor", NULL, &show_app_style_editor);
            //ImGui::MenuItem("About Dear ImGui", NULL, &show_app_about);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::Spacing();


    static char AddressURL[500] = "http://localhost/mousetest.html";

    ImGui::Text("Address:");
    ImGui::SameLine();
    ImGui::InputText("", AddressURL, IM_ARRAYSIZE(AddressURL));


    if (tex_id != nullptr)
    {
        ImTextureID my_tex_id = tex_id;
        float my_tex_w = 800.0f;
        float my_tex_h = 600.0f;

 
        ImVec2 curpos = ImGui::GetCursorPos();
        ImVec2 winpos = ImGui::GetWindowPos();
        ImGui::ImageButton(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
        //if (ImGui::IsItemHovered())
        {
            UpdateBrowserMouse(winpos, curpos);
            // process mouse data
            // cancel all mouse event
        }



    }



    // End of ShowDemoWindow()
    ImGui::End();
}