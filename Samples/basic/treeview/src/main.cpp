/*
 * Copyright (c) 2006 - 2008
 * Wandering Monster Studios Limited
 *
 * Any use of this program is governed by the terms of Wandering Monster
 * Studios Limited's Licence Agreement included with this program, a copy
 * of which can be obtained by contacting Wandering Monster Studios
 * Limited at info@wanderingmonster.co.nz.
 *
 */

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include <Input.h>
#include <Shell.h>
#include "FileFormatter.h"
#include "FileSystem.h"

Rocket::Core::Context* context = NULL;

void GameLoop()
{
	glClear(GL_COLOR_BUFFER_BIT);

	context->Update();
	context->Render();

	Shell::FlipBuffers();
}

#if defined EMP_PLATFORM_WIN32
#include <windows.h>
int APIENTRY WinMain(HINSTANCE EMP_UNUSED(instance_handle), HINSTANCE EMP_UNUSED(previous_instance_handle), char* EMP_UNUSED(command_line), int EMP_UNUSED(command_show))
#else
int main(int EMP_UNUSED(argc), char** EMP_UNUSED(argv))
#endif
{
	// Generic OS initialisation, creates a window and attaches OpenGL.
	if (!Shell::Initialise("../Samples/basic/treeview/") ||
		!Shell::OpenWindow("Tree View Sample", true))
	{
		Shell::Shutdown();
		return -1;
	}

	// Rocket initialisation.
	ShellRenderInterfaceOpenGL opengl_renderer;
	Rocket::Core::SetRenderInterface(&opengl_renderer);

	ShellSystemInterface system_interface;
	Rocket::Core::SetSystemInterface(&system_interface);

	Rocket::Core::Initialise();

	// Create the main Rocket context and set it on the shell's input layer.
	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(1024, 768));
	if (context == NULL)
	{
		Rocket::Core::Shutdown();
		Shell::Shutdown();
		return -1;
	}

	Rocket::Controls::Initialise();
	Rocket::Debugger::Initialise(context);
	Input::SetContext(context);

	Shell::LoadFonts("../../assets/");

	// Create the file data source and formatter.
	FileSystem file_system("../Samples/basic/treeview/");
	FileFormatter file_formatter;

	// Load and show the demo document.
	Rocket::Core::ElementDocument* document = context->LoadDocument("data/treeview.rml");
	if (document != NULL)
	{
		document->GetElementById("title")->SetInnerRML(document->GetTitle());
		document->Show();
		document->RemoveReference();
	}

	Shell::EventLoop(GameLoop);

	// Shutdown Rocket.
	context->RemoveReference();
	Rocket::Core::Shutdown();

	Shell::CloseWindow();
	Shell::Shutdown();

	return 0;
}