# 1D track simulator

The environment is a 1D line onto which an agent can go right or left. The agent
uses a UCT like policy in order to reach its goal.

Every parameters such as the state transition function, the reward function,
the model of the agent, the shape of the world, the action space etc. are 
tunable. In order to do so, one should directly modify the source code and
compile the code using the provided Makefile.

Header only library.

# Use

First download the code and compile it with the provided Makefile (type 
'make compile' in the command prompt). There should be no exotic requirements in
order to do so since it only uses standard c++ libraries. Once done, you can 
directly modify the 'main.cpp' file in order to select the parameters of the 
simulations. The different methods the user has to deal with are the following:
- The method 'simulate_episode' runs a single episode given every parameters set
in the arguments;
- The method 'run_with' runs a given number of simulations with every parameters
set in the arguments;
- The method 'bunch_of_run' does multiple calls to 'run_with' method that allow
the user to modify some parameters between simulations for comparison purpose.

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

