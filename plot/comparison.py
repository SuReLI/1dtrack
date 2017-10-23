
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

def extract(repo, pth1, fp_rng, pth2, take_log):
	lomns = []
	lostd = []
	cpmns = []
	cpstd = []
	camns = []
	castd = []
	for fp in fp_rng:
		path =	 repo + pth1 + fp + pth2 + fp + ".csv"
		data = pd.read_csv(path,sep = ',')
		lo = data["score"]
		cp = data["computational_cost"]
		ca = data["nb_calls"]
		if take_log:
			lo = np.log(lo)
			cp = np.log(cp)
			ca = np.log(ca)
		lomns.append(lo.mean())
		lostd.append(lo.std())
		cpmns.append(cp.mean())
		cpstd.append(cp.std())
		camns.append(ca.mean())
		castd.append(ca.std())
	return [lomns, lostd, cpmns, cpstd, camns, castd]

# Variables ####################################################################

# Take the log of the data or not
take_log = False

# Different failure probabilities (wrt the files names)
#fp_range = ["000", "005", "010", "015", "020", "025", "030", "035", "040", "045", "050", "055", "060", "065", "070", "075", "080", "085", "090", "095", "1"]
fp_range = ["000", "005", "010", "015", "020", "025", "030", "035", "040", "045", "050"]

# Same with real values for plotting
#fp_range_values = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.]
fp_range_values = [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5]

# Initialisation ###############################################################

repo = "data/backup/long/"
pth = "_100_40_2_090_000_25_000_"
[lo1_mns, lo1_std, cp1_mns, cp1_std, ca1_mns, ca1_std] = extract(repo, "0_25_000_", fp_range, pth, take_log)
[lo2_mns, lo2_std, cp2_mns, cp2_std, ca2_mns, ca2_std] = extract(repo, "1_25_000_", fp_range, pth, take_log)
[lo3_mns, lo3_std, cp3_mns, cp3_std, ca3_mns, ca3_std] = extract(repo, "2_25_000_", fp_range, pth, take_log)

plt.close('all')
f, (ax1, ax2, ax3) = plt.subplots(3, 1, sharex=True)

### Plot #######################################################################

# Plot 1 - loss

l1 = plot_error_bar(ax1,fp_range_values,lo1_mns,lo1_std,BLUE)
l2 = plot_error_bar(ax1,fp_range_values,lo2_mns,lo2_std,ORNG)
l3 = plot_error_bar(ax1,fp_range_values,lo3_mns,lo3_std,GREE)

ax1.set_xlabel('Transition failure probability')
if take_log:
	ax1.set_ylabel('Log of the loss (time steps to the goal)')
else:
	ax1.set_ylabel('Loss (time steps to the goal)')
ax1.legend([l1,l2,l3],['Vanilla UCT','Plain OLUCT','State mode-test OLUCT'],numpoints=1,loc='upper left')

# Plot 2 - computational cost

l1 = plot_error_bar(ax2,fp_range_values,cp1_mns,cp1_std,BLUE)
l2 = plot_error_bar(ax2,fp_range_values,cp2_mns,cp2_std,ORNG)
l3 = plot_error_bar(ax2,fp_range_values,cp3_mns,cp3_std,GREE)

ax2.set_xlabel('Transition failure probability')
if take_log:
	ax2.set_ylabel('Log of the computational cost (ms)')
else:
	ax2.set_ylabel('Computational cost (ms)')
ax2.legend([l1,l2,l3],['Vanilla UCT','Plain OLUCT','State mode-test OLUCT'],numpoints=1,loc='upper left')

# Plot 3 - number of call

l1 = plot_error_bar(ax3,fp_range_values,ca1_mns,ca1_std,BLUE)
l2 = plot_error_bar(ax3,fp_range_values,ca2_mns,ca2_std,ORNG)
l3 = plot_error_bar(ax3,fp_range_values,ca3_mns,ca3_std,GREE)

ax3.set_xlabel('Transition failure probability')
if take_log:
	ax3.set_ylabel('Log of the number of call')
else:
	ax3.set_ylabel('Number of call')
ax3.legend([l1,l2,l3],['Vanilla UCT','Plain OLUCT','State mode-test OLUCT'],numpoints=1,loc='upper left')

plt.show()

