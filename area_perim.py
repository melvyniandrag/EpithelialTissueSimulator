import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns

fin = open("parameters.hpp", 'r')
for i in range(3):
    junk = fin.readline()
beta = fin.readline().split()[4]
lam = fin.readline().split()[4]
junk = fin.readline()
gamma = fin.readline().split()[4]
area = fin.readline().split()[4]
fin.close()
plot_text = 'beta: ' + beta + '\nlambda: ' + lam + '\ngamma: ' + gamma + '\narea: ' + area

# Make area plot
plt.figure()
a = open('area.txt').read().splitlines()
data = map(float, a)
plt.hist(data, 10)
Title = 'Equilibrium Area: beta = ' + beta + ' lambda = ' + lam + ' gamma = ' + gamma
plt.title(Title, fontsize = 18, fontweight='bold')
plt.xlabel('Area', fontsize=14, fontweight='bold')
plt.ylabel('Number of Cells', fontsize = 14, fontweight='bold')
plt.savefig('Images/area.png')

plt.figure()
p = open('perim.txt').read().splitlines()
data = map(float, p)
plt.hist(data, 10)
Title = 'Equilibrium Perimeter: beta = ' + beta + ' lambda = ' + lam + ' gamma = ' + gamma
plt.title(Title, fontsize = 18, fontweight='bold')
plt.xlabel('Perimeter', fontsize=14, fontweight='bold')
plt.ylabel('Number of Cells', fontsize = 14, fontweight='bold')
plt.savefig('Images/perim.png')

