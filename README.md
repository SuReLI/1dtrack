# 1D track simulator

The environment is a 1D line onto which an agent can go right or left. The agent
uses a UCT like policy in order to reach its goal.

Every parameters such as the state transition function, the reward function,
the model of the agent, the shape of the world, the action space etc. are 
tunable. In order to do so, one should directly modify the source code and
compile the code using the provided Makefile.

Header only library.

# Use

First download the code and compile it with the provided Makefile (you can type 
'make compile' in the command prompt). The only non standard c++ library used
in this project is 'libconfig.h++'. You can install it via your package manager
or from source via https://github.com/hyperrealm/libconfig

Once the code compiled, you can set the parameters of the simulations via the
configuration file 'main.cfg'.

Running the code will run 100 simulations, this is default. To set the number of
simulations, for instance to 1000, you can execute './exe 1000' or type
'make NBSIM=1000' using the Makefile.

Python scripts for plotting are provided in 'plot/' repository.

# Files details
Short explanation of the content of each file:
- 'agent.hpp': contains the classes 'agent'; 'model'; and 'policy_parameters'
respectively being the physical agent and its policy; the model used by the
policy; and the parameters of this policy.
- 'display.hpp': general display methods.
- 'node.hpp': the node class used by the policy.
- 'parameters.hpp': the parameters of the simulations including those of the
environment, the agent and its policy.
- 'save.hpp': saving methods.
- 'test.hpp': general test cases. To be improved with more unit tests.
- 'track.hpp': the environment of the simulation.
- 'utils.hpp': generic methods used by every other classes. Mostly templates
methods.

# Notes
This is a 'work in progress' version and modifications will be brought from time
to time.

# Edit
9th October 2017

