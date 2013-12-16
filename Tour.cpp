#include "Tour.h"
#include "Simulation.h"
#include <iostream>

Tour& Tour::portfolio(const std::string& name, const std::string& description)
{
	port.name = name;
	port.description = description;
	port.projects.clear();
	return *this;
}

Tour& Tour::section(const std::string& name, const std::string& description)
{
	port.projects.emplace_back(name, description);
	return *this;
}

Tour& Tour::piece(const std::string name, const std::string& load_name, const std::string& help)
{
	port.projects.back().pieces.emplace_back(name, load_name, help);
	return *this;
}

void Tour::build()
{
	(*this)
		.portfolio(
			"A study of 2D physicality through 1x5 rectangles\n"
			"Fundamentals of 2D Design Final Portfolio\n"
			"Nathan West",

			"A series of images primarly focused on variations of simple black 1x5\n"
			"rectangles, with special emphasis on the use of implied physicality to\n"
			"create the sense of real objects, despite the medium being essentially\n"
			"purely abstract.")

			.section("Basic Composition",
			"In the basic composition project, we worked on simple placement of\n"
			"visual elements in the space. This first inspired me to create my\n"
			"work out of a rendering and physics engine. I wanted to explore how\n"
			"interesting images could be created by manipulating literally\n"
			"physicially simulated objects.")
			.piece("Top", "explosion",
			"Press Space to trigger an explosion.")

			.section("Depth",
			"This project was concerned with various ways of creating depth in an\n"
			"image.")
			.piece("Size Change", "resize")
			.piece("Layers", "layer")
			.piece("Height", "height",
			"Move your mouse to the top or bottom of the window to see the\n"
			"different gravity wells")
			.piece("Gradient", "gradient")

			.section("Space",
			"This project was all about creating different kinds of space. My aim\n"
			"when creating these was to have the blocks moving through the view in\n"
			"different ways, creating the space. The Open Space piece has the\n"
			"blocks freely flowing from one side of the screen to the other, while\n"
			"the Close Space piece has them filling a bucket before overflowing.\n"
			"Meanwhile, the packed space squeezes the blocks through a narrow\n"
			"aperture, while the empty space has sparse blocks falling and\n"
			"embedding in the ground.")
			.piece("Open Space", "open",
			"Press R to reset the simulation")
			.piece("Closed Space", "closed")
			.piece("Packed Space", "packed",
			"Press the Spacebar to add extra pressure to the aperture")
			.piece("Empty Space", "empty",
			"Press the Spacebar to spawn a falling block\n"
			"Press the K key to remove a block from the ground")

			.section("Perspective",
			"The perspectives project was the most technically challenging so far,\n"
			"and might have been the most technically challenging project of the\n"
			"semester. For it, I created a perfect-vanishing-point renderer to\n"
			"draw the right-angled rectangles on an angled plane, to a vanishing\n"
			"point. The Vanishing Point and Flat pieces are supposed to show\n"
			"different views of the same senario, which is a theme I use in much\n"
			"of my future work. The Ambiguous space was created by drawing random\n"
			"rectangles on to random spaces, where each space had a different\n"
			"vanishing point.")
			.piece("Vanishing Point", "vanishing")
			.piece("Flat", "flat")
			.piece("Ambiguous", "ambiguous")
			.piece("Isometric", "isometric")

			.section("Scale and Shape",
			"For this project, I hit upon perhaps my most inspired idea- a simple\n"
			"perspective shift to transfom a monumental scale into an intimate, in\n"
			"an otherwise nearly identical composition. The tension image had me\n"
			"significantly stretching out the blocks to create a sense of literal\n"
			"tension, while the negative space image was a simple twist on the\n"
			"classic \"blocks fall into a container\" theme.")
			.piece("Monumental", "monumental")
			.piece("Intimate", "intimate")
			.piece("Negative Space", "negative")
			.piece("Tension", "tension")

			.section("Line",
			"If the perspectives project was the most technically demanding I had,\n"
			"then the lines project was the most artistically. While rectangles are\n"
			"great for different expressions of shape, space, and scale, they are\n"
			"much less so for linework. This project was also a humbling reminder\n"
			"that there isn't always a direct relationship between \"hours-put-in\"\n"
			"and \"final image quality\"- The Implied Line image only took me about\n"
			"half an hour, while the rendering tech to create the gestural image\n"
			"took many many hours.")
			.piece("Implied Line", "implied")
			.piece("Gestural", "gestural",
			"Press and hold the spacebar to cause blocks to fall, release to stop.\n"
			"Press P to pause the simulation, then press F to draw the linework.\n"
			"The linework may take up to a few minutes to calculate. Press F again\n"
			"to recalculate the lines, or P to unpause the simulation.")
			.piece("Impersonal", "impersonal")
			.piece("3D Line Surface", "3d_surface")

			.section("Color",
			"After taking a 2 week hiatus from creating images, I returned to find\n"
			"that while perspectives had been technically challenging, and lines\n"
			"artistically challenging, color was both, and much worse. The theme I\n"
			"had been going with simply did not accomodate color very well, as\n"
			"numerous scrapped sketchbook pages can attest. As a result, these\n"
			"images are significant departure in visual style from my earlier work,\n"
			"while still maintaining the small rectangle as the fundamental unit.\n"
			"I discovered that the best way to make color meaningful and useful in\n"
			"my style was to create identical compositions, where the image differs\n"
			"only in color. There were also significant technical challenges- while\n"
			"on the surface, computer color is very simple, it is very difficult to\n"
			"perform useful color transormations (blended color, lighter/darker/\n"
			"saturated/desaturated, etc) in a way that resembles real life. That's\n"
			"not to mention that often all that work is made irrelevant when the\n"
			"printed colors look differnet than those on the screen.\n")
			.piece("Soft Color", "soft")
			.piece("Warm Color", "warm")
			.piece("Cool Color", "cool")
			.piece("Intense Color", "intense")

			.section("Color 2",
			"No comments. This was a reapplication of the ideas from the previous\n"
			"project.")
			.piece("Vibrating", "vibrating")
			.piece("Changing Greys", "changing-greys")

			.section("Light",
			"Again, this was a reapplication of the color ideas- four identical\n"
			"compositions, each with a different set of colors. It was especially\n"
			"important for this one that they all be the same setting, because the\n"
			"forms in my work don't have a native concept of color. By having these\n"
			"images together, the viewer can tell that they are supposed to be the\n"
			"same setup in different lighting conditions. The major technical\n"
			"challenges for this one were getting the light and color to blend\n"
			"correctly, and getting the shadow layering to work.")
			.piece("Local Color", "local")
			.piece("Warm Light", "warm-light")
			.piece("Cool Light", "cool-light")
			.piece("Subjective Color", "subjective")

			.section("Composition 2",
			"This was a refreshing return to form for me, after the trouble I had\n"
			"with color. Unfortunatly, because I immediatly snapped back into my\n"
			"comfort zone with this project, these are all very safe images, and\n"
			"none of them I would say are my best work.")
			.piece("Dissonance", "dissonance")
			.piece("Harmony", "harmony")
			.piece("Non-Heirarchical", "non-hierarchy")
			.piece("Heirarchical", "hierarchy");
}


void Tour::run()
{
	std::cout << "Welcome to the Art2D Expression Engine, by Nathan West\nLoading portfolio...\n\n";
	build();
	show_intro();

	while(true)
	{
		int selection = menu.static_menu(
			"Begin tour",
			"Load specific piece",
			"Show intro",
			"Show help",
			"Quit");

		switch (selection)
		{
		case 1:
			run_tour();
			break;
		case 2:
			run_specific();
			break;
		case 3:
			show_intro();
			break;
		case 4:
			show_help();
			break;
		case 5:
			return;
		}
	}
}

void Tour::run_tour()
{
	int project = 0;
	int piece = 0;

	auto get_project = [&]()->const Project&
	{
		return port.projects[project];
	};

	auto get_piece = [&]()->const Piece&
	{
		return get_project().pieces[piece];
	};

	auto project_info = [&]()
	{
		std::cout << "\n" << "Project: " << get_project().name << "\n" << get_project().description << "\n\n";
	};

	auto piece_info = [&]()
	{
		std::cout << "Piece: " << get_piece().name << "\n";
		if(!get_piece().help.empty())
			std::cout << "Additional controls:\n" << get_piece().help << "\n\n";
	};

	project_info();
	piece_info();

	while(true)
	{
		int selection = menu.static_menu(
			"Continue",
			"View Piece",
			"Next Piece",
			"Next Project",
			"Show Help",
			"Back");

		switch(selection)
		{
		case 1:
			run_piece(get_piece().load_name);
			piece = (piece + 1) % get_project().pieces.size();
			if(piece == 0)
			{
				project = (project + 1) % port.projects.size();
				if(project == 0)
					return;
				project_info();
			}
			piece_info();
			break;
		case 2:
			run_piece(get_piece().load_name);
			break;
		case 3:
			piece = (piece + 1) % get_project().pieces.size();
			piece_info();
			break;
		case 4:
			project = (project + 1) % port.projects.size();
			piece = 0;
			project_info();
			piece_info();
			break;
		case 5:
			show_help();
			break;
		case 6:
			return;
		}
	}
}

void Tour::run_specific()
{
	int selection = menu.static_menu(
		"Select from projects",
		"Select from all pieces",
		"Show help",
		"Back");

	switch (selection)
	{
	case 1:
		{
			std::cout << "Select Project:\n";
			const Project& project = menu.dynamic_menu(port.projects, [](const Project& project)
			{
				return project.name;
			});
			std::cout << "\n" << "Project: " << project.name << "\n" << project.description << "\n\n";

			std::cout << "Select Piece:\n";
			const Piece& piece = menu.dynamic_menu(project.pieces, [](const Piece& piece)
			{
				return piece.name;
			});
			std::cout << "Piece: " << piece.name << "\n";
			if(!piece.help.empty())
				std::cout << "Additional controls:\n" << piece.help << "\n\n";

			run_piece(piece.load_name);
		}
		break;
	case 2:
		{
			std::vector<const Piece*> pieces;
			for(const Project& project : port.projects)
				for(const Piece& piece : project.pieces)
					pieces.push_back(&piece);

			std::sort(pieces.begin(), pieces.end(), [](const Piece* lhs, const Piece* rhs)
			{
				return lhs->name < rhs->name;
			});

			std::cout << "Select Piece:\n";
			const Piece& piece(*menu.dynamic_menu(pieces, [](const Piece* piece)
			{
				return piece->name;
			}));

			std::cout << "Piece: " << piece.name << "\n";
			if(!piece.help.empty())
				std::cout << "Additional controls:\n" << piece.help << "\n\n";
			run_piece(piece.load_name);
		}
		break;
	case 3:
		show_help();
		break;
	case 4:
		return;
	}
}

void Tour::show_intro()
{
	std::cout << port.name << "\n\n" << port.description << "\n\n";
}

void Tour::show_help()
{
	std::cout <<
		"At a menu, enter the number of your selection and press enter.\n"
		"When viewing a piece, you can use the following controls:\n"
		"    Q: quit\n"
		"    R: restart from beginning\n"
		"    S: save a screenshot\n"
		"    P: pause the piece. Screenshots can be taken while paused.\n"
		"Some pieces may have special specific controls. These will be\n"
		"displayed when the piece is loading.\n\n";
}

void Tour::run_piece(const std::string& name)
{
	bool reset;
	do
	{
		reset = false;
		try
		{
			std::unique_ptr<Simulation> sim(MAKE_TYPE(Simulations, name));
			if(sim)
				sim->execute();
		}
		catch(Restart&)
		{
			reset = true;
		}
	} while(reset);
}
