# SmartDDoSFilter

Repository used for the Cybercamp 2018 event

## Tools

* Nftlb:

nftlb stands for nftables load balancer, the next generation linux firewall that will replace iptables is adapted to behave as a complete load balancer and traffic distributor.
nftlb is provided with a JSON API, so you can use your preferred health checker to enable/disable backends or virtual services and automate processed with it.

nftlb stands for nftables load balancer, the next generation linux firewall that will replace iptables is adapted to behave as a complete load balancer and traffic distributor.
nftlb is provided with a JSON API, so you can use your preferred health checker to enable/disable backends or virtual services and automate processed with it.

Dependencies:
- Netfilter & nftables as datapath
- libmnl to communicate with the kernel through netlink
- libev as events library for the web service
- libjson: JSON parser for the API

* wrk:

Tool to execute stress tests of HTTP backend applications

* Modsecurity:

Lua scripting integration with the nftlb API daemon.

## First Milestone:

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

## Lab description

Client: Several scripts will be used to attack backends through the Load Balancer.

Firewall/LB: The Load Balancer will be used as a firewall and a gateway, nftables/nftlb will provide those features.

2 Backends: They contain apache2 web server with the modsecurity module. The will use LUA scripting language to communicate with nftlb daemon.

# Nftlb Model

```JSON
  “farms” : [
      {
        // service parameters
         “name” : “lb01”,
         “family” : “ipv4”,                	// ipv4 / ipv6 / dual
         “virtual-addr” : “192.168.0.100”, 	// Virtual address
         “virtual-ports” : “80”,            // Service ports to listen
         “source-addr” : “192.168.0.101”, 	// no masquerade, use source nat address
         “mode” : “snat”,             	// snat / dnat / dsr
         “protocol” : “tcp”,          	// tcp / udp / sctp / all
         “helper” : “sip”,            	// sip / ftp/ tftp / …
         “scheduler” : “weight”,        // rr / weight / hash / symhash
										// (prio is used intrinsically)
         "state" : “up”,            	// up / down / off
         “mark” : “0x200”,            	// flow ct mark per virtual service

		*// security parameters*
        * “valid-tcp” : “on”,            		// Validate the TCP protocol*
        * “est-connlimit-saddr” : “10”,         // Set a limit of established connections for service*
        * “new-ratelimit-saddr” : “10”,         // Set a maximun rate for new connections*
        * “new-ratelimit-burst-saddr” : “10”,   // Allow a rate of extra new connections*
        * “rst-ratelimit-saddr” : “10”,         // Set a maximun rate for TCP reset packets*

         “backends” : [
            {
               "name" : “bck0”,        // backend ID
               "ip-addr" : "192.168.0.10",    // backend ip addr
               "ports" : "80",        // backend port (port or empty)
               "weight" : “5”,        // backend weight
               "priority" : “5”,        // backend prio
               "state" : “up”,        // up / down / off
               "mark" : "0x01",        // flow ct mark per backend
            },
         ],

         “policies” : [
            {
               "name" : “blacklist01”,    // policy name
            },
            {
               "name" : “whitelist01”,    // policy name
            }
         ],

      }
   ],
}
```
