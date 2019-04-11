# War 

## Disclaimer

This Virus was not designed as a means to exact vengance on anyone.
I, Jefferson Le Quellec, take no responsibility for any damage that this software may cause.
Please be responsible and friendly.

### Infection

### Propagation

### Obfuscation

### Polymorphism

### Architecture


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

## Author

Jefferson Le Quellec 🐜  
Djelali Dinaut 🐜
