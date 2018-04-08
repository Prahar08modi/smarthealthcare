from django.shortcuts import render
from django.http import HttpResponse,JsonResponse
from django.shortcuts import render
import socket
import time
# Create your views here.
def index(request):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	print(socket.gethostname())
	s.bind(("",8080))
	s.listen(1)
	while(1):
		conn,addr = s.accept()
		message=conn.recv(1024)
		context = {'mes':message}	
		return 	JsonResponse({'mes':message.decode()[:-1]})
