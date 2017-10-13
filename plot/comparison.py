
"""
Plot the reached losses and computational costs means and standard deviation for a bunch of
different groups of simulations.

edit: 26/09/2017
"""

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys

plt.close('all')
BLUE = '#333399';
ORNG = '#ff6600';
GREE = '#00cc66';

take_log = False # Take the log of the data or not
repo = "data/backup/long_track/" # Path to the saved data
fp_range = [ # Different failure probabilities (wrt the files names)
    "00", "01", "02", "03",
    "04"]
pl_range = [0.0, 0.1, 0.2, 0.3, 0.4] # Same with real values for plotting

# Initialisation
loss1_means = [] # List of the means, to be completed
loss1_stdev = [] # List of the standard deviations, to be completed
loss2_means = []
loss2_stdev = []

cput1_means = []
cput1_stdev = []
cput2_means = []
cput2_stdev = []

calls1_means = []
calls1_stdev = []
calls2_means = []
calls2_stdev = []

for fp in fp_range:
	path1 =	 repo + "0_25_00_" + fp + "_100_40_2_09_00_25_00_" + fp + ".csv" # Vanilla UCT
	path2 = repo + "1_25_00_" + fp + "_100_40_2_09_00_25_00_" + fp + ".csv" # OLUCT

	# Open files
	d1 = pd.read_csv(path1,sep = ',')
	loss1 = d1["score"]
	cput1 = d1["computational_cost"]
	calls1 = d1["nb_calls"]

	d2 = pd.read_csv(path2,sep = ',')
	loss2 = d2["score"]
	cput2 = d2["computational_cost"]
	calls2 = d2["nb_calls"]

	if take_log:
		loss1 = np.log(loss1)
		loss2 = np.log(loss2)
		cput1 = np.log(cput1)
		cput2 = np.log(cput2)
		calls2 = np.log(calls2)
		calls1 = np.log(calls1)

	# Store the data
	loss1_means.append(loss1.mean())
	loss1_stdev.append(loss1.std())
	loss2_means.append(loss2.mean())
	loss2_stdev.append(loss2.std())

	cput1_means.append(cput1.mean())
	cput1_stdev.append(cput1.std())
	cput2_means.append(cput2.mean())
	cput2_stdev.append(cput2.std())

	calls1_means.append(calls1.mean())
	calls1_stdev.append(calls1.std())
	calls2_means.append(calls2.mean())
	calls2_stdev.append(calls2.std())

f, (ax1, ax2, ax3) = plt.subplots(3, 1, sharex=True)

l1 = ax1.errorbar(pl_range, loss1_means, color=BLUE, yerr=loss1_stdev, fmt='o')
l2 = ax1.errorbar(pl_range, loss2_means, color=ORNG, yerr=loss2_stdev, fmt='o')
ax1.set_xlabel('Transition failure probability')
ax1.set_ylabel('Loss')
ax1.legend([l1,l2],['Vanilla UCT','OLUCT'],numpoints=1,loc='upper left')

l1 = ax2.errorbar(pl_range, cput1_means, color=BLUE, yerr=cput1_stdev, fmt='o')
l2 = ax2.errorbar(pl_range, cput2_means, color=ORNG, yerr=cput2_stdev, fmt='o')
ax2.set_xlabel('Transition failure probability')
ax2.set_ylabel('Computational cost')
ax2.legend([l1,l2],['Vanilla UCT','OLUCT'],numpoints=1,loc='upper left')

l1 = ax3.errorbar(pl_range, calls1_means, color=BLUE, yerr=calls1_stdev, fmt='o')
l2 = ax3.errorbar(pl_range, calls2_means, color=ORNG, yerr=calls2_stdev, fmt='o')
ax3.set_xlabel('Transition failure probability')
ax3.set_ylabel('Number of calls')
ax3.legend([l1,l2],['Vanilla UCT','OLUCT'],numpoints=1,loc='upper left')

plt.show()

