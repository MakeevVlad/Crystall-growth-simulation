from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d import proj3d
import matplotlib.pyplot as plt
import matplotlib.ticker
import numpy as np

fig = plt.figure()
fg = plt.figure()
ax = fig.add_subplot(111, projection='3d')
gp = fg.add_subplot(111, projection='3d')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')


x = []  #PHI
y = []	#TETA
z = []	#EFF
pot = []



f = open('crystal.txt')
for line in f:
	nums = [float(i) for i in line.split()]
	y.append(nums[0])
	x.append(nums[1])
	z.append(nums[2])
	pot.append(np.abs(nums[3]))

xa = [0, 40]
ya = [0, 40]
za = [40, 40]


bottom = np.zeros_like(z)
width = depth = 1
gp.bar3d(x, y, bottom, width, depth, z)
#gp.scatter(xa, ya, za)
ax.plot_trisurf(x, y, z)
#ax.scatter(xa, ya, za)
plt.show()
