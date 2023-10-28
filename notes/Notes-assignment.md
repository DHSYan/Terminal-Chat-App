---
id: "Notes-assignment"
aliases:
  - "3331 Assignment - Chat App"
tags: []
---

# 3331 Assignment - Chat App


## Requirments

### Base Specification (20 Marks) (Section 3.2 - 3.3)
#### First Part (Self Contained, 15 Marks)
> Basic Interaction between client and server

#### Second Part (5 Marks)
> Additional functionality whereby two clients can upload/doawnload video
files to each other directly in a peer-to-peer fashion via UDP

### "this assignment includes 2 major modules"
1. Server Program
2. Client Program

### List of requirement listed in Page 2, point 3 Assignment spec
1. > Text Message must communicatie over TCP to the _server_
2. > Client communicate video fiels in UDP _themsevles_
3. > Authenticating a User
4. > Exchanging messages with the server
5. > Send Private Message to another particular participant
6. > read message from server
7. > Read Active Users' Information
8. > Upload Video files from one user to another user.

> (You will implement the application protocol to impelment these functions)

- > The server will listen on a port speficied as the CLI Argument, 
will wait for a client to connect
- > Client init a TCP Connection with the server.
- > The user will init the authunication progess
- > Client is implement in a CLI Interafce 
- > After auth, user will init one of the availble commmand
- > ALl commands require a simple request response interaction between the
client and server or two clients (P2P).
- > The User may exeecute a series of command ( one after the other) and 
eventually quit 
  - > [!faq] Question #question
    > what does one after the other mean? Does this mean that the user's 
      program won't terminate just after one command, instead the user will 
      be brought back to the main menu of the application?


### 3.2 Authentication
- > Server promnt the user to input the username and password 
- > Valid Username and password combination stored a file called `credential.txt`
which will be in the same directory as the server program.
- > Username and Password are case-sensitive
- > Each Usure name and password will be on a separet line and that there 
will be one white space between the two
  - > [!faq] Question #question
    > does this look like
      ```
      username1
      password1

      username2
      password2
      ```
      or
      ```
      username1
      password1
      username2
      password2
      ```
    <!-- - Probably the second one eh? -->


- > If invalid credential entered, the user is prompted to retry
- > After many _consectutive_ failed attemps, the user is blocked for 1s second
  - > The $number$ is an int that supplied to the command line argument and
    is between 1 and 5
    - > If Invalid $number$, print message 
- > The USer can't be logged in duriing 10 sec, _even from differenet IP_
- > after sucessful login, the client should next send the UDP port Number 
that it is listening to the server
  - > [!faq] #question
    > What does this mean? the user will listening to server?
    > does this mean that the user opens a port that accepts UDP Messages?
- > The server record a timestamp of teh user loginning in event, the username,
ip addres and port number that the client liesten to in the acitve user log 
file (`userlog.txt`)
   -  ```
      Active user sequence number; timestamp; username; client IP Addr; 
      client UDP Server port number

      1; 01 Jun 2022 21:30:04; Yoda; 129.64.1.11; 6666
      ```

- multiple logins concurrently are not allowed (not tested)

### 3.3 Comands


### Restrictions
1. the Only library allow ar the Standard Libraries





### need to be learned
1. Multithreading
  - for Blocking user, because once someone gets block, we have to start a 
  timer aside, for 10 sec, but other user should be able to login in



### What _DOESN"T_ need to be done
1. catupre and Display Live Video
  - We will just tramsmit and recieve video files

# Notes
1. All login function should probably take inputs, param in terms of texts
