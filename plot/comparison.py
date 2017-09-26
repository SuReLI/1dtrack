
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

take_log = True # Take the log of the data or not
repo = "data/backup/" # Path to the saved data
fp_range = [ # Different failure probabilities
    "00", "01", "02", "03",
    "04", "05"]

# Initialisation
loss1_means = [] # List of the means, to be completed
loss1_stdev = [] # List of the standard deviations, to be completed
loss2_means = []
loss2_stdev = []

cput1_means = []
cput1_stdev = []
cput2_means = []
cput2_stdev = []

for fp in fp_range:
    path1 = repo + "0_25_0_" + fp + "_20_50_07_09.csv"
    path2 = repo + "1_25_0_" + fp + "_20_50_07_09.csv"

    d1 = pd.read_csv(path1,sep = ',')
    loss1 = d1["score"]
    cput1 = d1["computational_cost"]
    d2 = pd.read_csv(path2,sep = ',')
    loss2 = d2["score"]
    cput2 = d2["computational_cost"]

    if take_log:
        loss1 = np.log(loss1)
        loss2 = np.log(loss2)
        cput1 = np.log(cput1)
        cput2 = np.log(cput2)

    # Store the data
    loss1_means.append(loss1.mean())
    loss1_stdev.append(loss1.std())
    loss2_means.append(loss2.mean())
    loss2_stdev.append(loss2.std())

    cput1_means.append(cput1.mean())
    cput1_stdev.append(cput1.std())
    cput2_means.append(cput2.mean())
    cput2_stdev.append(cput2.std())

f, (ax1, ax2) = plt.subplots(2, 1, sharey=True)
ax1.errorbar(fp_range, loss1_means, color=BLUE, yerr=loss1_stdev, fmt='o')
ax1.errorbar(fp_range, loss2_means, color=ORNG, yerr=loss2_stdev, fmt='o')
ax1.set_xlabel('Failure probabilities x1e1')
ax1.set_ylabel('Loss')

ax2.errorbar(fp_range, cput1_means, color=BLUE, yerr=cput1_stdev, fmt='o')
ax2.errorbar(fp_range, cput2_means, color=ORNG, yerr=cput2_stdev, fmt='o')
ax2.set_xlabel('Failure probabilities x1e1')
ax2.set_ylabel('Loss')

plt.show()
