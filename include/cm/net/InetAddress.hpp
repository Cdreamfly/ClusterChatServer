#pragma once

#include "cm/base/Copyable.hpp"

#include <arpa/inet.h>
#include <string>

namespace cm::net {
	class InetAddress : public Copyable {
	public:
		explicit InetAddress(uint16_t port = 0, const std::string& ip = "127.0.0.1");

		explicit InetAddress(const sockaddr_in &addr) : addr_(addr) {}

		[[nodiscard]] std::string toIp() const;

		[[nodiscard]] std::string toIpPort() const;

		[[nodiscard]] uint16_t toPort() const;

		[[nodiscard]] const sockaddr_in *getSockAddr() const { return &addr_; }

		void setSockAddr(const sockaddr_in &addr) { addr_ = addr; }

		[[nodiscard]] sa_family_t family() const { return addr_.sin_family; }

		[[nodiscard]] const sockaddr *getSocketAddr() const { return (const sockaddr *) &addr_; }

		void setSocketAddr(const sockaddr_in &addr) { addr_ = addr; }

	private:
		sockaddr_in addr_{};
	};
}