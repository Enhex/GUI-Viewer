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
	auto font = app.style_manager.create_font(vg, "../GUI/example/font/Roboto-Regular.ttf"); // in case of running from build folder


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
	

	/*TODO
	style file should be included from the layout file?
	how the layout file is included?
	have separate entry points for style and layout?
	let the user specify the file paths?
		- style/layout path is relative to the working directory path
	*/
	{
		auto file = std::ifstream("style.deco", std::ios::binary);
		std::string file_str{
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>() };

		auto stream = deco::make_InputStream(file_str.cbegin());

		while (stream.position != file_str.cend() && !stream.peek_list_end())
		{
			std::string name;
			style::style_st properties;
			deco::serialize(stream, deco::begin_list(name));
			deco::serialize(stream, properties);
			deco::serialize(stream, deco::end_list);

			app.style_manager.styles.emplace(name, properties);
		}
	}


	// test using layout file
	// read from file
	{
		auto file = std::ifstream("layout.deco", std::ios::binary);
		std::string file_str{
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>() };

		auto entries = deco::parse(file_str.begin(), file_str.end());
		
		//TODO this functionality should be provided by the GUI library
		for (auto& entry : entries)
		{
			using namespace deco;

			auto const& name = entry.content;

			if (name == "element") {
				auto& child = emplace_back_derived<element>(app.root.children);
				read(entry, child);
			}
			else if (name == "panel") {
				auto& child = emplace_back_derived<panel>(app.root.children);
				read(entry, child);
			}
			else if (name == "text") {
				auto& child = emplace_back_derived<text>(app.root.children);
				read(entry, child);
			}
		}
	}


	// Loop until the user closes the window
	app.run([&]()
	{
		app.root.draw_recursive();
	});
}