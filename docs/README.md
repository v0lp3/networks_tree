# Examples

## IPv4
```
0
10.0.128.0/17
add lan00 2
assign lan00 y router1
add lan01 4 lan00 router1
assign lan01 n A
assign lan01 y router1
assign lan01 y router2
add lan02 1000 lan01 router1
add lan03 50 lan01 router1
add lan04 250 lan01 router2
add lan05 500 lan01 router2
assign lan02 n B
assign lan02 n C
assign lan03 n D
assign lan03 n E
assign lan04 n F
assign lan04 n G
assign lan05 n H
assign lan05 n J
``` 

## IPv6

```
1
2001:abcd::1234:0000/96
add lan00 10
assign lan00 y router1
assign lan00 n A
assign lan00 n B
add lan01 150000 lan00 router1
add lan02 500000 lan00 router1
add lan03 50000 lan00 router1
assign lan01 y router1
assign lan01 n C
assign lan01 n D
assign lan02 n E
assign lan02 n F
assign lan03 n G
assign lan03 n H
add lan04 2000000 lan01 router1
assign lan04 n I

```
