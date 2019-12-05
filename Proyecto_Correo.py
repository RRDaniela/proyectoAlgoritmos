import time
from firebase import firebase
import smtplib #Simple mail transfer protocol, para enviar correos. 
server = smtplib.SMTP('smtp.gmail.com', 587)

firebase = firebase.FirebaseApplication("https://proyectoalgoritmos-d3ba4.firebaseio.com/", None)
temperatura = firebase.get('/Proyecto/Temperatura',None)

def send_mail():
    server = smtplib.SMTP('smtp.gmail.com', 587)
    server.ehlo()
    server.starttls()
    server.ehlo()
    server.login("rrdaniela99@gmail.com","rrdaniela123")
    subject='¡TEMPERATURA DEMASIADO ELEVADA!'
    body = 'Por favor revisa el estado actual de tu invernadero. '
    msg = f"subject: {subject}\n\n{body}"
    server.sendmail("invernadero@gmail.com","rrdaniela99@gmail.com",msg.encode('utf-8'))
    server.quit()

def consultar():
    if float(temperatura)>60:
        send_mail()

while(True):
    consultar()
    time.sleep(1000)