import os
ppid = os.getpid()
newpid = os.fork()

if newpid == 0:
    import numpy as np
    from keras.models import load_model
    from keras.applications.mobilenet import preprocess_input
    import cv2
    import tensorflow as tf
    from tensorflow.python.keras.backend import set_session
    import signal
    model = load_model('megamodel.h5')

    def recieveSignal(signalNumber, frame):
        print('LOL')
        os.system('ffmpeg -f video4linux2 -i /dev/video2 -vframes 2 test%3d.jpeg')
        im = cv2.imread('test002.jpeg')
        im = cv2.resize(im, (224,224))
        x = preprocess_input(np.array([im]))
        aux = model.predict(x)
        print(aux[0][0] < aux[0][1])
        if aux[0][0] < aux[0][1]:
            os.kill(ppid, signal.SIGUSR1)
        else:
            os.kill(ppid, signal.SIGUSR2)
    signal.signal(signal.SIGUSR1, recieveSignal)
    while True:
        pass

else:
    from flask import Flask
    import signal
    app = Flask(__name__)
    stop = 0
    def recieveSignal(numberf, frame):
        global stop
        print(numberf)
        if numberf == signal.SIGUSR1:
            stop = 1
        else:
            stop = 2
    signal.signal(signal.SIGUSR1, recieveSignal)
    signal.signal(signal.SIGUSR2, recieveSignal)

    @app.route("/")
    def predictEnpoint():
        global stop
        os.kill(newpid, signal.SIGUSR1)
        while stop == 0:
            pass
        aux = stop
        stop = 0
        return str(aux)

    app.run('192.168.1.118', port=80)