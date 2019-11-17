import pandas as pd
import numpy as np
import os
import keras
import json
import matplotlib.pyplot as plt
from keras.layers import Dense,GlobalAveragePooling2D
from keras.applications import MobileNet
from keras.preprocessing import image
from keras.applications.mobilenet import preprocess_input
from keras.preprocessing.image import ImageDataGenerator
from keras.models import Model
from keras.optimizers import Adam
import cv2
base_model=MobileNet(weights=None,include_top=False) #imports the mobilenet model and discards the last 1000 neuron layer.
base_model.load_weights('mobilenet_1_0_224_tf_no_top.h5')

x=base_model.output
x=GlobalAveragePooling2D()(x)
x=Dense(1024,activation='relu')(x) #we add dense layers so that the model can learn more complex functions and classify for better results.
x=Dense(1024,activation='relu')(x) #dense layer 2
x=Dense(512,activation='relu')(x) #dense layer 3
preds=Dense(2,activation='softmax')(x) #final layer with softmax activation
model=Model(inputs=base_model.input,outputs=preds)
#specify the inputs
#specify the outputs
#now a model has been created based on our architecture
for layer in model.layers:
    layer.trainable=False
# or if we want to set the first 20 layers of the network to be non-trainable
for layer in model.layers[:20]:
    layer.trainable=False
for layer in model.layers[20:]:
    layer.trainable=True
model.compile(metrics=["accuracy"], loss="categorical_crossentropy",optimizer="adam")
model.summary()

count = 0
X = []
Y = []
for f in os.listdir('./data/'):
    print(count)
    count += 1
    if f.endswith('.json'):
        with open('./data/' + f, 'r') as fi:
            d = json.load(fi)
            b = False
            for elem in d['arr_boxes']:
                clase = elem['class']
                if clase == 'chaquetas' or clase == 'abrigos' or clase == 'cazadoras' or clase == 'bufandas':
                    b = True
            if b:
                Y.append(1)
            else:
                Y.append(0)

            im = cv2.imread('./fotos/' + d['file_name'])

            X.append(cv2.resize(im, (224,224)))


X = preprocess_input(np.array(X))
Y = keras.utils.to_categorical(np.array(Y), num_classes=2)

from sklearn.model_selection import train_test_split
xtrain, xtest, ytrain, ytest = train_test_split(X, Y)

print('pos jastaria')
model.fit(xtrain, ytrain, validation_data=(xtest, ytest), epochs=20, verbose=True)

model.save('megamodel.h5')
