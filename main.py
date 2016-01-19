import serial
import pygame
from pygame.mixer import Sound
import time
import os.path
import random


class SocketControl(object):

    def __init__(self, connection):
        self.state = [0, 0, 0]
        self.map = [1, 2, 4]
        self.connection_name = connection
        self.port = serial.Serial(connection)

    def set_A(self, state):
        self.set(0, state)

    def set_B(self, state):
        self.set(1, state)

    def set_C(self, state):
        self.set(2, state)

    def set(self, idx, state):
        self.state[idx] = 1 if state else 0

    def send(self):
        val = map(lambda x, y: x*y, self.state, self.map)
        val = sum(val)
        self.port.write(chr(val))

    def close(self):
        self.port.close()


class SoundControl(object):

    def __init__(self, serial_port, data_folder):
        self.lightning = SocketControl(serial_port)
        self._data_folder = data_folder
        self._rain = Sound("%s/rain.wav" % data_folder)
        self._thunder = 0
        self._last = 0

    def rain(self):
        self._rain.play(-1)

    def exit(self, time):
        pygame.mixer.fadeout(time)

    # 0 - 6
    def thunder(self, inc):
        wait = random.randint(0, 2+inc)
        while (self._last == wait):
            wait = random.randint(0, 2+inc)

        self._last = wait
        print wait
        thunder = Sound("%s/thunder_0%d.wav" % (self._data_folder, wait))
        if wait < 6:
            self.lightning.set_C(ON)
            self.lightning.send()
            time.sleep(random.uniform(0.5, 1.5))
            self.lightning.set_C(OFF)
            self.lightning.send()
            time.sleep(wait)

        thunder.play()
        time.sleep(thunder.get_length()-0.3)
        thunder.fadeout(200)
        time.sleep(wait)

    def devil(self):
        self.lightning.set_C(ON)
        self.lightning.send()
        poodle = Sound("%s/poodle.wav" % (self._data_folder))
        poodle.play()
        while True:
            wait = random.randint(0, 8)
            while (self._last == wait):
                wait = random.randint(0, 8)
            print wait
            self._last = wait
            thunder = Sound("%s/thunder_0%d.wav" % (self._data_folder, wait))
            thunder.play()
            time.sleep(2)

# global constants
FREQ = 44100   # same as audio CD
BITSIZE = -16  # unsigned 16 bit
CHANNELS = 2   # 1 == mono, 2 == stereo
BUFFER = 1024  # audio buffer size in no. of samples
FRAMERATE = 30 # how often to check if playback has finished
SERIAL_STRING='/dev/cu.usbmodem1411'
DATA_FOLDER='data'
ON = True
OFF = False
A = 0
B = 1
C = 2

pygame.init()
pygame.mixer.init(FREQ, BITSIZE, CHANNELS, BUFFER)

sound = SoundControl(SERIAL_STRING, DATA_FOLDER)
sound.rain()

while(True):
    sound.thunder(6)
    #sound.devil()