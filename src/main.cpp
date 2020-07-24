#include <deco/all.h>
#include <framework/application.h>
#include <fstream>
#include <gui/gui.h>
#include <iostream>

constexpr int window_width = 640;
constexpr int window_height = 480;


int main()
{
	application app(window_width, window_height, "GUI Viewer", nullptr, nullptr);
	element::context = &app;

	auto& vg = app.vg;


	/*TODO
	font should be relative to the working directory path.
	working directory path is the "URL"?
	*/
	auto font = app.style_manager.create_font(vg, "../../GUI/example/font/Roboto-Regular.ttf"); // in case of running from build folder


	//TODO framework lib should provide default styles
	{
		auto const& font_name = app.style_manager.font_id_to_name(font);

		auto& style = app.style_manager.styles["text"];
		style.emplace(std::make_pair("font", font_name));
		style.emplace(std::make_pair("font_size", 18.f));
		style.emplace(std::make_pair("color", nvgRGBA(255, 255, 255, 255)));
	}

	// menu
	//TODO file path should be relative
	app.root.expand = { true, true };
	app.root.create_layout<gui::layout::box>().orient = layout::horizontal;
	app.load_style_file("gui/style.deco");
	app.load_layout("gui/layout.deco", app.root);

	app.root.child_layout->perform();


	// Loop until the user closes the window
	app.run([&]()
	{
		app.root.draw_recursive();
	});
}