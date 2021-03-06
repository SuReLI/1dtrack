
"""
Plot the reached losses and computational costs for 2 groups of simulations, the first one
being vanilla UCT and the second one an experimental UCT.

edit: 26/09/2017
"""

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys

plt.close('all')
BLUE = '#333399';
ORANGE = '#ff6600';
GREEN = '#00cc66';

repo = "data/backup/short_track/"
fp = "00" # failure probability
path1 = repo + "0_2_00_" + fp + "_20_10_2_09_00_2_00_" + fp + ".csv" # Vanilla UCT
path2 = repo + "1_2_00_" + fp + "_20_10_2_09_00_2_00_" + fp + ".csv" # OLUCT

d1 = pd.read_csv(path1,sep = ',')
scr1 = d1["score"]
cpu1 = d1["computational_cost"]
calls1 = d1["nb_calls"]

d2 = pd.read_csv(path2,sep = ',')
scr2 = d2["score"]
cpu2 = d2["computational_cost"]
calls2 = d2["nb_calls"]

f, (ax1, ax2, ax3) = plt.subplots(3, 1, sharex=True)

lg = False
if lg:
    scr1 = np.log(scr1)
    scr2 = np.log(scr2)
    cpu1 = np.log(cpu1)
    cpu2 = np.log(cpu2)
    calls1 = np.log(calls1)
    calls2 = np.log(calls2)

rng = max(scr1.max(),scr2.max())
bins1 = np.linspace(0, rng, 20)
ax1.hist(scr1,bins1,color=BLUE,alpha=0.3)
ax1.hist(scr2,bins1,color=ORANGE,alpha=0.3)
if take_log:
	ax1.set_xlabel('Log of the loss (time steps to the goal)')
else:
	ax1.set_xlabel('Loss (time steps to the goal)')
ax1.set_ylabel('Frequency')
handles, labels = ax1.get_legend_handles_labels()
ax1.legend(handles, ["Vanilla UCT","Plain OLUCT"])

rngmin = min(cpu1.min(),cpu2.min(),0.0)
rngmax = max(cpu1.max(),cpu2.max())
bins2 = np.linspace(rngmin, rngmax, 20)
ax2.hist(cpu1,bins2,color=BLUE,alpha=0.3)
ax2.hist(cpu2,bins2,color=ORANGE,alpha=0.3)
if take_log:
	ax2.set_xlabel('Log of the computational cost (ms)')
else:
	ax2.set_xlabel('Computational cost (ms)')
ax2.set_ylabel('Frequency')
handles, labels = ax2.get_legend_handles_labels()
ax2.legend(handles, ["Vanilla UCT","Plain OLUCT"])

rngmin = min(calls1.min(),calls2.min(),0.0)
rngmax = max(calls1.max(),calls2.max())
bins3 = np.linspace(rngmin, rngmax, 20)
ax3.hist(calls1,bins3,color=BLUE,alpha=0.3)
ax3.hist(calls2,bins3,color=ORANGE,alpha=0.3)
if take_log:
	ax3.set_xlabel('Log of the number of calls')
else:
	ax3.set_xlabel('Number of calls')
ax3.set_ylabel('Frequency')
handles, labels = ax3.get_legend_handles_labels()
ax3.legend(handles, ["Vanilla UCT","Plain OLUCT"])

plt.show()
