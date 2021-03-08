# Networks tree
> Build your network in an easy way

<p align="center">
      <img
            src="https://raw.githubusercontent.com/v0lp3/networks_tree/master/docs/show.png"
            width=50%
            height=50%
            alt="show command"
      /> 
 </p>

> A subnetwork or subnet is a logical subdivision of an IP network. The practice of dividing a network into two or more networks is called subnetting.

`Networks tree` is a software that automate IPv4/IPv6 subnetting, you can simulate a real networks infrastructure scenario, and building that without any waste bits. [Static address configuration](https://wiki.debian.org/NetworkConfiguration)

### Examples

Two examples (IPv4 & IPv6) are available in the [docs/README](https://github.com/v0lp3/networks_tree/blob/master/docs/README.md)

### Requirements

- Compiler with C++11 support

### Build

```
git clone https://github.com/v0lp3/networks_tree.git
cd networks_tree
g++ src/main.cpp -o networks_tree
```

### Run

```
./networks_tree
```
