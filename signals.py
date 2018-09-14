import numpy as np
from sklearn.preprocessing import scale
from scipy.interpolate import interp1d

'''

'''

class Sample:
	'''
	ignals from file and process them.
	'''
	def __init__(self, acx, acy, acz, gx, gy, gz):
		self.acx = acx
		self.acy = acy
		self.acz = acz
		self.gx = gx
		self.gy = gy
		self.gz = gz

	def get_linearized(self, reshape = False):
	
		
		data_raw = [map(lambda x: int(x), i.split(" ")[1:-1]) for i in open(filename)]

		
		data = np.array(data_raw).astype(float)

		
		data_norm = scale(data)

		
		acx = data_norm[:,0]
		acy = data_norm[:,1]
		acz = data_norm[:,2]

		gx = data_norm[:,3]
		gy = data_norm[:,4]
		gz = data_norm[:,5]

		#Create a function for each axe that interpolates the samples
		x = np.linspace(0, data.shape[0], data.shape[0])
		f_acx = interp1d(x, acx)
		f_acy = interp1d(x, acy)
		f_acz = interp1d(x, acz)

		f_gx = interp1d(x, gx)
		f_gy = interp1d(x, gy)
		f_gz = interp1d(x, gz)

		
		xnew = np.linspace(0, data.shape[0], size_fit)
		acx_stretch = f_acx(xnew)
		acy_stretch = f_acy(xnew)
		acz_stretch = f_acz(xnew)

		gx_stretch = f_gx(xnew)
		gy_stretch = f_gy(xnew)
		gz_stretch = f_gz(xnew)

		
		return Sample(acx_stretch, acy_stretch, acz_stretch, gx_stretch, gy_stretch, gz_stretch)