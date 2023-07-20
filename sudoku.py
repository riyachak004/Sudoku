#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jul 12 21:12:04 2022

@author: riyachakravarty
"""


import numpy as np
            

question = np.zeros((9,9,1), dtype=float)
solution = np.zeros((9,9,1), dtype=float)
list_q = []
list_s = []
with open('sudokueasy.txt') as file:
    for readline in file: 
        line = readline.rstrip('\n')
        list = (line.split(',')) 
        row = 0
        col = 0
        for chars in list[0]:
            question[row,col,0] = (float(chars)/9) - 0.5
            col = (col + 1) % 9 
            if (col == 0):
                row = row + 1
        list_q.append(question)
        row = 0
        col = 0
        for chars in list[1]:
            solution[row,col,0] = chars
            col = (col + 1) % 9 
            if (col == 0):
                row = row + 1
        list_s.append(solution)
    


    
print("question:")
print(len(list_q))
print("answer")
print(len(list_s))


model = keras.models.Sequential()

model.add(Conv2D(64, kernel_size=(3,3), activation='relu', padding='same', input_shape=(9,9,1)))
model.add(BatchNormalization())
model.add(Conv2D(64, kernel_size=(3,3), activation='relu', padding='same'))
model.add(BatchNormalization())
model.add(Conv2D(128, kernel_size=(1,1), activation='relu', padding='same'))

model.add(Flatten())
model.add(Dense(81*9))
model.add(Reshape((-1, 9)))
model.add(Activation('softmax'))

model.compile(loss='sparse_categorical_crossentropy', optimizer='adam')
    