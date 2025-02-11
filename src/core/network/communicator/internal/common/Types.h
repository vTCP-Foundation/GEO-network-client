#ifndef VTCPD_NETWORK_COMMON_TYPES_H
#define VTCPD_NETWORK_COMMON_TYPES_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/signals2.hpp>

#include <string>
#include <chrono>


using IOCtx = boost::asio::io_context;

using UDPSocket = boost::asio::ip::udp::socket;
using UDPEndpoint = boost::asio::ip::udp::endpoint;

using TCPSocket = boost::asio::ip::tcp::socket;
using TCPEndpoint = boost::asio::ip::tcp::endpoint;
using TCPResolver = boost::asio::ip::tcp::resolver;

namespace signals = boost::signals2;


using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;


using Host = std::string;
using Port = uint16_t;

using namespace boost::placeholders;

#endif // VTCPD_NETWORK_COMMON_TYPES_H
