# Networks tree
> Build your network in an easy way

<p align="center">
      <img
            src="https://raw.githubusercontent.com/v0lp3/networks_tree/master/docs/show.png"
            alt="show command"
      /> 
 </p>

> A subnetwork or subnet is a logical subdivision of an IP network. The practice of dividing a network into two or more networks is called subnetting.

`Networks tree` is a software that automate IPv4 subnetting, after you have designed your networks, you can output the entire structure and put the static configuration files in your devices. [More info](https://wiki.debian.org/NetworkConfiguration)

[Here](https://github.com/v0lp3/networks_tree/tree/master/output) an example of output, result of [this commands](https://github.com/v0lp3/networks_tree/blob/master/docs/test_config.txt).

### Requirements

- Compiler with C++11 support

### Build

```
git clone https://github.com/v0lp3/networks_tree.git
cd networks_tree
g++ main.cpp -o networks_tree
```

### Run

```
./networks_tree
```