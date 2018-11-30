#!/usr/bin/lua
function main()

m.log(1,"Starting Lua Scripting  \n")

--capture remote addr from Connection
local remote_addr = m.getvar("REMOTE_ADDR") 

---send request to NFTLB with the origin IP. NFTLB remote host 172.16.64.53
---TODO: use libcurl instead. 
local sys = os.execute("curl -X PUT -H 'Content-Type: application/json' http://172.16.64.53:5555/policies/l7filter -d '{ policies: [{ \"name\" : \"l7filter\", \"elements\": [{ \"data\" : \" " ..remote_addr.. " \" }] }] }' ")

end
