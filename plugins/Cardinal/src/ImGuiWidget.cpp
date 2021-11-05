/*
 * Dear ImGui for DPF, converted to VCV
 * Copyright (C) 2021 Filipe Coelho <falktx@falktx.com>
 * Copyright (C) 2021 Jean Pierre Cimalando <jp-dev@inbox.ru>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "ImGuiWidget.hpp"

#ifndef DGL_NO_SHARED_RESOURCES
# include "../../../dpf/dgl/src/Resources.hpp"
#endif

#include "DearImGui/imgui.h"
#include "DearImGui/imgui_impl_opengl2.h"

struct ImGuiWidget::PrivateData {
    ImGuiContext* context;

    PrivateData(const float width, const float height, const double scaleFactor = 1.0)
    {
        IMGUI_CHECKVERSION();
        context = ImGui::CreateContext();
        ImGui::SetCurrentContext(context);

        ImGuiIO& io(ImGui::GetIO());
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.DisplaySize.x = width;
        io.DisplaySize.y = height;
        // not needed, we handle this ourselves
        // io.DisplayFramebufferScale = ImVec2(scaleFactor, scaleFactor);
        io.IniFilename = nullptr;

        ImGuiStyle& style(ImGui::GetStyle());
        style.ScaleAllSizes(scaleFactor);

#ifndef DGL_NO_SHARED_RESOURCES
        using namespace dpf_resources;
        ImFontConfig fc;
        fc.FontDataOwnedByAtlas = false;
        fc.OversampleH = 1;
        fc.OversampleV = 1;
        fc.PixelSnapH = true;
        io.Fonts->AddFontFromMemoryTTF((void*)dejavusans_ttf, dejavusans_ttf_size, 13.0f * scaleFactor, &fc);
        io.Fonts->Build();
#endif

        io.KeyMap[ImGuiKey_Tab] = '\t';
        /*
        io.KeyMap[ImGuiKey_LeftArrow] = 0xff + kKeyLeft - kKeyF1;
        io.KeyMap[ImGuiKey_RightArrow] = 0xff + kKeyRight - kKeyF1;
        io.KeyMap[ImGuiKey_UpArrow] = 0xff + kKeyUp - kKeyF1;
        io.KeyMap[ImGuiKey_DownArrow] = 0xff + kKeyDown - kKeyF1;
        io.KeyMap[ImGuiKey_PageUp] = 0xff + kKeyPageUp - kKeyF1;
        io.KeyMap[ImGuiKey_PageDown] = 0xff + kKeyPageDown - kKeyF1;
        io.KeyMap[ImGuiKey_Home] = 0xff + kKeyHome - kKeyF1;
        io.KeyMap[ImGuiKey_End] = 0xff + kKeyEnd - kKeyF1;
        io.KeyMap[ImGuiKey_Insert] = 0xff + kKeyInsert - kKeyF1;
        io.KeyMap[ImGuiKey_Delete] = kKeyDelete;
        io.KeyMap[ImGuiKey_Backspace] = kKeyBackspace;
        */
        io.KeyMap[ImGuiKey_Space] = ' ';
        io.KeyMap[ImGuiKey_Enter] = '\r';
        /*
        io.KeyMap[ImGuiKey_Escape] = kKeyEscape;
        // io.KeyMap[ImGuiKey_KeyPadEnter] = '\n';
        */
        io.KeyMap[ImGuiKey_A] = 'a';
        io.KeyMap[ImGuiKey_C] = 'c';
        io.KeyMap[ImGuiKey_V] = 'v';
        io.KeyMap[ImGuiKey_X] = 'x';
        io.KeyMap[ImGuiKey_Y] = 'y';
        io.KeyMap[ImGuiKey_Z] = 'z';

        ImGui_ImplOpenGL2_Init();
    }

    ~PrivateData()
    {
        ImGui::SetCurrentContext(context);
        ImGui_ImplOpenGL2_Shutdown();
        ImGui::DestroyContext(context);
    }
};

ImGuiWidget::ImGuiWidget(const float width, const float height)
    : imData(new PrivateData(width, height))
{
}

ImGuiWidget::~ImGuiWidget()
{
    delete imData;
}

void ImGuiWidget::drawFramebuffer()
{
//     const math::Vec fbSize = getFramebufferSize();
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     glOrtho(0.0, fbSize.x, fbSize.y, 0.0, 0.0, 1.0);
//     glViewport(0.0, 0.0, fbSize.x, fbSize.y);
//     glMatrixMode(GL_MODELVIEW);
//     glLoadIdentity();
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    ImGui::SetCurrentContext(imData->context);

    ImGui_ImplOpenGL2_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(box.size.x, box.size.y));
    ImGui::ShowDemoWindow();

    ImGui::Render();

    if (ImDrawData* const data = ImGui::GetDrawData())
    {
        // data->DisplayPos.x = -imData->getDisplayX();
        // data->DisplayPos.y = imData->getDisplayY();
        data->DisplayPos.x = 0;
        data->DisplayPos.y = 0;
        ImGui_ImplOpenGL2_RenderDrawData(data);
    }
}

void ImGuiWidget::onHover(const HoverEvent& e)
{
    ImGui::SetCurrentContext(imData->context);

    ImGuiIO& io(ImGui::GetIO());
    io.MousePos.x = e.pos.x;
    io.MousePos.y = e.pos.y;
}

void ImGuiWidget::onButton(const ButtonEvent& e)
{
    ImGui::SetCurrentContext(imData->context);

    ImGuiIO& io(ImGui::GetIO());

    switch (e.button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        io.MouseDown[0] = e.action;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        io.MouseDown[1] = e.action;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        io.MouseDown[2] = e.action;
        break;
    }
}