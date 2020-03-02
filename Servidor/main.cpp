#include <QCoreApplication>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    // Crear el socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "No se pudo crear el socket";
        return -1;
    }

    // Enlazar el socket a una IP
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint))==-1)
    {
        cerr << "No se pudo enlazar a un IP/port";
        return -2;

    }

    // Maracr el socket para qe escuche
    if (listen(listening, SOMAXCONN)==-1)
    {
        cerr << "No se puede escuchar";
        return -3;

    }

    // Aceptar la llamada
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);

    if (clientSocket == -1)
    {
        cerr << "Problema con la conexión del cliente";
        return -4;
    }
    close(listening);
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client),host, NI_MAXHOST, svc, NI_MAXSERV,0);
    if (result)
    {
        cout << host << "connected on" << svc << endl;
    }
    else
    {
        inet_ntop(AF_INET,&client.sin_addr, host, NI_MAXHOST);
        cout << host << "connected on" << ntohs(client.sin_port) << endl;
    }


    // Cerrar el socket que escucha
    // Mientras recibe, lo muestra, echo
    char buf[4096];
    while(true)
    {
        // Limpiar buffer
        memset(buf, 0, 4096);
        // Esperar msj
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1)
        {
            cerr << "Huboun problema de conexión" << endl;
            break;
        }

        if (bytesRecv==0)
        {
            cout << " CLiente se desconectó" << endl;
            break;

        }

        // Mostrar msj
        cout << "Recibido: " << string(buf, 0, bytesRecv) << endl;

        // Reenviar msj
        send(clientSocket, buf, bytesRecv+1, 0);



    }
    // Cerrar el socket
    close(clientSocket);


    QCoreApplication a(argc, argv);

    return a.exec();
}
