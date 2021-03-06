#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
using namespace std;


int sock = socket(AF_INET, SOCK_STREAM, 0);
int port = 54000;
string ipAddress = "127.0.0.1";
char buf[4096];
string userInput;
sockaddr_in hint;
int createSocket(){
    // Crear socket

       if (sock == -1)
       {
           return 1;
       }

       //Estructura para el servidor que se va a conectar

       hint.sin_family = AF_INET;
       hint.sin_port = htons(port);
       inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
       //Conectar al servidor con el socket
       int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
       if (connectRes == -1)
       {
           return 1;
       }

       //	While loop:

}


int stop()
{
 close(sock);
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)


{
    ui->setupUi(this);
    createSocket();


}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_boton0_clicked()
{
    //ui->source->text();
    int src= ui->source->value();
    int dest= ui->destination->value();
    string str = to_string(src);
    str.append(to_string(dest));



        //Enviar al servidor
        int sendRes = send(sock, str.c_str(), str.size() + 1, 0);
        if (sendRes == -1)
        {
            cout << "No se pudo enviar";

        }

        //Esperar respuesta
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            cout << "Hubo un error obteniendo la respuesta";
        }
        else
        {
            // Mostrar respuesta
            cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";

            QString qstr = QString::fromStdString(string(buf, bytesReceived));
            ui->label->setText(qstr);
        }


    //Cerrar el socket
    //stop();



}



