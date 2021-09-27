# Named-Pipes-Hangman

A C program using named pipes for a hangman game. The server waits for inputs through the named pipe and forks a process for each input. The child process sends its unique pipename to the client and starts the game. Parent process waits for new inputs. Server can support any number of clients in parallel. 

![image](https://user-images.githubusercontent.com/32169836/134988480-bfd3107d-4fae-4a6b-b7e2-83b818de7e81.png)
