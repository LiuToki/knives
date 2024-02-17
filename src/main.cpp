#include <iostream>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4819)
#endif
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

class Server {
	boost::asio::io_service& m_ioService;
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::asio::ip::tcp::socket m_socket;
	boost::asio::streambuf m_rcvBuf;

public:
	Server(boost::asio::io_service& io_service)
		: m_ioService(io_service),
		  m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 31400)),
		  m_socket(io_service) {}

	void start()
	{
		StartAccept();
	}

private:
	// 接続待機.
	void StartAccept()
	{
		m_acceptor.async_accept(
			m_socket,
			boost::bind(&Server::OnAccept, this, boost::asio::placeholders::error));
	}
	
	// 接続待機完了.
	void OnAccept(const boost::system::error_code& error)
	{
		if (error) {
			std::cout << "accept failed: " << error.message() << std::endl;
			return;
		}

		start_receive();
	}

	// メッセージ受信
	void start_receive()
	{
		boost::asio::async_read(
			m_socket,
			m_rcvBuf,
			boost::asio::transfer_all(),
			boost::bind(&Server::on_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}

	// 受信完了
	// error : エラー情報
	// bytes_transferred : 受信したバイト数
	void on_receive(const boost::system::error_code& error, size_t /*bytes_transferred*/)
	{
		if (error && error != boost::asio::error::eof) {
			std::cout << "receive failed: " << error.message() << std::endl;
		}
		else {
			const char* data = boost::asio::buffer_cast<const char*>(m_rcvBuf.data());
			std::cout << data << std::endl;

			m_rcvBuf.consume(m_rcvBuf.size());
		}
	}
};

int main()
{
	boost::asio::io_service ioService;

	Server server(ioService);

	server.start();

	ioService.run();
}