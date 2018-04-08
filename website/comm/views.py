from django.shortcuts import render
from django.http import HttpResponse,JsonResponse
from django.shortcuts import render
import socket
import json
import pandas as pd
import time
import numpy as np
from sklearn.externals import joblib
clf=joblib.load('model1.pkl')
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("",8080))
s.listen(1)
left = []
right=[]
chest=[]
# Create your views here.
def index(request):
	while(1):
		conn,addr = s.accept()
		message=conn.recv(1024)
		print(message.decode())
		if(message.decode().endswith('r')):
			print("Right")
			print(message.decode()[:-1])
			tmp = np.array(message[:-1].decode().split(','))
			print(tmp.astype(np.float));
			
			right.append(tmp.astype(float))
		elif(message.decode().endswith('c')):
			if(message.decode().endswith('c\r')):
				message.decode.splice(-1,2)
			print("Chest")
			tmp = np.array(message[:-1].decode().split(','))
			print(tmp.astype(np.float));
					
			chest.append(tmp.astype(np.float))
			print(message.decode()[:-1])
		else:
			print("left")
			print(message.decode()[:-1])
			tmp = np.array(message[:-1].decode().split(','))
			# print(tmp.astype(np.float));
			print(tmp[2].endswith('c\r'))
			left.append(tmp.astype(np.float))
		if(len(left)!=0 and len(chest)!=0 and len(right)!=0):
			printed = []	
			for i in range(len(chest)):
				printed.append(chest[0][i])
			for i in range(len(left)):
				printed.append(left[0][i])
			for i in range(len(right)):
				printed.append(right[0][i])
			print(np.array([left[0],right[0],chest[0]]).flatten())
			print(printed)
			print(np.concatenate([chest[0],left[0],right[0]]))
			y_pred=clf.predict(np.concatenate([chest[0],left[0],right[0]]).reshape(1,-1))
			print(y_pred)
			left.pop()
			right.pop()
			chest.pop()
			predictions = ['standing','Sitting','Lying down','Walking','Walking','Climbing Stairs','Waist bends forward','frontal elevation of arms','Knees bending (crouching)','Cycling','Jogging','Running','Jump front & back ']
			html = "<h1>You are %s</h1>"%predictions[y_pred[0]];
			# return JsonResponse({'res':json.dumps({'tm':pd.Series(y_pred).to_json(orient='values')})})
			return HttpResponse(html);