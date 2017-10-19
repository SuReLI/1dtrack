
"""
Plot the reached losses and computational costs means and standard deviation for
a bunch of different groups of simulations.
Set the correct paths and run this script.

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
fp_range = ["000", "005", "010", "015", "020", "025", "030", "035", "040", "045", "050", "055", "060", "065", "070", "075", "080", "085", "090", "095", "1"] # Different failure probabilities (wrt the files names)
pl_range = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.] # Same with real values for plotting

# Initialisation
loss1_means = [] # List of the means
loss1_stdev = [] # List of the standard deviations
loss2_means = []
loss2_stdev = []
loss3_means = []
loss3_stdev = []

cput1_means = []
cput1_stdev = []
cput2_means = []
cput2_stdev = []
cput3_means = []
cput3_stdev = []

calls1_means = []
calls1_stdev = []
calls2_means = []
calls2_stdev = []
calls3_means = []
calls3_stdev = []

for fp in fp_range:
	repo = "data/" # Path to the saved data
	# Vanilla UCT
	path1 =	 repo + "0_2_000_" + fp + "_20_10_2_090_000_2_000_" + fp + ".csv"
	# Plain OLUCT
	path2 = repo + "1_2_000_" + fp + "_20_10_2_090_000_2_000_" + fp + ".csv"
	# State multi-modality test OLUCT
	path3 = repo + "2_2_000_" + fp + "_20_10_2_090_000_2_000_" + fp + ".csv"

	# Open files
	d1 = pd.read_csv(path1,sep = ',')
	loss1 = d1["score"]
	cput1 = d1["computational_cost"]
	calls1 = d1["nb_calls"]

	d2 = pd.read_csv(path2,sep = ',')
	loss2 = d2["score"]
	cput2 = d2["computational_cost"]
	calls2 = d2["nb_calls"]

	d3 = pd.read_csv(path3,sep = ',')
	loss3 = d3["score"]
	cput3 = d3["computational_cost"]
	calls3 = d3["nb_calls"]

	if take_log:
		loss1 = np.log(loss1)
		loss2 = np.log(loss2)
		loss3 = np.log(loss3)
		cput1 = np.log(cput1)
		cput2 = np.log(cput2)
		cput3 = np.log(cput3)
		calls1 = np.log(calls1)
		calls2 = np.log(calls2)
		calls3 = np.log(calls3)

	# Store the data
	loss1_means.append(loss1.mean())
	loss1_stdev.append(loss1.std())
	loss2_means.append(loss2.mean())
	loss2_stdev.append(loss2.std())
	loss3_means.append(loss3.mean())
	loss3_stdev.append(loss3.std())

	cput1_means.append(cput1.mean())
	cput1_stdev.append(cput1.std())
	cput2_means.append(cput2.mean())
	cput2_stdev.append(cput2.std())
	cput3_means.append(cput3.mean())
	cput3_stdev.append(cput3.std())

	calls1_means.append(calls1.mean())
	calls1_stdev.append(calls1.std())
	calls2_means.append(calls2.mean())
	calls2_stdev.append(calls2.std())
	calls3_means.append(calls3.mean())
	calls3_stdev.append(calls3.std())

f, (ax1, ax2, ax3) = plt.subplots(3, 1, sharex=True)

# Plot 1 - loss

l2 = ax1.errorbar(pl_range, loss2_means, color=ORNG, yerr=loss2_stdev)
l2 = ax1.errorbar(pl_range, loss2_means, color=ORNG, yerr=loss2_stdev, fmt='o')
v = np.copy(loss2_stdev)
ax1.fill_between(pl_range, np.add(loss2_means,v), np.add(loss2_means,-v), color=ORNG, alpha=0.2)

l1 = ax1.errorbar(pl_range, loss1_means, color=BLUE, yerr=loss1_stdev)
l1 = ax1.errorbar(pl_range, loss1_means, color=BLUE, yerr=loss1_stdev, fmt='o')
v = np.copy(loss1_stdev)
ax1.fill_between(pl_range, np.add(loss1_means,v), np.add(loss1_means,-v), color=BLUE, alpha=0.2)

l3 = ax1.errorbar(pl_range, loss3_means, color=GREE, yerr=loss3_stdev)
l3 = ax1.errorbar(pl_range, loss3_means, color=GREE, yerr=loss3_stdev, fmt='o')
v = np.copy(loss3_stdev)
ax1.fill_between(pl_range, np.add(loss3_means,v), np.add(loss3_means,-v), color=GREE, alpha=0.2)

ax1.set_xlabel('Transition failure probability')
if take_log:
	ax1.set_ylabel('Log of the loss (time steps to the goal)')
else:
	ax1.set_ylabel('Loss (time steps to the goal)')
ax1.legend([l1,l2,l3],['Vanilla UCT','Plain OLUCT','State mode-test OLUCT'],numpoints=1,loc='upper right')

# Plot 2 - computational cost

l1 = ax2.errorbar(pl_range, cput1_means, color=BLUE, yerr=cput1_stdev)
l1 = ax2.errorbar(pl_range, cput1_means, color=BLUE, yerr=cput1_stdev, fmt='o')
v = np.copy(cput1_stdev)
ax2.fill_between(pl_range, np.add(cput1_means,v), np.add(cput1_means,-v), color=BLUE, alpha=0.2)

l2 = ax2.errorbar(pl_range, cput2_means, color=ORNG, yerr=cput2_stdev)
l2 = ax2.errorbar(pl_range, cput2_means, color=ORNG, yerr=cput2_stdev, fmt='o')
v = np.copy(cput2_stdev)
ax2.fill_between(pl_range, np.add(cput2_means,v), np.add(cput2_means,-v), color=ORNG, alpha=0.2)

l3 = ax2.errorbar(pl_range, cput3_means, color=GREE, yerr=cput3_stdev)
l3 = ax2.errorbar(pl_range, cput3_means, color=GREE, yerr=cput3_stdev, fmt='o')
v = np.copy(cput3_stdev)
ax2.fill_between(pl_range, np.add(cput3_means,v), np.add(cput3_means,-v), color=GREE, alpha=0.2)

ax2.set_xlabel('Transition failure probability')
if take_log:
	ax2.set_ylabel('Log of the computational cost (ms)')
else:
	ax2.set_ylabel('Computational cost (ms)')
ax2.legend([l1,l2,l3],['Vanilla UCT','Plain OLUCT','State mode-test OLUCT'],numpoints=1,loc='upper right')

# Plot 3 - number of calls

l1 = ax3.errorbar(pl_range, calls1_means, color=BLUE, yerr=calls1_stdev)
l1 = ax3.errorbar(pl_range, calls1_means, color=BLUE, yerr=calls1_stdev, fmt='o')
v = np.copy(calls1_stdev)
ax3.fill_between(pl_range, np.add(calls1_means,v), np.add(calls1_means,-v), color=BLUE, alpha=0.2)

l2 = ax3.errorbar(pl_range, calls2_means, color=ORNG, yerr=calls2_stdev)
l2 = ax3.errorbar(pl_range, calls2_means, color=ORNG, yerr=calls2_stdev, fmt='o')
v = np.copy(calls2_stdev)
ax3.fill_between(pl_range, np.add(calls2_means,v), np.add(calls2_means,-v), color=ORNG, alpha=0.2)

l3 = ax3.errorbar(pl_range, calls3_means, color=GREE, yerr=calls3_stdev)
l3 = ax3.errorbar(pl_range, calls3_means, color=GREE, yerr=calls3_stdev, fmt='o')
v = np.copy(calls3_stdev)
ax3.fill_between(pl_range, np.add(calls3_means,v), np.add(calls3_means,-v), color=GREE, alpha=0.2)

ax3.set_xlabel('Transition failure probability')
if take_log:
	ax3.set_ylabel('Log of the number of calls')
else:
	ax3.set_ylabel('Number of calls')
ax3.legend([l1,l2,l3],['Vanilla UCT','Plain OLUCT','State mode-test OLUCT'],numpoints=1,loc='upper right')

#ax1.set_xlim(0.,0.4)

plt.show()

