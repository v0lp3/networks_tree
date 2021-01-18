# Network example

```
Ip address in output file are assigned by random function, you are freely to change they, with anothers valid, if you want.
```

### Structure


```

                                                         ----lan02---B---C
                                                         |
                                        -----ROUTER1-----           
                                       /                 |
                                      /                  ----lan03---D---E
                                  A  /
                                  | /              
GATEWAY---lan00---ROUTER0---lan01---                 
                                    \
                                     \                  
                                      \                   ----lan04--F--G
                                       \                 |
                                        -----ROUTER2-----
                                                         |
                                                          ----lan05--H--J
```
### Requirements

The subnet will have different capacity:

- `lan02`: 1000 hosts  
- `lan03`: 50 hosts  
- `lan04`: 250 hosts  
- `lan05`: 500 hosts



