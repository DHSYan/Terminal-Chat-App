// This is a chat app program coded for the Course COMP3331 at UNSW.
// Author: Sean Yan
// zID: z5521807
//
// Acknowledegment
// All Code borrowed or taken from other resources will be comment before the 
// function or block section
//
//


// Stdlib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// My Own Modules
#include "auth.h"
#include "TCPServer.h"

// This has to interact with the client
int main() {
    // struct user* test = load_credentials(); 
    // print_all_valided_user(test);
    //
    //
    //
    // while(login(test, 2) != true);
        
    init_server();

    return 0;
}



// References
//
// Auth.c 
    // reading a file: https://www.youtube.com/watch?v=fLPqn026DaE
    // struct user* valid_users = malloc(sizeof(struct user));
