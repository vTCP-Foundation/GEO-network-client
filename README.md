# How to build
1. Ensure requirements: `libboost`, `libsodium23`
1. `git clone git@github.com:GEO-Protocol/GEO-network-client.git geo-client`
1. `cd geo-client`
1. `cmake ./ && make`

Client build would be available in `./bin/` as `geo_network_client`.
<br/>
<br/>
<br/>

# How to use
First of all: **you do not need to build the client for your own**. <br/> 
Just use our [docker image](https://hub.docker.com/r/geoprotocol/network-client-beta) and you are ready to go.

Please, follow the [documentation](https://github.com/GEO-Protocol/Documentation/tree/master/client) link for the detailed instructions how to [run the node](https://github.com/GEO-Protocol/Documentation/tree/master/client/tutorials).
<br/>
<br/>
<br/>


# How to report issues
1. Please, use GitHub issues of this repo to describe your problem.
1. If possible, please attach the log of the node, that is usually located in `operations.log`. <br/> 
Please, make sure that the node is runninf in `DEBUG` mode so more information would be included in final report.
