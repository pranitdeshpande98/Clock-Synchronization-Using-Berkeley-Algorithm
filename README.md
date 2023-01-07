# Clock-Synchronization-Using-Berkeley-Algorithm

Time-Synchronization-in-distributed-systems

A makefile has been created that has the following commands:
1. compileAll: make compileAll
  a. make compile writing “make compileAll” in terminal will create the executable objects for Berkeley Algorithm’s Server and Client Programs Respectively.
2. p2_serverRun: make p2_serverRun
  a. Writing “make p2_serverRun” In the terminal will run the server or daemon process to synchronize all the other client/ slave processes.
3. P2_clientRun: make p2_clientRun
  a. Writing “make p2_clientRun” in the terminal will run the client/slave processes for clock synchronization. Multiple terminals can be opened, and the same command can be inserted into each of these terminals. Make sure you open the client terminals with the equal count that you enter as your total processes in the server terminal. Running the command will also execute the distributed locking scheme by showing the counter getting updated every time a new process shares the file counter.txt. This command must be written only after executing the server Run.
