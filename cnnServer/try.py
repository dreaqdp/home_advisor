import os
import numpy as np
from keras.models import load_model
from keras.applications.mobilenet import preprocess_input
#from PIL import Image
import cv2

model = load_model('megamodel.h5')
print('model carregat')

while True:
    os.system('ffmpeg -f video4linux2 -i /dev/video2 -vframes 2 test%3d.jpeg')
 #   im = Image.open('test002.jpeg')
 #   im = im.resize((224,224))
    im = cv2.imread('test002.jpeg')
    im = cv2.resize(im, (224,224))
    aux = model.predict(preprocess_input(np.array([im])))
    print(aux[0][0] < aux[0][1])
    input()

#for f in os.listdir('./guill/'):
#	print(f)
#	im = cv2.imread('./guill/' + f)
#	im = cv2.resize(im, (224,224))
#	aux = model.predict(preprocess_input(np.array([im])))
#	print(aux[0][0] < aux[0][1])
#	input()

