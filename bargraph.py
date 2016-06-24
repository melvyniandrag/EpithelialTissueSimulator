from numpy import *
import matplotlib.pyplot as plt

sides0, num_cells0 = loadtxt('hist_data0.txt', unpack=1, usecols=[0,1])
sides1, num_cells1 = loadtxt('hist_data1.txt', unpack=1, usecols=[0,1])
# We need to make sure that both rectangles have the same number of sides, 
# otherwise the x axis will be missing some ticks.

# I am going to deal with this in the easiest way possible. I am going 
# to force both num_cells to have the sides from 3 to 10.

fig = plt.figure(facecolor='white')
ax1 = fig.add_subplot(221)

N = len(sides0)
ind = arange(N)  # the x locations for the groups
width = 0.35       # the width of the bars

rects1 = ax1.bar(ind, num_cells0, width, color='orange')
rects2 = ax1.bar(ind + width, num_cells1, width, color = 'b' )

ax1.set_ylabel('Number of Cells', fontsize=12, fontweight='bold')
ax1.set_xlabel('Sides', fontsize=12, fontweight='bold')
ax1.set_title('Distribution of Cell Shapes',fontsize=14, fontweight='bold')
ax1.set_xticks(ind+width)
ax1.set_xticklabels(sides0)
ax1.legend( (rects1[0], rects2[0]), ('Before', 'After') )
ax1.set_xlim(0, N-width)

plt.savefig('Images/bargraph.pdf')
