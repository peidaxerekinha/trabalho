#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define SERVER_HOST "www.hryatech.com"  // Altere para o domínio desejado
#define SERVER_PORT "443"             // Porta HTTPS
#define BIND_IP "192.168.1.78"       // IP específico para conexão


void handle_openssl_error() {
	ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
}
int main() {
    int sock;
    struct addrinfo hints, *res;
    struct sockaddr_in bind_addr;
    SSL_CTX *ctx;
    SSL *ssl;

    // Inicializa OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    // Cria contexto SSL
    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) handle_openssl_error();

    // Resolve o endereço do servidor
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(SERVER_HOST, SERVER_PORT, &hints, &res) != 0) {
        perror("Erro ao resolver endereço");
        exit(EXIT_FAILURE);
    }

    // Cria o socket
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    // Configura IP específico para conexão
    memset(&bind_addr, 0, sizeof(bind_addr));
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = inet_addr(BIND_IP);
    bind_addr.sin_port = 0; // Sistema escolhe a porta

    if (bind(sock, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) < 0) {
        perror("Erro ao associar o socket ao IP específico");
        exit(EXIT_FAILURE);
    }

    // Conecta ao servidor
    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
        perror("Erro ao conectar");
        exit(EXIT_FAILURE);
    }

      // Cria a conexão S
