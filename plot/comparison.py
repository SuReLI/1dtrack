
"""
Plot the reached loes and computational costs mns and standard deviation for
a bunch of different groups of simulations.
Set the correct paths and run this script.

edit: 26/09/2017
"""

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys

BLUE = '#333399';
ORNG = '#ff6600';
GREE = '#00cc66';

def plot_error_bar(ax, rng, mean, stddev, cl):
	l = ax.errorbar(rng, mean, color=cl, yerr=stddev)
	l = ax.errorbar(rng, mean, color=cl, yerr=stddev, fmt='o')
	v = np.copy(stddev)
	up = np.add(mean, +v)
	dw = np.add(mean, -v)
	ax.fill_between(rng, up, dw, color=cl, alpha=0.1)
	return l

take_log = False # Take the log of the data or not
fprng = ["000", "005", "010", "015", "020", "025", "030", "035", "040", "045", "050", "055", "060", "065", "070", "075", "080", "085", "090", "095", "1"] # Different failure probabilities (wrt the files names)
rng = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.] # Same with real values for plotting

# Initialisation
lo1_mns = [] # List of the means
lo1_std = [] # List of the standard deviations
lo2_mns = []
lo2_std = []
lo3_mns = []
lo3_std = []

cp1_mns = []
cp1_std = []
cp2_mns = []
cp2_std = []
cp3_mns = []
cp3_std = []

ca1_mns = []
ca1_std = []
ca2_mns = []
ca2_std = []
ca3_mns = []
ca3_std = []

for fp in fprng:
	repo = "data/" # Path to the saved data
	# Vanilla UCT
	path1 =	 repo + "0_2_000_" + fp + "_20_10_2_090_000_2_000_" + fp + ".csv"
	# Plain OLUCT
	path2 = repo + "1_2_000_" + fp + "_20_10_2_090_000_2_000_" + fp + ".csv"
	# State multi-modality test OLUCT
	path3 = repo + "2_2_000_" + fp + "_20_10_2_090_000_2_000_" + fp + ".csv"

	# Open files
	d1 = pd.read_csv(path1,sep = ',')
	lo1 = d1["score"]
	cp1 = d1["computational_cost"]
	ca1 = d1["nb_calls"]

	d2 = pd.read_csv(path2,sep = ',')
	lo2 = d2["score"]
	cp2 = d2["computational_cost"]
	ca2 = d2["nb_calls"]

	d3 = pd.read_csv(path3,sep = ',')
	lo3 = d3["score"]
	cp3 = d3["computational_cost"]
	ca3 = d3["nb_calls"]

	if take_log:
		lo1 = np.log(lo1)
		lo2 = np.log(lo2)
		lo3 = np.log(lo3)
		cp1 = np.log(cp1)
		cp2 = np.log(cp2)
		cp3 = np.log(cp3)
		ca1 = np.log(ca1)
		ca2 = np.log(ca2)
		ca3 = np.log(ca3)

	# Store the data
	lo1_mns.append(lo1.mean())
	lo1_std.append(lo1.std())
	lo2_mns.append(lo2.mean())
	lo2_std.append(lo2.std())
	lo3_mns.append(lo3.mean())
	lo3_std.append(lo3.std())

	cp1_mns.append(cp1.mean())
	cp1_std.append(cp1.std())
	cp2_mns.append(cp2.mean())
	cp2_std.append(cp2.std())
	cp3_mns.append(cp3.mean())
	cp3_std.append(cp3.std())

	ca1_mns.append(ca1.mean())
	ca1_std.append(ca1.std())
	ca2_mns.append(ca2.mean())
	ca2_std.append(ca2.std())
	ca3_mns.append(ca3.mean())
	ca3_std.append(ca3.std())

plt.close('all')
f, (ax1, ax2, ax3) = plt.subplots(3, 1, sharex=True)

# Plot 1 - loss

l1 = plot_error_bar(ax1,rng,lo1_mns,lo1_std,BLUE)
l2 = plot_error_bar(ax1,rng,lo2_mns,lo2_std,ORNG)
l3 = plot_error_bar(ax1,rng,lo3_mns,lo3_std,GREE)

ax1.set_xlabel('Transition failure probability')
if take_log:
	ax1.set_ylabel('Log of the loss (time steps to the goal)')
else:
	ax1.set_ylabel('Loss (time steps to the goal)')
ax1.legend([l1,l2,l3],['Vanilla UCT','Plain OLUCT','State mode-test OLUCT'],numpoints=1,loc='upper right')

# Plot 2 - computational cost

l1 = plot_error_bar(ax2,rng,cp1_mns,cp1_std,BLUE)
l2 = plot_error_bar(ax2,rng,cp2_mns,cp2_std,ORNG)
l3 = plot_error_bar(ax2,rng,cp3_mns,cp3_std,GREE)

ax2.set_xlabel('Transition failure probability')
if take_log:
	ax2.set_ylabel('Log of the computational cost (ms)')
else:
	ax2.set_ylabel('Computational cost (ms)')
ax2.legend([l1,l2,l3],['Vanilla UCT','Plain OLUCT','State mode-test OLUCT'],numpoints=1,loc='upper right')

# Plot 3 - number of call

l1 = plot_error_bar(ax3,rng,ca1_mns,ca1_std,BLUE)
l2 = plot_error_bar(ax3,rng,ca2_mns,ca2_std,ORNG)
l3 = plot_error_bar(ax3,rng,ca3_mns,ca3_std,GREE)

ax3.set_xlabel('Transition failure probability')
if take_log:
	ax3.set_ylabel('Log of the number of call')
else:
	ax3.set_ylabel('Number of call')
ax3.legend([l1,l2,l3],['Vanilla UCT','Plain OLUCT','State mode-test OLUCT'],numpoints=1,loc='upper right')



plt.show()

