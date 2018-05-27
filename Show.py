from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import matplotlib.ticker


fig = plt.figure()
gp = fig.add_subplot(111, projection='3d')
gp.set_xlabel('X')
gp.set_ylabel('Y')
gp.set_title('Z')

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


#bottom = np.zeros_like(z)
#width = depth = 1
#gp.bar3d(x, y, bottom, width, depth, z)

gp.plot_trisurf(x, y, z)

plt.show()
