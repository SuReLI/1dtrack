# 1D track simulator

The environment is a 1D line onto which an agent can go right or left. The agent
uses a UCT like policy in order to reach its goal.

Every parameters such as the state transition function, the reward function,
the model of the agent, the shape of the world, the action space etc. are 
tunable. In order to do so, one should directly modify the source code and
recompile using the provided Makefile.

The library is header only.

# Files details
Short explanation of the content of each file:
- 'agent.hpp': contains the classes 'agent'; 'model'; and 'policy_parameters'
respectively being the physical agent and the policy; the model used by the
policy; and the parameters of this policy.
- 'display.hpp': general printing methods.
- 'node.hpp': the node class used by the policy.
- 'test.hpp': general test cases. To be improved.
- 'track.hpp': the environment of the simulation.
- 'utils.hpp': generic methods used by every other classes. Mostly templates
methods.

# Notes
This is a 'work in progress' version and modifications will be brought from time
to time.

# Edit
22th September 2017

