#include <vector>
#include <string>
#include <iostream>

#include "TypeRegistry.h"
#include "Simulation.h"

int main(int nargs, char* args[])
{
	std::vector<std::string> simulations(args, args+nargs);

	for(auto it = simulations.begin(); it != simulations.end(); ++it)
	{
		bool reset;
		do
		{
			reset = false;
			try
			{
				auto sim = MAKE_TYPE(Simulation, *it);
				if(sim)
					sim->execute();
			}
			catch(Restart&)
			{
				reset = true;
			}
		} while(reset);
	}
}