# War 

## Disclaimer

This Virus was not designed as a means to exact vengance on anyone.
I, Jefferson Le Quellec, take no responsibility for any damage that this software may cause.
Please be responsible and friendly.

### Introduction

This project is part of the Virology branch at 42.

The idea behind War is to write a virus  (more evolved than famine) that is not about executing malicious payload but only about propagating itself in an organic/natural way and obfuscate has much possible his infection routine.

### Architecture

The flow of execution is shown below.

- The entry point is on the decryption routine, it will decrypt the whole infection routine. then pass the flow to the infection routine.

- The rip is now on the infection routine, it will choose its target and replicate itself.

- Now that the host is infected, we give back the control flow to the original entry point.
```
------------------------
|    header            | ------
------------------------      |
|                      |      |
|    segment header    |      |
|                      |      |
------------------------ <------------ (3)
|                      |      |      |
|                      |      |      |
|    text segment      |      |      |
|                      |      |      |
|                      |      |      |
------------------------ <----- (1)  |
|                      |             |
|    stub              |             |
|                      |             |
------------------------ ------      |
|                      |      |      |
|                      |      |      |
|    data segment      |      |      |
|                      |      |      |
|                      |      |      |
------------------------ <----- (2)  |
|                      |             |
|                      |             |
|    parasite          |             |
|                      |             |
|                      |             |
------------------------ -------------
```

### Infection

War contains two different types of infection. The first is the Segment Text, and the second is the PT_NOTE to PT_LOAD.

Segment text infection is used for the decryption routine, the size of the stub is 342 bytes. We can then inject this code between the TEXT_SEGMENT and DATA_SEGMENT since the stub is less than 1 page (4096 bytes).
If the size of the stub was more than 4096 bytes we would not have been able to inject it between those segment since we will have to increase the offset of the data segment and his virtual memory, which means that every interaction (segment text -> data segment) will be wronged.

PT_NOTE to PT_LOAD infection is used for the infection routine, the size of the routine is about 18000 bytes. We use the PT_NOTE segment which is not necessary for the runtime of the binary, and convert it to PT_LOAD, change his offset and virtual memory to reside after the data segment.
We can now inject the parasite inside the host.

### Propagation

War will infect different directory if you run it as root or as a simple user, but the way it chooses its target will be the same.

First, it will count the number of entries inside the predefined directories. Now that we know how many executables are inside the directories, it will choose a random integer that is in range of the maximum entries and save one executable for each directory. Then select, again randomly, one of those executables and start the infection routine. This way every executable has as many chances to be chosen.

War by default will try to infect as many executable as possible.

When a new binary is infected, the infector will execute it - which will start the whole (infection | propagation) routine. It means that famine has to infect only one executable on the system to be able to infect the whole system. Since the infected will do the work for me.

This execution flow will stop if the virus selects a binary that has already been infected or does not fit the criteria given by war.

** If you run it as simple user, it will only target binary inside `/tmp/test/` and `/tmp/test2/` **
** As root, `/bin/`, `/sbin/`,  `/usr/bin/` and `/usr/sbin/` **

### Obfuscation

The routine of infection as a very particular execution flow.

All function has the same parameters `struct s_host *host, struct s_keychain *keychain, enum e_context context`.
This is because the whole execution has to be linear, the functions can never return, they are all considered as `((no_return))`.

They all receive a context to know what happened before, but will still continue to execute, we always go forward with what we have.

The explanation behind that is because we wanted the program to be anti breakpoint software, to do that we encrypted every function with a different key, all stored inside `keychain`.
So every function is encrypted differently.

*Example: main is the key to foo, and foo is the key to bar ...*

The typical flow of execution will be like this:

You have one `__entry` and one `__exit`, both non-encrypted.
`__entry` is only here to back up the register and save the stack frame. 
`__exit` is here to retrieve the register, realign the stack frame and jump to the original entry point.

The *flip flop*:
- `function1` is encrypted so `__entry` will generate a key with the signature of itself and unencrypt `function1`.
- `rip` is now on `function1`, `function1` will generate a new key with itself and unencrypt `function2`.

Now, this is interesting.

- `function2` will re-encrypt `function1`, and continue the whole generate, decrypt, re-encrypt function after.
- So from a higher perspective, you will always have one function in clear. Functions only shows themselves when executed.

Just before the `__exit`, there is an autodestruction function, that will rewrite on the whole infection routine with random bytes.
The idea behind that is to let the person trying to reverse the binary thinks that it is still encrypted even though, it's now nothing but garbage.

```
__entry -> x -> x -> x -> __exit
__entry -> function1 -> x -> x -> __exit
__entry -> x -> function2 -> x -> __exit
__entry -> x -> x -> autodestruction -> __exit
__entry -> y -> y -> y -> __exit
```

### Polymorphism

To be able to achieve polymorphism, which is different from metamorphism ;

*Polymorphism: the encryption layer will change in each replication.*
*Metamorphism: the code itself will change in each replication.*

We modify the code with a simple metamorphism. We will now generate a key from the stub, it will now give us a different key in each replication, meaning which achieve polymorphism, via the metamorphism.

The encryption used for the first layer of encryption is RC4. In this code, we will create `placeholder` and we will inject random code (junk code) inside those placeholders. If we now create a hash of this function we will have a different key at each replication, and a different layer of encryption in each replication.

### Logger

By default (like Famine), War is compiled in debug mode which means it will log on `tmp/logger`. You can now trace the path of the virus.

## Author

Jefferson Le Quellec 🐜  
Djelali Dinaut 🐜 
