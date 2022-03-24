import numpy as np
import random
import cv2

class SaltAndPepper(object):
	def __init__(self, filename, sp_prob):
		super(SaltAndPepper, self).__init__()
		self.is_img = False
		self.is_video = False

		if filename.split(".")[-1].lower() == "jpg" or filename.split(".")[-1].lower() == "png":
			self.is_img = True
		elif filename.split(".")[-1].lower() == "avi" or filename.split(".")[-1].lower() == "mp4":
			self.is_video = True
		else:
			print("File format "+filename.split(".")[-1]+" did not support.")
			return

		if sp_prob > 1.0 or sp_prob < 0:
			print("Bad probability of salt and pepper noise.")
			return

		self.img = None
		self.video = None

		self.filename = filename
		self.sp_prob = sp_prob
		self.ext = "."+filename.split(".")[-1]
		
	def get_pepper_and_salt_img(self, frame, prob):
		prob = prob
		out = np.copy(frame)

		p_pepper = 1-prob
		p_salt = prob

		for i in range(frame.shape[0]):
			for j in range(frame.shape[1]):
				rnd = [random.random(), random.random(), random.random()]
				out[i][j][0] = 255 if rnd[0] > p_pepper else out[i][j][0]
				out[i][j][1] = 255 if rnd[1] > p_pepper else out[i][j][1]
				out[i][j][2] = 255 if rnd[2] > p_pepper else out[i][j][2]

				out[i][j][0] = 0 if rnd[0] < p_salt else out[i][j][0]
				out[i][j][1] = 0 if rnd[1] < p_salt else out[i][j][1]
				out[i][j][2] = 0 if rnd[2] < p_salt else out[i][j][2]

		return out

	def add_noise_on_images(self):
		if self.is_video:
			vid = cv2.VideoCapture(self.filename)
			w = vid.get(cv2.CAP_PROP_FRAME_WIDTH)
			h = vid.get(cv2.CAP_PROP_FRAME_HEIGHT)


			output_filename = self.filename.replace(self.ext, "_noise"+self.ext)
			fourcc = cv2.VideoWriter_fourcc(*'XVID')
			out = cv2.VideoWriter(output_filename, fourcc, 25.0, (w, h))

			while 1:
				ret, frame = vid.read()
				if ret:
					output = self.get_pepper_and_salt_img(frame, self.sp_prob)
					out.write(output)

			vid.release()
			out.release()

		elif self.is_img:
			img = cv2.imread(self.filename)
			output = self.get_pepper_and_salt_img(img, self.sp_prob)
			output_filename = self.filename.replace(self.ext, "_noise"+self.ext)
			cv2.imwrite(output_filename, output)


spp = SaltAndPepper("4K.jpg", 0.05)
spp.add_noise_on_images()