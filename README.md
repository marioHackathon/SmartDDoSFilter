# SmartDDoSFilter

Repository used for the Cybercamp 2018 event

## Tools

### Nftlb:

nftlb stands for nftables load balancer, the next generation linux firewall that will replace iptables is adapted to behave as a complete load balancer and traffic distributor.
nftlb is provided with a JSON API, so you can use your preferred health checker to enable/disable backends or virtual services and automate processed with it.

nftlb stands for nftables load balancer, the next generation linux firewall that will replace iptables is adapted to behave as a complete load balancer and traffic distributor.
nftlb is provided with a JSON API, so you can use your preferred health checker to enable/disable backends or virtual services and automate processed with it.

Dependencies:
- Netfilter & nftables as datapath
- libmnl to communicate with the kernel through netlink
- libev as events library for the web service
- libjansson: JSON parser for the API

### wrk:

Tool to execute stress tests of HTTP backend applications

### Modsecurity:

In Modsecurity, it is usedLua scripting for the integration with the nftlb API daemon.

## Tasks:

* * [x] Creating lab environment

* Create following attacks:
* * * [x] syn_flood
* * * [x] rst_flood

* Tools improvements
* * * [x] Modify wrk to set the source IP
* * * [ ] Modify wrk to allow config file

* Create firewall defend rules
* * * [x] Support to limit TCP requests based on rate IP source, util for atacks like "syn_flood".
* * * [x] Add nftlb infraestructure to support b/w lists.
* * * [x] L7 inspection (Apache+modsecurity) integrated with nftlb.

* Tests
* * * [x] Add funtional test for the new nftlb feature, limit the rate of new conns.
* * * [x] Add funtional test for the new nftlb feature, limit the rate of reset TCP packages per second.

## Lab description

Client: Several scripts will be used to attack backends through the Load Balancer. The client uses several IPs to simulate several clients.

Firewall/LB: The Load Balancer will be used as a firewall and a gateway, nftables/nftlb will provide those features.

2 Backends: They contain apache2 web server with the modsecurity module. The will use LUA scripting language to communicate with nftlb daemon.

### Compile nftlb

The operative system used is Debian Buster. In this Debian version, nftlib supports all features of nftlb, so it is not necessarty to compile handly the library. 

* Install required dependencies

`
apt-get update

apt-get install -y
    git
    bison
    flex
    binutils 
    build-essential 
    autoconf 
    libtool 
    pkg-config 
    libgmp-dev 
    libreadline-dev 
    libjansson-dev 
    libev-dev 
    cmake 
    curl 
    dnsutils 
    libmnl-dev 
    libnftnl-dev 
    libnftables-dev 
    libnftables0 
    libxtables-dev
`

* Download the most recent version of nftlb and install it

git clone https://github.com/zevenet/nftlb

cd nftlb

autoreconf -fi

./configure

make

make install

## Tests description

We have put special attention to prove that everything developed is working as expected, related with this, we are developing many shellscripts in order to perform TCP and HTTP flood attacks agains the system in order to check that the security apply to the system is working properly. Those scripts are:
* syn_flood
* rst_flood
* bogus_flood
* tcp_established_conn
* http_flood

We need to modify the repository of a stress testing tool called wrk in order to allow the tool to retrieve IPs from a config file.

# Nftlb API

```JSON
  "farms" : [
      {
         "name" : "lb01",
         "family" : "ipv4",                	
         "virtual-addr" : "192.168.0.100", 	
         "virtual-ports" : "80",            
         "source-addr" : "192.168.0.101", 
         "mode" : "snat",             	
         "protocol" : "tcp",          	
         "helper" : "sip",            	
         "scheduler" : "weight",        
         "state" : "up",            	
         "mark" : "0x200",            	

         "valid-tcp" : "on",         		// Validate the TCP protocol*
         "est-connlimit-saddr" : "10",         	// Set a limit of established connections for service*
         "new-ratelimit-saddr" : "10",         	// Set a maximun rate for new connections*
         "new-ratelimit-burst-saddr" : "10",   	// Allow a rate of extra new connections*
         "rst-ratelimit-saddr" : "10",        	// Set a maximun rate for TCP reset packets*

         "backends" : [
            {
               "name" : "bck0",        
               "ip-addr" : "192.168.0.10",
               "ports" : "80",        
               "weight" : "5",        
               "priority" : "5",      
               "state" : "up",        
               "mark" : "0x01",       
            },
         ],
	 
         "policies" : [
            {
               "name" : "blacklist01",    // policy name
            },
            {
               "name" : "whitelist01",    // policy name
            }
         ]
      }
   ],
}
```
