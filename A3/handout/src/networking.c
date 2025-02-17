#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#ifdef _APPLE_
#include "./endian.h"
#else
#include <endian.h>
#endif

#include "./networking.h"
#include "./sha256.h"

char server_ip[IP_LEN];
char server_port[PORT_LEN];
char my_ip[IP_LEN];
char my_port[PORT_LEN];

int c;

/*
 * Gets a sha256 hash of specified data, sourcedata. The hash itself is
 * placed into the given variable 'hash'. Any size can be created, but a
 * a normal size for the hash would be given by the global variable
 * 'SHA256_HASH_SIZE', that has been defined in sha256.h
 */
void get_data_sha(const char* sourcedata, hashdata_t hash, uint32_t data_size, 
    int hash_size)
{
  SHA256_CTX shactx;
  unsigned char shabuffer[hash_size];
  sha256_init(&shactx);
  sha256_update(&shactx, sourcedata, data_size);
  sha256_final(&shactx, shabuffer);

  for (int i=0; i<hash_size; i++)
  {
    hash[i] = shabuffer[i];
  }
}

/*
 * Gets a sha256 hash of specified data file, sourcefile. The hash itself is
 * placed into the given variable 'hash'. Any size can be created, but a
 * a normal size for the hash would be given by the global variable
 * 'SHA256_HASH_SIZE', that has been defined in sha256.h
 */
void get_file_sha(const char* sourcefile, hashdata_t hash, int size)
{
    int casc_file_size;

    FILE* fp = Fopen(sourcefile, "rb");
    if (fp == 0)
    {
        printf("Failed to open source: %s\n", sourcefile);
        return;
    }

    fseek(fp, 0L, SEEK_END);
    casc_file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char buffer[casc_file_size];
    Fread(buffer, casc_file_size, 1, fp);
    Fclose(fp);

    get_data_sha(buffer, hash, casc_file_size, size);
}

/*
 * Combine a password and salt together and hash the result to form the 
 * 'signature'. The result should be written to the 'hash' variable. Note that 
 * as handed out, this function is never called. You will need to decide where 
 * it is sensible to do so.
 */
void get_signature(char* password, char* salt, hashdata_t* hash)
{
    // Your code here. This function has been added as a guide, but feel free 
    // to add more, or work in other parts of the code


    char *passwordWithSalt[strlen(password) + strlen(salt)];

    char *passwordWithSalt = malloc(strlen(password) + strlen(salt) + 1);
    strcpy(passwordWithSalt, password);
    strcat(passwordWithSalt, salt);
    




    //PIGERNES KODE!!!
    //*******************************************************************
    char to_hash[strlen(password) + strlen(salt)];

    memcpy(to_hash, password, strlen(password));
    memcpy(to_hash+strlen(password), salt, strlen(salt));
    get_data_sha(to_hash, hash, strlen(to_hash), SHA256_HASH_SIZE);
    //*******************************************************************
}

/*
 * Register a new user with a server by sending the username and signature to 
 * the server
 */
void register_user(char* username, char* password, char* salt)
{
    // Your code here. This function has been added as a guide, but feel free 
    // to add more, or work in other parts of the code





    //Pigernes kode
    //************************************************************************************************
    hashdata_t hash; //Hash should come from get signature
    int clientfd;
    rio_t rio;
    char buf[MAXLINE];

    char* host = server_ip;
    char* port = server_port;

    get_signature(password, salt, hash); //hash from get signature is not saved right now

    struct RequestHeader to_send;
    // to_send.username = username;
    // to_send.salted_and_hashed = hash;

    memcpy(to_send.username, username, USERNAME_LEN);
    memcpy(to_send.salted_and_hashed, hash, 32);
    memcpy(to_send.length, 0, 4);

    memcpy(buf, &to_send, MAX_MSG_LEN);
    clientfd = open_clientfd(host, port); // Opens connection to server at <hostname, port>
    Rio_readinitb(&rio, clientfd);

    printf("Clientfd value: %d\n", clientfd);
    Rio_writen(clientfd, buf, MAX_MSG_LEN); //says "bad File descripter" that is clientfd
    Rio_readnb(&rio, buf, MAX_MSG_LEN); // says "bad File descripter" that is clientfd
    char *test = malloc(100);
    memcpy(test, &buf[RESPONSE_HEADER_LEN], 100);
    printf("%s", test);
    // close(clientfd); //line:netp:echoclient:close
    // free(buf);
    //free(to_send);    

    //*********************************************************************************************
}

/*
 * Get a file from the server by sending the username and signature, along with
 * a file path. Note that this function should be able to deal with both small 
 * and large files. 
 */
void get_file(char* username, char* password, char* salt, char* to_get)
{
    // Your code here. This function has been added as a guide, but feel free 
    // to add more, or work in other parts of the code
}

int main(int argc, char **argv)
{
    // Users should call this script with a single argument describing what 
    // config to use
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <config file>\n", argv[0]);
        exit(EXIT_FAILURE);
    } 

    // Read in configuration options. Should include a client_directory, 
    // client_ip, client_port, server_ip, and server_port
    char buffer[128];
    fprintf(stderr, "Got config path at: %s\n", argv[1]);
    FILE* fp = Fopen(argv[1], "r");
    while (fgets(buffer, 128, fp)) {
        if (starts_with(buffer, CLIENT_IP)) {
            memcpy(my_ip, &buffer[strlen(CLIENT_IP)], 
                strcspn(buffer, "\r\n")-strlen(CLIENT_IP));
            if (!is_valid_ip(my_ip)) {
                fprintf(stderr, ">> Invalid client IP: %s\n", my_ip);
                exit(EXIT_FAILURE);
            }
        }else if (starts_with(buffer, CLIENT_PORT)) {
            memcpy(my_port, &buffer[strlen(CLIENT_PORT)], 
                strcspn(buffer, "\r\n")-strlen(CLIENT_PORT));
            if (!is_valid_port(my_port)) {
                fprintf(stderr, ">> Invalid client port: %s\n", my_port);
                exit(EXIT_FAILURE);
            }
        }else if (starts_with(buffer, SERVER_IP)) {
            memcpy(server_ip, &buffer[strlen(SERVER_IP)], 
                strcspn(buffer, "\r\n")-strlen(SERVER_IP));
            if (!is_valid_ip(server_ip)) {
                fprintf(stderr, ">> Invalid server IP: %s\n", server_ip);
                exit(EXIT_FAILURE);
            }
        }else if (starts_with(buffer, SERVER_PORT)) {
            memcpy(server_port, &buffer[strlen(SERVER_PORT)], 
                strcspn(buffer, "\r\n")-strlen(SERVER_PORT));
            if (!is_valid_port(server_port)) {
                fprintf(stderr, ">> Invalid server port: %s\n", server_port);
                exit(EXIT_FAILURE);
            }
        }        
    }
    fclose(fp);

    fprintf(stdout, "Client at: %s:%s\n", my_ip, my_port);
    fprintf(stdout, "Server at: %s:%s\n", server_ip, server_port);

    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
    char user_salt[SALT_LEN+1];
    
    fprintf(stdout, "Enter a username to proceed: ");
    scanf("%16s", username);
    while ((c = getchar()) != '\n' && c != EOF);
    // Clean up username string as otherwise some extra chars can sneak in.
    for (int i=strlen(username); i<USERNAME_LEN; i++)
    {
        username[i] = '\0';
    }
 
    fprintf(stdout, "Enter your password to proceed: ");
    scanf("%16s", password);
    while ((c = getchar()) != '\n' && c != EOF);
    // Clean up password string as otherwise some extra chars can sneak in.
    for (int i=strlen(password); i<PASSWORD_LEN; i++)
    {
        password[i] = '\0';
    }

    // Note that a random salt should be used, but you may find it easier to
    // repeatedly test the same user credentials by using the hard coded value
    // below instead, and commenting out this randomly generating section.
    for (int i=0; i<SALT_LEN; i++)
    {
        user_salt[i] = 'a' + (random() % 26);
    }
    user_salt[SALT_LEN] = '\0';
    //strncpy(user_salt, 
    //    "0123456789012345678901234567890123456789012345678901234567890123\0", 
    //    SALT_LEN+1);

    fprintf(stdout, "Using salt: %s\n", user_salt);

    // The following function calls have been added as a structure to a 
    // potential solution demonstrating the core functionality. Feel free to 
    // add, remove or otherwise edit. 

    // Register the given user
    register_user(username, password, user_salt);

    // Retrieve the smaller file, that doesn't not require support for blocks
    get_file(username, password, user_salt, "tiny.txt");

    // Retrieve the larger file, that requires support for blocked messages
    get_file(username, password, user_salt, "hamlet.txt");

    exit(EXIT_SUCCESS);
}