#include "UDPNetwork.h"
#include <boost/bind/bind.hpp>

BasicVRFBEPlugin::UDPNetwork::UDPNetwork(boost::asio::io_service& io_service, const boost::asio::ip::address& listen_address, unsigned short port)
	: socket_(io_service), buffer(max_length)
{
    // Create the socket so that multiple may be bound to the same address.
    boost::asio::ip::udp::endpoint listen_endpoint(listen_address, port);
    socket_.open(listen_endpoint.protocol());
    socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.set_option(boost::asio::socket_base::broadcast(true));
    socket_.bind(listen_endpoint);

    socket_.async_receive_from(
        boost::asio::buffer(buffer, max_length), sender_endpoint_,
        boost::bind(&UDPNetwork::handle_receive_from, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void BasicVRFBEPlugin::UDPNetwork::registerCallback(DataReceivedCallback callback)
{
    callbacks_.push_back(callback);
}

void BasicVRFBEPlugin::UDPNetwork::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
{
    if (!error)
    {
        for (const auto& callback : callbacks_)
        {
            callback(buffer);
        }
    }

    // Start another cycle of async receive
    socket_.async_receive_from(
        boost::asio::buffer(buffer, max_length), sender_endpoint_,
        boost::bind(&UDPNetwork::handle_receive_from, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

    // Reset buffer to prevent irregular characters from occurring during the next receive operation
    std::fill(std::begin(buffer), std::end(buffer), 0);
}
