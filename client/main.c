// This is a chat app program coded for the Course COMP3331 at UNSW
// This is the client app
//
// Author: Sean Yan
// zID: z5521807
//
// Acknowledegment
// All Code borrowed or taken from other resources will be comment before the 
// function or block section
//
//


// Stdlib
#include "connect-to-server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auth.h"

int main() {
    struct user* valid_users = load_credentials();

    connect_to_server();


    return 0;
}

