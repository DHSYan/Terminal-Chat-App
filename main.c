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
//

int main() {
    struct user* test = load_credentials(); 
    print_all_valided_user(test, 10);
    return 0;
}



// References
