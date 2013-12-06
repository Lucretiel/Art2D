#include <iostream>
#include <string>

class Menu
{
private:
	int count;

	int pick()
	{
		int selection = 0;
		while(selection < 1 || selection > count)
		{
			std::cout << "Selection (1-" << count << "): ";
			if(!(std::cin >> selection))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				selection = 0;
			}
		}
		count = 0;
		return selection;
	}

	void item(const std::string& item)
	{
		count += 1;
		std::cout << count << ") " << item << '\n';
	}

	int static_menu()
	{
		return pick();
	}


public:
	Menu(): count(0)
	{}

	template<class... Items>
	int static_menu(const std::string& i, Items&&... rest)
	{
		item(i);
		return static_menu(rest...);
	}
};