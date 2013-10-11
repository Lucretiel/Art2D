#include <vector>
#include <string>
#include <iostream>
#include <memory>


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
				std::unique_ptr<Simulation> sim(MAKE_TYPE(Simulations, *it));
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