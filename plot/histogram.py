
"""
Plot the reached scores and computational costs for 2 groups of simulations, the first one
being vanilla UCT and the second one an experimental UCT.

edit: 26/09/2017
"""

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys

plt.close('all')
data = pd.read_csv("data/data.csv",sep = ',')
scores = data["score"]
cput = data["computational_cost"]
data2 = pd.read_csv("data/data2.csv",sep = ',')
scores2 = data2["score"]
cput2 = data2["computational_cost"]

def b():
	return '#333399';
def o():
	return '#ff6600';
def g():
	return '#00cc66';

f, (ax1, ax2) = plt.subplots(2, 1, sharey=True)

sp1 = np.linspace(0, 10, 50)
ax1.hist(np.log(scores),sp1,color=b(),alpha=0.5)
ax1.hist(np.log(scores2),sp1,color=o(),alpha=0.5)
ax1.set_xlabel('Scores')
ax1.set_ylabel('Frequency')

sp2 = np.linspace(0, 10, 50)
ax2.hist(np.log(cput),sp2,color=b(),alpha=0.5)
ax2.hist(np.log(cput2),sp2,color=o(),alpha=0.5)
ax2.set_xlabel('Computational cost')
ax2.set_ylabel('Frequency')

plt.show()
