#!/usr/bin/lua
function main()

m.log(1,"Starting l7-2-nftlb Lua Scripting  \n")

--capture remote addr from Connection
local remote_addr = m.getvar("REMOTE_ADDR") 

---send request to NFTLB with the origin IP.
---test purpose
---local sys = os.execute("curl http://172.16.64.53:5555/torta")
---it should work
local sys = os.execute("ssh apiuser@172.16.64.121 \"curl -X PUT -H 'Content-Type: application/json' http://localhost:5555/policies/l7filter -d '{ policies: [{ \"name\" : \"l7fiter\", \"elements\": [{ \"data\" : \" " ..remote_addr.. " \" }] }] }' \" ")
---local sys = os.execute("curl -X PUT -H 'Content-Type: application/json' http://172.16.64.121:5556/policies/l7filter -d '{ policies: [{ \"name\" : \"l7fiter\", \"elements\": [{ \"data\" : \" " ..remote_addr.. " \" }] }] }' ")


--save in log file information for debug purpose
local fileHandle = assert(io.open('/lua_output.txt','a'))
fileHandle:write("--- Output start ---\n")
fileHandle:write(remote_addr) 
fileHandle:write("\n --- Output end --- \n")
m.log(1,"Finishing l7-2-nftlb Lua Scripting\n")

end
