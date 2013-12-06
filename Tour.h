#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "Menu.h"

struct Piece
{
	std::string name;
	std::string load_name;
	std::string help;

	Piece(
			const std::string& name,
			const std::string& load_name,
			const std::string& help):
		name(name),
		load_name(load_name),
		help(help)
	{}
};

struct Project
{
	std::string name;
	std::string description;
	std::vector<Piece> pieces;

	Project(const std::string& name, const std::string& description):
		name(name),
		description(description)
	{}
};

struct Portfolio
{
	std::string name;
	std::string description;
	std::vector<Project> projects;
};

class Tour
{
private:
	Portfolio port;
	Menu menu;
public:
	Tour& portfolio(const std::string& name, const std::string& description);
	Tour& section(const std::string& name, const std::string& description);
	Tour& piece(const std::string name, const std::string& load_name, const std::string& help="");

	void build(); //Hardcoded build function


	void run();
	void run_tour();
	void run_specific();
	void show_intro();
	void show_help();
	void run_piece(const std::string& name);
};

