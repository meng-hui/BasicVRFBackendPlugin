#pragma once

#include <functional>
#include <boost/asio.hpp>

namespace BasicVRFBEPlugin
{
	class UDPNetwork final
	{
    public:
        using DataReceivedCallback = std::function<void(std::vector<char>)>;

        UDPNetwork(boost::asio::io_service& io_service, const boost::asio::ip::address& listen_address, unsigned short port);

        void registerCallback(DataReceivedCallback callback);

    private:
        void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);

        std::vector<DataReceivedCallback> callbacks_;
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint sender_endpoint_;
        static constexpr size_t max_length = 65000;
        std::vector<char> buffer;
	};
}